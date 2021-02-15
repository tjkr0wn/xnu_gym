#ifndef xnu_gym_h
#define xnu_gym_h

#define ARG_EXISTS(a, s) (memmem(a, strlen(a), s, strlen(s))
#define DEBUG(m) (pretty_log(m, DG))
//#define DEBUG(m) (asm volatile("mov x0, x0\n"))

struct bug_t {
  int (*cb)();
  struct bug_t *next;
};

#endif /* xnu_gym_h */
