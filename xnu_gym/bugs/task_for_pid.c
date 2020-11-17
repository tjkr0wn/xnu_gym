#include <pongo.h>
#include <defs.h>

extern void pretty_log(char *m, uint32_t err);

//TODO
void tfp0_all_callback(uint32_t *err) {
  pretty_log("Doing tfp0 patches. Transferring output to match handlers...", INFO);

  xnu_pf_patchset_t *patchset = xnu_pf_patchset_create(XNU_PF_ACCESS_32BIT);

  uint64_t task_for_pid_posix_check_opcodes[] = {

  };

  const size_t task_for_pid_posix_check_opcodes_c =
      sizeof(task_for_pid_posix_check_opcodes) /
      sizeof(*task_for_pid_posix_check_opcodes);

  uint64_t task_for_pid_posix_check_masks[] = {
    0xffffffff,     /* match exactly */
    0xfc000000,     /* ignore immediate */
    0xffffffff,     /* match exactly */
    0xffc003ff,     /* ignore immediate */
    0xffc003ff,     /* ignore immediate */
  };

  xnu_pf_maskmatch(
            patchset,
            task_for_pid_posix_check_opcodes,
            task_for_pid_posix_check_masks,
            task_for_pid_posix_check_opcodes_c,
            false,
            handle_task_for_pid_posix_check
          );


  return;
}

static bool handle_task_for_pid_posix_check(struct xnu_pf_patch* patch, void* cacheable_stream) {
  printf("%s: Started\n", __func__);
  xnu_pf_disable_patch(patch);
  

  return;
}

static bool handle_task_conversion_eval(struct xnu_pf_patch* patch, void* cacheable_stream) {
  printf("%s: Started\n", __func__);
  xnu_pf_disable_patch(patch);


  return;
}

static bool handle_inline_task_for_pid_check(struct xnu_pf_patch* patch, void* cacheable_stream) {
  printf("%s: Started\n", __func__);
  xnu_pf_disable_patch(patch);


  return;
}
