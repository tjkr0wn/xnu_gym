#ifndef xnu_gym_h
#define xnu_gym_h

struct bug_t {
  void (*cb)(uint32_t *error_counter);
  struct bug_t *next;
};

#endif /* xnu_gym_h */
