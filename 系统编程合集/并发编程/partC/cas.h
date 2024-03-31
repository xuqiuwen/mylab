#ifndef CAS_H
#define CAS_H
// static __inline__
unsigned long atomic_cmp_set(volatile long *ptr, unsigned long old, unsigned long set);
unsigned long atomic_myadd(volatile long *ptr, unsigned long val);
unsigned long atomic_mysub(volatile long *ptr, unsigned long val);
void atomic_mystore(volatile long *ptr, unsigned long val);
unsigned long atomic_myload(volatile long *ptr);
#endif