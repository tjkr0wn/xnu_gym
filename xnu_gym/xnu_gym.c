#include <pongo.h>
#include <xnu_gym.h>
#include <defs.h>
#include <bug_desc.h>
#include <stdarg.h>

void (*existing_preboot_hook)();
struct bug_t *g_top_patch = NULL;
struct bug_t *null_patch = NULL;
int g_queued_patches_counter = 0;
int g_failed_patches_counter = 0;

void debug(const char * s, int c, ...) {
  #ifdef DEBUGBUILD
  va_list args;
  va_start(args, c);
  printf("xnu_gym: [DEBUG] ");
  printf(s, args);
  va_end(args);
  #endif
  return;
}

void pretty_log(char *m, int err) {
  switch (err) {
    case SUCCESS:
      printf("xnu_gym: [+] %s\n", m);
      return;
    case FAIL:
      printf("xnu_gym: [!] %s\n", m);
      return;
    case INFO:
      printf("xnu_gym: [*] %s\n", m);
      return;
    case PATCH:
      printf("xnu_gym: [-] %s\n", m);
      return;
  }
}

static void print_help() {
  puts("\txnu_gym usage:");
  puts("\t\t-h | Prints this message");
  puts("\t\t-t | Patches task_for_pid(0) for ANY process (warning: unsafe).");
  puts("\t\t-s | Reintroduces the sock_puppet bug.");
  puts("\t\t-r | Reintroduces the Trident bug(s).");
  puts("\t\t-m | Reintroduces the mach_portal bug(s).");
  return;
}

void init_new_patch(int (*cb)()) {
  debug("Making a patch...\n", 0);

  struct bug_t *patch = malloc(sizeof(struct bug_t));
  if (patch == NULL) {
    pretty_log("malloc failed!", FAIL);
    return;
  }
  /*
  1. First added patch:
    [BOTTOM] <Bug 1, next=NULL> [TOP]
  2. Second added patch:
    [BOTTOM] <Bug 1, next=NULL> <Bug 2, next=Bug 1> [TOP]
  */
  patch->cb = cb;
  patch->next = g_top_patch;
  g_top_patch = patch;
  g_queued_patches_counter += 1;
  return;
}

void arg_parse(const char* cmd, char* args) {
  if (ARG_EXISTS(args, "-h"))
    print_help();

  else if (ARG_EXISTS(args, "-t"))
    init_new_patch(tfp0_all_callback);

  else if (ARG_EXISTS(args, "-r"))
    init_new_patch(trident_bugs_callback);

  else if (ARG_EXISTS(args, "-s"))
    init_new_patch(sock_puppet_all_callback);

  return;
}

static void do_all_patches() {
  pretty_log("Entered pre-boot hook; doing patches...", INFO);

  int i, r;
  struct bug_t *current_looped_patch;
  for (i = 0; i < g_queued_patches_counter; i++) {
    current_looped_patch = g_top_patch;
    r = current_looped_patch->cb();
    g_failed_patches_counter += r;
    g_top_patch = current_looped_patch->next;
    free(current_looped_patch);
  }

  printf("xnu_gym: [*] Leaving with %d failed patches...\n", g_failed_patches_counter);

  /*Spinning for enough time for the user to catch an error message before boot.*/
  /*Can exit pongo by just holding buttons*/

  if (g_failed_patches_counter > 0) {
    pretty_log("g_failed_patches_counter > 0; SPINNING\n", FAIL);
    SPIN();
  }

  return;
}

void module_entry() {
  pretty_log("Initializing xnu_gym...", INFO);
  debug("DEBUG enabled\n", 0);

  existing_preboot_hook = preboot_hook;
  preboot_hook = do_all_patches;
  command_register("xnu_gym", "Send 'xnu_gym -h' for a list of commands", arg_parse);
}

//https://gist.github.com/ccbrown/9722406
void DumpHex(const void* data, size_t size) {
	char ascii[17];
	size_t i, j;
	ascii[16] = '\0';
	for (i = 0; i < size; ++i) {
		printf("%02X ", ((unsigned char*)data)[i]);
		if (((unsigned char*)data)[i] >= ' ' && ((unsigned char*)data)[i] <= '~') {
			ascii[i % 16] = ((unsigned char*)data)[i];
		} else {
			ascii[i % 16] = '.';
		}
		if ((i+1) % 8 == 0 || i+1 == size) {
			printf(" ");
			if ((i+1) % 16 == 0) {
				printf("|  %s \n", ascii);
			} else if (i+1 == size) {
				ascii[(i+1) % 16] = '\0';
				if ((i+1) % 16 <= 8) {
					printf(" ");
				}
				for (j = (i+1) % 16; j < 16; ++j) {
					printf("   ");
				}
				printf("|  %s \n", ascii);
			}
		}
	}
}

char* module_name = "xnu_gym";

struct pongo_exports exported_symbols[] = {
    {.name = 0, .value = 0}
};
