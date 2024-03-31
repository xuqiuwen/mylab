#include "sema.h"
#include <assert.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>

void sema_init(sema_t *sem, int v) {
    // Exercise 2:
    // Add your code here:
    atomic_init(&sem->value, v);
}

void sema_wait(sema_t *sem) {
    // Exercise 2:
    // Add your code here:

    // 便于理解的版本
    // while (1) {
    //     int old = atomic_load(&sem->value);
    //     if (old > 0) {
    //         // atomic_fetch_sub(&sem->value, 1);//这个不行，因为sem->value是共享变量，可能不等于old了
    //         if (atomic_compare_exchange_strong(&sem->value, &old, old - 1))
    //             break;
    //     }
    // }

    // 简化版本
    atomic_int old = atomic_load(&sem->value);
    while (old <= 0 || !atomic_compare_exchange_strong(&sem->value, &old, old - 1)) {
        old = atomic_load(&sem->value);
    }
}

void sema_post(sema_t *sem) {
    // Exercise 2:
    // Add your code here:
    atomic_fetch_add(&sem->value, 1);
}

typedef struct {
    sema_t sem;
    int num;
} counter_t;

counter_t counter;

void *start(void *arg) {
    for (int i = 0; i < 10000; i++) {
        sema_wait(&counter.sem);
        counter.num++;
        sema_post(&counter.sem);
    }
    pthread_exit(0);
}

int main() {
    pthread_t pids[10];
    sema_init(&counter.sem, 1);

    for (int i = 0; i < 10; i++) {
        pthread_create(&pids[i], NULL, start, NULL);
    }

    for (int i = 0; i < 10; i++) {
        pthread_join(pids[i], NULL);
    }

    printf("Counter: %d\n", counter.num);
    assert(counter.num == 100000);
    return 0;
}