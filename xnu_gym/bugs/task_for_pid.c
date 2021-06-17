#include <pongo.h>
#include <defs.h>

extern void pretty_log(char *m, int err);
//
// static bool handle_task_for_pid_posix_check(struct xnu_pf_patch* patch, void* cacheable_stream) {
//   printf("%s: Entered matchhandler\n", __func__);
//   xnu_pf_disable_patch(patch);
//
//
//   return true;
// }
//
// static bool handle_task_conversion_eval(struct xnu_pf_patch* patch, void* cacheable_stream) {
//   printf("%s: Entered matchhandler\n", __func__);
//   xnu_pf_disable_patch(patch);
//
//
//   return true;
// }
//
// static bool handle_inline_task_for_pid_check(struct xnu_pf_patch* patch, void* cacheable_stream) {
//   printf("%s: Entered matchhandler\n", __func__);
//   xnu_pf_disable_patch(patch);
//
//
//   return true;
// }
//
 int tfp0_all_callback() {
   return 0;
 }
//   pretty_log("Doing tfp0 patches. Transferring output to match handlers...", INFO);
//
//   xnu_pf_patchset_t *patchset = xnu_pf_patchset_create(XNU_PF_ACCESS_32BIT);
//   struct mach_header_64 *main_header = xnu_header();
//   xnu_pf_range_t *TEXTEXEC = xnu_pf_segment(main_header, "__TEXT_EXEC");
//
//   if (!patchset || !main_header || !TEXTEXEC) {
//     pretty_log("Got a NULL value!", FAIL);
//     return 1;
//   }
//
//   /*
//   Not official patches; testing pf with JIT to ensure correct usage of
//   the API
//   */
//   uint64_t task_for_pid_posix_check_opcodes[] = {
//     0x94000000,     /* bl XXX */
//     0xd538d080,     /* mrs xX, tpidr_el1 */
//     0xf9418d08      /* ldr x8, [x8, 0x318]*/
//   };
//
//   const size_t task_for_pid_posix_check_opcodes_c =
//       sizeof(task_for_pid_posix_check_opcodes) /
//       sizeof(*task_for_pid_posix_check_opcodes);
//
//   uint64_t task_for_pid_posix_check_masks[] = {
//     0xfc000000,     /* mask immediate */
//     0xffffffe0,     /* mask register */
//     0xffffffff
//   };
//
//
//   xnu_pf_maskmatch(
//             patchset,
//             "task_for_pid_posix_check_matcher",
//             task_for_pid_posix_check_opcodes,
//             task_for_pid_posix_check_masks,
//             task_for_pid_posix_check_opcodes_c,
//             false,
//             handle_task_for_pid_posix_check
//   );
//
//   xnu_pf_emit(patchset);
//   xnu_pf_apply(TEXTEXEC, patchset);
//
//   xnu_pf_patchset_destroy(patchset);
//   return 0;
// }
