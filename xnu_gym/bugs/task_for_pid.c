#include <pongo.h>
#include <defs.h>

extern void pretty_log(char *m, uint32_t err);

static bool handle_task_for_pid_posix_check(struct xnu_pf_patch* patch, void* cacheable_stream) {
  printf("\t%s: Started\n", __func__);
  xnu_pf_disable_patch(patch);


  return true;
}

static bool handle_task_conversion_eval(struct xnu_pf_patch* patch, void* cacheable_stream) {
  printf("\t%s: Started\n", __func__);
  xnu_pf_disable_patch(patch);


  return true;
}

static bool handle_inline_task_for_pid_check(struct xnu_pf_patch* patch, void* cacheable_stream) {
  printf("\t%s: Started\n", __func__);
  xnu_pf_disable_patch(patch);


  return true;
}

void tfp0_all_callback(int *err) {
  pretty_log("Doing tfp0 patches. Transferring output to match handlers...", INFO);

  xnu_pf_patchset_t *patchset = xnu_pf_patchset_create(XNU_PF_ACCESS_32BIT);
  struct mach_header_64 *main_header = xnu_header();
  xnu_pf_range_t *TEXTEXEC = xnu_pf_segment(main_header, "__TEXT_EXEC");

  uint64_t task_for_pid_posix_check_opcodes[] = {
    0x130dee87,     /* bl sub_xxx */
    0x88d038d5,     /* mrs x_, tpidr_el1 */
    0x088d41f9      /* ldr x8, [xn, #0xn]*/
  };

  const size_t task_for_pid_posix_check_opcodes_c =
      sizeof(task_for_pid_posix_check_opcodes) /
      sizeof(*task_for_pid_posix_check_opcodes);

  uint64_t task_for_pid_posix_check_masks[] = {
    0xfc000000,     /* ignore immediate */
    0xffffffe0,     /* ignore x_ in mrs */
    0xffc0001f      /*  ignore [] */
  };

  xnu_pf_maskmatch(
            patchset,
            task_for_pid_posix_check_opcodes,
            task_for_pid_posix_check_masks,
            task_for_pid_posix_check_opcodes_c,
            false,
            handle_task_for_pid_posix_check
  );

  xnu_pf_emit(patchset);
  xnu_pf_apply(TEXTEXEC, patchset);
  xnu_pf_patchset_destroy(patchset);


  return;
}
