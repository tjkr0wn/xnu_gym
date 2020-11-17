#ifndef xnu_gym_h
#define xnu_gym_h

struct bug_t {
  struct *bug_t next;
  void (*cb)(uint32_t *error_counter);
};

#endif /* xnu_gym_h */
