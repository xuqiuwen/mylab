#ifndef LF_QUEUE_H
#define LF_QUEUE_H

#include <stdatomic.h>

typedef struct Node_t {
    int value;
    struct Node_t *next;
} node_t;

typedef _Atomic(node_t *) atomic_node;

typedef struct Lf_queue_t {
    atomic_node front;
    atomic_node rear;
} lf_queue_t;

void lf_queue_init(lf_queue_t *queue);
void lf_queue_enqueue(lf_queue_t *queue, int value);
int lf_queue_dequeue(lf_queue_t *queue);
int lf_queue_size(lf_queue_t *queue);

#endif