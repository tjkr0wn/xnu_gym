typedef struct patch_t {
  struct *patch_t next;
  void (*cb)(uint32_t *error_counter);
} patch_t;
