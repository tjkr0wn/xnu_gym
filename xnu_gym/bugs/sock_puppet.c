#include <pongo.h>
#include <defs.h>

extern void pretty_log(char *m, int err);

int sock_puppet_all_callback() {
  pretty_log("Doing sock_puppet patches. Transferring output to match handlers...", INFO);

  xnu_pf_patchset_t *patchset = xnu_pf_patchset_create(XNU_PF_ACCESS_32BIT);
  struct mach_header_64 *main_header = xnu_header();
  xnu_pf_range_t *TEXTEXEC = xnu_pf_segment(main_header, "__TEXT_EXEC");

  if (!patchset || !main_header || !TEXTEXEC) {
    pretty_log("Got a NULL value!");
    return 1;
  }


  //This sequence may only appear on some devices, TODO: check more kernels
  uint64_t in6_pcbdetach_opcodes[] = {
    0x7CA00000,     //ldr       xX,[xX, #0xXXX]
    0xF900001F,     //str       xzr,[xX, #0xXXX]
    0x7CA00000,     //ldr       xX,[xX, #0xXXX]
    0xF900001F,     //str       xzr,[xX, #0xXXX]
    0xaa1303e0,     //mov       x0,x19
    0x52800001,     //mov       w1,#0x0
    0x94000000      //bl        xxx
  };

  const size_t in6_pcbdetach_opcodes_c =
      sizeof(in6_pcbdetach_opcodess) /
      sizeof(*in6_pcbdetach_opcodes;

  uint64_t in6_pcbdetach_opcodes_masks[] = {
    0x7FE00000, /*mask all*/
    0xFFC0001F, /*mask base register and offset*/
    0x7FE00000, /*mask all*/
    0xFFC0001F, /*mask base register and offset*/
    0xffffffff,
    0xffffffff,
    0xfc000000, /*mask immediate*/
  };

  xnu_pf_maskmatch(
            patchset,
            "in6_pcbdetach_matcher",
            in6_pcbdetach_opcodes,
            in6_pcbdetach_opcodes_masks,
            in6_pcbdetach,opcodes_c,
            false,
            handle_in6_pcbdetach
  );

  xnu_pf_emit(patchset);

  xnu_pf_apply(TEXTEXEC, patchset);

  xnu_pf_patchset_destroy(patchset);

  return 0;
}

static bool handle_in6_pcbdetach(struct xnu_pf_patch* patch, void* cacheable_stream) {
  printf("%s: Entered matchhandler\n", __func__);
  xnu_pf_disable_patch(patch);

  return true;
}
