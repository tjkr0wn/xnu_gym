#ifndef xnu_gym_h
#define xnu_gym_h

#define ARG_EXISTS(a, s) (memmem(a, strlen(a), s, strlen(s))
#define DEBUG(m) (pretty_log(m, INFO))

struct bug_t {
  void (*cb)(uint32_t *error_counter);
  struct bug_t *next;
};


#endif /* xnu_gym_h */
