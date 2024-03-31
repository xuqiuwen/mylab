#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 10
#define NUM_ITERATIONS 1000000

struct counter_t {
    pthread_mutex_t counter_mutex;
    int counter;
};

struct counter_t shared_counter;

void initialize_counter(struct counter_t *counter) {
    // Exercise 2: Use the mutex to solve this problem
    // Add your code here:
    pthread_mutex_init(&shared_counter.counter_mutex, NULL);
}

void *increment_count(void *arg) {
    // Exercise 2: Use the mutex to solve this problem
    // Add your code here:
    for (long i = 0; i < NUM_ITERATIONS; i++) {
        pthread_mutex_lock(&shared_counter.counter_mutex);
        shared_counter.counter++;
        pthread_mutex_unlock(&shared_counter.counter_mutex);
    }
    pthread_exit(0);
}

void *decrement_count(void *arg) {
    // Exercise 2: Use the mutex to solve this problem
    // Add your code here:
    for (long i = 0; i < NUM_ITERATIONS; i++) {
        pthread_mutex_lock(&shared_counter.counter_mutex);
        shared_counter.counter--;
        pthread_mutex_unlock(&shared_counter.counter_mutex);
    }
    pthread_exit(0);
}

int main() {
    pthread_t threads[NUM_THREADS];

    initialize_counter(&shared_counter);

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
    assert(shared_counter.counter == 0);
    printf("Actual count: %d\n", shared_counter.counter);
    pthread_mutex_destroy(&shared_counter.counter_mutex);
    pthread_exit(0);
}
