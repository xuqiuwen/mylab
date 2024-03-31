#ifndef LF_STACK_H
#define LF_STACK_H
#include <stdatomic.h>

typedef struct Node_t {
    int value;
    struct Node_t *next;
} Node_t;

typedef _Atomic(Node_t *) atomic_node; // 这个指针是原子的

typedef struct Lf_stack_t {
    atomic_node top;
} lf_stack_t;

void lf_stack_init(lf_stack_t *stack);
void lf_stack_push(lf_stack_t *stack, int value);
int lf_stack_pop(lf_stack_t *stack);
int lf_stack_size(lf_stack_t *stack);

#endif