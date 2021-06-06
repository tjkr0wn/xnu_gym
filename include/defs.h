#ifndef defs_h
#define defs_h

#define FAIL 1
#define SUCCESS 0
#define INFO 3
#define PATCH 4
#define DG 5

#define DEBUG(m) (pretty_log(m, DG))
//#define DEBUG(m) (asm volatile("mov x0, x0\n");)

#define SPIN() \
	while (true) {asm volatile("mov x0, x0\n");} \

extern void DumpHex(const void* data, size_t size);

#endif /* defs_h */
