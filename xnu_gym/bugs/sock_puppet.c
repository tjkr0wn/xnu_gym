#include <pongo.h>
#include <defs.h>

extern void pretty_log(char *m, int err);

//It's ugly but I put this in a global scope so that I can do some debug stuff
struct mach_header_64 *main_header;
xnu_pf_range_t *TEXTEXEC;

static bool handle_in6_pcbdetach(struct xnu_pf_patch* patch, void* cacheable_stream) {
  printf("%s: Entered matchhandler\n", __func__);
  xnu_pf_disable_patch(patch);

  debug("TEXTEXEC->cacheable_base: %p\n", 1, (void *) TEXTEXEC->cacheable_base);
  debug("Offset from __TEXT_EXEC: %d\n", 1, (void *) TEXTEXEC->cacheable_base - (void *) cacheable_stream);

  DumpHex(cacheable_stream - 7, 21);

  uint32_t *opcode_stream = cacheable_stream;

  bool found_mov = false;
  bool found_patch = false;

  /*
  Now we have to scroll back until we find the sequence where the former ip6_freepcbopts is embedded;

  ip6_clearpktopts(pktopt, -1); <== We can easily look for this, as the arg is a simple mov with a negative value,
  and there aren't any negative values used closer to the entry than this. The instruction we look for is mov xX,#-0x1

	FREE(pktopt, M_IP6OPT);
  */

  /*
  LAB
  f007e27630 3601 00b4     cbz       x22,LAB_fffffff007e27654
  f007e27634 e003 16aa     mov       x0,x22
  f007e27638 0100 8012     mov       w1,#0xffffffff
  f007e2763c 0acb ff97     bl        FUN_fffffff007e1a264                       undefined FUN_fffffff007e1a2
  f007e27640 80c7 fff0     adrp      x0,-0xff88e6000
  f007e27644 0020 1591     add       x0=>PTR_PTR_DAT_fffffff00771a548,x0,#0x548 = fffffff00771a570
  f007e27648 e103 16aa     mov       x1,x22
  f007e2764c 0200 8092     mov       x2,#-0x1
  f007e27650 649b f197     bl        FUN_fffffff007a8e3e0                       undefined FUN_fffffff007a8e3
  */

  /*
    0x92800000 = mov xX, #-0x1
    & val = 0xFFFFFFE0, will include opcode, shift, and the immediate (-1)
  */

  for (int i = 0; i < 50; i++) {
    if ((*opcode_stream & 0xFFFFFFE0) == 0x92800000) {
      found_mov = true;
      break;
    }
    opcode_stream--;
  }

  if (!found_mov) {
    pretty_log("Couldn't find a negative immediate for a mov instr!", FAIL);
    SPIN();
  }

  debug("Found an instr with - val!\n", 0);

  /*
  Lastly, we have to increment the stream to find where inp->in6p_outputopts is NULL'ed. This is extremely trivial
  and can be found simply by looking for a str xzr,[xX, #0xXXX].
  */

  /*
    0xF900001F = str xzr,[xX, #0xXXX]
    & val = 0xFFC0001F, will include opcode, store register
  */

  for (int i = 0; i < 25; i++) {
    if ((*opcode_stream & 0xFFC0001F) == 0xF900001F) {
      found_patch = true;
      break;
    }
    opcode_stream++;
  }

  if (!found_patch) {
    pretty_log("Couldn't find inp->in6p_outputopts == NULL!", FAIL);
    SPIN();
  }

  debug("Found inp->in6p_outputopts == NULL!\n", 0);

  /*
    And we just replace it with a nop :)
    mov x0, x0 = 0x52800001
  */


  *opcode_stream = 0x52800001;

  pretty_log("Done with handling in6_pcbdetach!", PATCH);

  return true;
}

int sock_puppet_all_callback() {
  pretty_log("Doing sock_puppet patches. Transferring output to match handlers...", INFO);

  xnu_pf_patchset_t *patchset = xnu_pf_patchset_create(XNU_PF_ACCESS_32BIT);
  main_header = xnu_header();
  TEXTEXEC = xnu_pf_segment(main_header, "__TEXT_EXEC");

  if (!patchset || !main_header || !TEXTEXEC) {
    pretty_log("Got a NULL value!", FAIL);
    return 1;
  }

  /*
     I could check for iOS 12 here, but iOS 13+ is supposed to be
     supported, so no need for a check.
  */


  /*
    MATCHES
      TESTED VERSIONS
        iOS 14.5.1 18E212, iPhone10,1
  */
  uint64_t in6_pcbdetach_opcodes[] = {
    0xF9400000,     //ldr       xX,[xX, #0xXXX]
    0xF900001F,     //str       xzr,[xX, #0xXXX]
    0xF9400000,     //ldr       xX,[xX, #0xXXX]
    0xF900001F,     //str       xzr,[xX, #0xXXX]
    0xAA1303E0,     //mov       x0,x19
    0x52800001,     //mov       w1,#0x0
    0x94000000      //bl        xxx
  };

  const size_t in6_pcbdetach_opcodes_c =
      sizeof(in6_pcbdetach_opcodes) /
      sizeof(*in6_pcbdetach_opcodes);

  uint64_t in6_pcbdetach_opcodes_masks[] = {
    0xFFC00000, /*mask all*/
    0xFFC0001F, /*mask base register and offset*/
    0xFFC00000, /*mask all*/
    0xFFC0001F, /*mask base register and offset*/
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFC000000 /*mask immediate*/
  };

  xnu_pf_maskmatch(
            patchset,
            "in6_pcbdetach_matcher",
            in6_pcbdetach_opcodes,
            in6_pcbdetach_opcodes_masks,
            in6_pcbdetach_opcodes_c,
            false,
            handle_in6_pcbdetach
  );

  xnu_pf_emit(patchset);

  xnu_pf_apply(TEXTEXEC, patchset);

  xnu_pf_patchset_destroy(patchset);

  return 0;
}
