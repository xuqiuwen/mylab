#include "cas.h"

unsigned long atomic_cmp_set(volatile long *ptr, unsigned long old, unsigned long set) {
    unsigned long prev;
    __asm__ volatile(
        "LOCK\n"
        "cmpxchgq %2,%1"
        : "=a"(prev), "+m"(*(volatile long *)(ptr))
        : "r"(set), "0"(old)
        : "memory");
    // printf("prev=%ld\n", prev);
    return prev;
}

unsigned long atomic_myadd(volatile long *ptr, unsigned long val) {
    unsigned long prev;
    __asm__ volatile(
        "LOCK\n"
        "xaddq %2, %1"
        : "=r"(prev)
        : "m"(*(volatile long *)(ptr)), "a"(val)
        : "cc", "memory");
    return prev;
}
unsigned long atomic_mysub(volatile long *ptr, unsigned long val) {
    unsigned long prev;
    __asm__ volatile(
        "LOCK\n"
        "subq %2, %1"
        : "=r"(prev)
        : "m"(*(volatile long *)(ptr)), "a"(val)
        : "cc", "memory");
    return prev;
}
void atomic_mystore(volatile long *ptr, unsigned long val) {
    __asm__ volatile(
        "LOCK\n"
        "movq %1, %0"
        :
        : "m"(*(volatile long *)(ptr)), "r"(val)
        : "memory");
}
unsigned long atomic_myload(volatile long *ptr) {
    unsigned long result;
    __asm__ volatile(
        "movq %1, %0"
        : "=r"(result)
        : "m"(*(volatile long *)(ptr))
        : "memory");
    return result;
}