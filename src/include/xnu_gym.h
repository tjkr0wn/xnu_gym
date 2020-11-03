#define TFP0 12
#define BUG_INSERT_TRIDENT 24
#define BUG_INSERT_MACH_PORTAL 36

typedef struct patch_t {
  struct *patch_t next;
  void (*cb)(uint32_t *error_counter);
} patch_t;
