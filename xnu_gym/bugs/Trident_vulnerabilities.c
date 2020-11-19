#include <pongo.h>
#include <defs.h>

extern void pretty_log(char *m, int err);

void trident_bugs_callback(int *err) {
  pretty_log("Doing Trident bugs patches. Transferring output to match handlers...", INFO);
  //*err += SUCCESS;
  return;
}
