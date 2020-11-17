#include <pongo.h>
#include <defs.h>

extern void pretty_log(char *m, uint32_t err);

//TODO
void trident_bugs_callback(uint32_t *err) {
  *err += SUCCESS;
  return;
}
