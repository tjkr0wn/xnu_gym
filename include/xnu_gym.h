#ifndef xnu_gym_h
#define xnu_gym_h

#define ARG_EXISTS(a, s) (memmem(a, strlen(a), s, strlen(s)))

struct bug_t {
  int (*cb)();
  struct bug_t *next;
};

#endif /* xnu_gym_h */
