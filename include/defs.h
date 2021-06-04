#ifndef defs_h
#define defs_h

#define FAIL 1
#define SUCCESS 0
#define INFO 3
#define PATCH 4
#define DG 5

#define D1

#define DEBUG(m) \
	#ifdef D1 \
		pretty_log(m, DG) \
	#else \
		asm volatile("mov x0, x0\n"); \
	#endif \

#define SPIN() \
	while (true) {asm volatile("mov x0, x0\n");} \

#endif /* defs_h */
