#include <pongo.h>
#include <defs.h>

extern void pretty_log(char *m, int err);

static bool handle_in6_pcbdetach(struct xnu_pf_patch* patch, void* cacheable_stream) {
  printf("%s: Entered matchhandler\n", __func__);
  xnu_pf_disable_patch(patch);

  uint32_t *opcode_stream = cacheable_stream;

  bool found_mov = false;
  bool found_patch = false;

  /*

  Now we have to scroll back until we find the sequence where the former ip6_freepcbopts is embedded;

  ip6_clearpktopts(pktopt, -1); <== We can easily look for this, as the arg is a simple mov with a negative value,
  and there aren't any negative values used closer to the entry than this. The instruction we look for is mov xX,#-0x1

	FREE(pktopt, M_IP6OPT);

  */

  //0x92800000 = mov xX, #-0x1 0x92800001
  //& val = 0xFFFFFFE0, will include opcode, shift, and the immediate (-1)
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

  DEBUG("Found an instr with - val!");

  /*

  Lastly, we have to increment the stream to find where inp->in6p_outputopts is NULL'ed. This is extremely trivial
  and can be found simply by looking for a str xzr,[xX, #0xXXX].

  */

  //0xF900001F = str xzr,[xX, #0xXXX]
  //& val = 0xFFC0001F, will include opcode, store register
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

  DEBUG("Found inp->in6p_outputopts == NULL!");

  //And we just replace it with a nop :)
  //mov x0, x0 = 0x52800001

  *opcode_stream = 0x52800001;

  return true;
}

int sock_puppet_all_callback() {
  pretty_log("Doing sock_puppet patches. Transferring output to match handlers...", INFO);

  xnu_pf_patchset_t *patchset = xnu_pf_patchset_create(XNU_PF_ACCESS_32BIT);
  struct mach_header_64 *main_header = xnu_header();
  xnu_pf_range_t *TEXTEXEC = xnu_pf_segment(main_header, "__TEXT_EXEC");

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
  uint32_t in6_pcbdetach_opcodes[] = {
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

  uint32_t in6_pcbdetach_opcodes_masks[] = {
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
