#ifndef CONCUR_STACK_H
#define CONCUR_STACK_H

#include <pthread.h>

typedef struct Node_t {
    int value;
    struct Node_t *next;
} node_t;

typedef struct Concur_stack_t {
    pthread_mutex_t mu;
    node_t *top;
    // unsigned int size;//不应该使用size，在concur_stack_size遍历 栈 可以判断pop push的正确性
} concur_stack_t;

void concur_stack_init(concur_stack_t *stack);
void concur_stack_push(concur_stack_t *stack, int value);
int concur_stack_pop(concur_stack_t *stack);
int concur_stack_size(concur_stack_t *stack);

#endif
