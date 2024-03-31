#include "concur-stack.h"
#include <stdio.h>
#include <stdlib.h>

void concur_stack_init(concur_stack_t *stack) {
    // stack->size = 0; // 初始大小为0
    pthread_mutex_init(&stack->mu, NULL);
    return;
}

void concur_stack_push(concur_stack_t *stack, int value) {
    // Exercise 1: complete other operations of the concurrent stack.
    // Add your code here:
    pthread_mutex_lock(&stack->mu);
    node_t *p = (node_t *)malloc(sizeof(node_t));
    p->value = value;
    p->next = stack->top;
    stack->top = p;
    // stack->size++;
    pthread_mutex_unlock(&stack->mu);
}

int concur_stack_pop(concur_stack_t *stack) {
    int value = -1;
    // Exercise 1: complete other operations of the concurrent stack.
    // Add your code here:
    pthread_mutex_lock(&stack->mu);
    node_t *p = stack->top;
    if (p == NULL)
        return -1;
    stack->top = p->next;
    value = p->value;
    free(p);
    // stack->size--;
    pthread_mutex_unlock(&stack->mu);

    return value;
}

int concur_stack_size(concur_stack_t *stack) {
    int size = 0;
    // Exercise 1: complete other operations of the concurrent stack.
    // Add your code here:
    pthread_mutex_lock(&stack->mu);
    // size = stack->size;
    node_t *p = stack->top;
    while (p != NULL) {
        size++;
        p = p->next;
    }
    pthread_mutex_unlock(&stack->mu);
    return size;
}
