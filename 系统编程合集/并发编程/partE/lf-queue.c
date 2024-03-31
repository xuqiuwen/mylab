#include "lf-queue.h"
#include <stdio.h>
#include <stdlib.h>

void lf_queue_init(lf_queue_t *queue) {
    node_t *node = (node_t *)calloc(1, sizeof(*node)); // 有头节点
    atomic_store(&queue->front, node);
    atomic_store(&queue->rear, node);
    return;
}

void lf_queue_enqueue(lf_queue_t *queue, int value) {
    // Exercise 2: fill in your code:
    // Add your code here:
    node_t *node = (node_t *)calloc(1, sizeof(node_t));
    node->value = value;
    node->next = NULL;
    // queue->rear->next = node;
    // queue->rear = node;
    // while (1) {
    //     node_t *old = atomic_load(&queue->rear); // 保证是从内存中加载
    //     if (atomic_compare_exchange_strong(&queue->rear->next, &old->next, node)) {
    //         if (atomic_compare_exchange_strong(&queue->rear, &old, node)) { // 保证取的都是内存最新值，并禁止编译器优化
    //             break;
    //         }
    //     }
    // }
    node_t *old;
    while (1) { // 插入节点
        if (atomic_compare_exchange_strong(&queue->rear->next, &(node_t *){NULL}, node)) {
            old = atomic_load(&queue->rear);
            break;
        }
    }
    // 必然是连续的，别的线程是无法插入节点的，因为，rear不后移动，必然不满足&(node_t *){NULL}
    while (1) { // 更新rear
        if (atomic_compare_exchange_strong(&queue->rear, &old, node))
            break;
    }

    return;
}

int lf_queue_dequeue(lf_queue_t *queue) {
    // Exercise 2: fill in your code:
    // Add your code here:
    if (queue->front == queue->rear)
        return -1;
    // node_t *p = queue->front;
    // queue->front = p->next;
    node_t *p = queue->front;
    while (1) {
        p = atomic_load(&queue->front);
        if (atomic_compare_exchange_strong(&queue->front, &p, p->next)) {
            break;
        }
    }
    free(p);
}

int lf_queue_size(lf_queue_t *queue) {
    int size = 0;
    // Exercise 2: fill in your code:
    // Add your code here:
    node_t *p = queue->front;
    while (p != queue->rear) {
        size++;
        p = p->next;
    }

    return size;
}
