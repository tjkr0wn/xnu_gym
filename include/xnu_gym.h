#ifndef xnu_gym_h
#define xnu_gym_h

#define ARG_EXISTS(a, s) (memmem(a, strlen(a), s, strlen(s))
#define DEBUG(m) (pretty_log(m, DG))

struct bug_t {
  void (*cb)(int *error_counter);
  struct bug_t *next;
};


#endif /* xnu_gym_h */
