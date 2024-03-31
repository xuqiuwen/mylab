#define _GNU_SOURCE
#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 10
#define NUM_ITERATIONS 1000000

struct counter_t {
    pthread_spinlock_t spinlock;
    int counter;
};

struct counter_t shared_data;

void initialize_counter(struct counter_t *counter) {
    // Exercise 3: Use the SpinLocks to solve this problem
    // Add your code here:
    pthread_spin_init(&shared_data.spinlock, 0);
}

void *increment_count(void *arg) {
    // Exercise 3: Use the SpinLocks to solve this problem
    // Add your code here:
    for (long i = 0; i < NUM_ITERATIONS; i++) {
        pthread_spin_lock(&shared_data.spinlock);
        shared_data.counter++;
        pthread_spin_unlock(&shared_data.spinlock);
    }
    pthread_exit(0);
}

void *decrement_count(void *arg) {
    // Exercise 3: Use the SpinLocks to solve this problem
    // Add your code here:
    for (long i = 0; i < NUM_ITERATIONS; i++) {
        pthread_spin_lock(&shared_data.spinlock);
        shared_data.counter--;
        pthread_spin_unlock(&shared_data.spinlock);
    }
    pthread_exit(0);
}

int main() {
    pthread_t threads[NUM_THREADS];
    initialize_counter(&shared_data);

    for (int i = 0; i < NUM_THREADS / 2; i++) {
        pthread_create(&threads[i], NULL, increment_count, NULL);
    }
    for (int i = NUM_THREADS / 2; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, decrement_count, NULL);
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Expected count: 0 (because half increment and half decrement)\n");
    assert(shared_data.counter == 0);
    printf("Actual count: %d\n", shared_data.counter);
    pthread_spin_destroy(&shared_data.spinlock);
    pthread_exit(0);
}
