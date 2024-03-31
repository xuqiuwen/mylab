#include "lf-stack.h"
#include <stdio.h>
#include <stdlib.h>

void lf_stack_init(lf_stack_t *stack) {
    atomic_store(&stack->top, 0);
    return;
}

void lf_stack_push(lf_stack_t *stack, int value) {
    // Exercise 1: lock-free stack.
    // Add your code here:
    Node_t *p = (Node_t *)malloc(sizeof(Node_t));
    p->value = value;
    // p->next = stack->top;
    // stack->top = p;
    while (1) {
        p->next = atomic_load(&stack->top);                           // 预加载旧的堆栈顶部，并保证是从内存中加载
        if (atomic_compare_exchange_strong(&stack->top, &p->next, p)) // 尝试更新堆栈顶部
            break;
    }
}

int lf_stack_pop(lf_stack_t *stack) {
    // Exercise 1: lock-free stack
    // Add your code here:

    // Node_t *p = stack->top;
    //  stack->top = p->next;
    Node_t *p = atomic_load(&stack->top);
    if (p == NULL)
        return -1;
    int value;
    while (1) {
        p = atomic_load(&stack->top);
        if (atomic_compare_exchange_strong(&stack->top, &p, p->next))
            break;
    }
    value = p->value;
    free(p);
    return value;
}

int lf_stack_size(lf_stack_t *stack) {
    // Exercise 1: lock-free stack
    // Add your code here:
    int size = 0;
    Node_t *p = stack->top;
    while (p != NULL) {
        size++;
        p = p->next;
    }
    return size;
}
