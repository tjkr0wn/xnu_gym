#include <pongo.h>
#include <xnu_gym.h>
#include <defs.h>
#include <bug_desc.h>

void (*existing_preboot_hook)();
struct bug_t *g_top_patch = NULL;
struct bug_t *null_patch = NULL;
uint32_t *g_failed_patches_counter = 0;
uint32_t g_queued_patches_counter = 0;

void pretty_log(char *m, uint32_t err) {
  if (err == SUCCESS)
    printf("xnu_gym: [+] %s\n", m);
  else if (err == FAIL)
    printf("xnu_gym: [!] %s\n", m);
  else if (err == INFO)
    printf("xnu_gym: [*] %s\n", m);
  else if (err == PATCH)
    printf("xnu_gym: [-] %s\n", m);
}

void print_help() {
  puts("\txnu_gym usage:");
  puts("\t\t-h | Prints this message");
  puts("\t\t-t | Patches task_for_pid(0) for ANY process (warning: unsafe).");
  puts("\t\t-r | Reintroduces the Trident bug(s).");
  puts("\t\t-m | Reintroduces the mach_portal bug(s).");
  return;
}

struct bug_t * init_new_patch(void (*cb)(uint32_t *err)) {
  struct bug_t *patch = malloc(sizeof(struct bug_t));
  if (patch == NULL) {
    return null_patch;
  }
  /*
  1. First added patch:
    [BOTTOM] < |Bug 1, next=NULL|> [TOP]
  2. Second added patch:
    [BOTTOM] < |Bug 1, next=NULL| >   < |Bug 2, next=Bug 1|> [TOP]
  */
  patch->cb = cb;
  patch->next = g_top_patch;
  g_top_patch = patch;
  g_queued_patches_counter += 1;
  return patch;
}

void arg_parse(const char* cmd, char* args) {
  char *c;
  uint32_t mode = 0;

  for (c = &args[0]; *c != '\0'; c++ ) {
    if (strcmp(c, "-") == 104) { //No clue why this works :)
      mode = 1;
      continue;
    }
    if (mode == 1) {
      if(strcmp(c, "h") == 0) {
        print_help();
        break;
      }

      if (strcmp(c, "t") == 0) {
        if (init_new_patch(&tfp0_all_callback) == null_patch) {
          pretty_log("malloc failed!", FAIL);
          return;
        }
      }

      if (strcmp(c, "r") == 0) {
        if (init_new_patch(&trident_bugs_callback) == null_patch) {
          pretty_log("malloc failed!", FAIL);
          return;
        }
      }
      mode = 0;
    }
  }
  return;
}

void do_all_patches() {
  /*
  if (existing_preboot_hook != NULL)
    existing_preboot_hook();
  */
  pretty_log("Entered pre-boot hook; doing patches...", INFO);

  int i;
  struct bug_t *current_looped_patch;
  for (i = 0; i < g_queued_patches_counter; i++) {
    current_looped_patch = g_top_patch;
    current_looped_patch->cb(g_failed_patches_counter);
    g_top_patch = current_looped_patch->next;
    free(current_looped_patch);
  }

  printf("xnu_gym: [*] Leaving with %d failed patches...", g_failed_patches_counter);

  /*Sleeping for enough time for the user to catch an error message before boot.*/
  
  sleep(5);

  return;
}

void module_entry() {
  pretty_log("Initializing xnu_gym...", INFO);
  existing_preboot_hook = preboot_hook;
  preboot_hook = do_all_patches;
  command_register("xnu_gym", "Send 'xnu_gym -h' for a list of commands", arg_parse);
}

char* module_name = "xnu_gym";

struct pongo_exports exported_symbols[] = {
    {.name = 0, .value = 0}
};
