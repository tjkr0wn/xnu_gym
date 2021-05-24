#include <pongo.h>
#include <defs.h>

extern void pretty_log(char *m, int err);

int trident_bugs_callback() {
  pretty_log("Doing Trident bugs patches. Transferring output to match handlers...", INFO);
  return 0;
}
