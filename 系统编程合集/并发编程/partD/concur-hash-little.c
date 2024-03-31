#include "concur-hash-little.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// hash
/* this is the D. J. Bernstein hash function */
static unsigned int djb_hash(const char *cp) {
    unsigned hash = 5381;
    while (*cp)
        hash = 33 * hash ^ (unsigned char)*cp++;
    return hash;
}

void hash_init(hash_t *ht, int num_buckets) {
    ht->mus = calloc(num_buckets, sizeof(pthread_mutex_t));
    for (int i = 0; i < num_buckets; i++) { // 初始化桶互斥量
        pthread_mutex_init(&ht->mus[i], 0);
    }
    ht->buckets = calloc(num_buckets, sizeof(*ht->buckets)); // 分配桶指针数组
    // memset(ht->buckets, 0, num_buckets);//calloc自动初始化为0
    ht->num_buckets = num_buckets;
    // ht->size = 0;
}

void hash_insert(hash_t *ht, char *key, char *value) {
    // Exercise 2: complete the code below:
    // Add your code here:
    node_t *p = (node_t *)malloc(sizeof(node_t));
    unsigned int id = djb_hash(key) % ht->num_buckets;
    pthread_mutex_lock(&ht->mus[id]);
    p->key = malloc(strlen(key) + 1);
    strcpy(p->key, key);
    p->value = malloc(strlen(value) + 1);
    strcpy(p->value, value);
    p->next = ht->buckets[id]; // next要去掉吗，要去掉
    ht->buckets[id] = p;
    pthread_mutex_unlock(&ht->mus[id]);
    // ht->size++;
}

char *hash_lookup(hash_t *ht, char *key) { // 这个加不加锁呢，得加，因为有可能找到了，要求找到的不能被删除
    // Exercise 2: complete the code below:
    // Add your code here:
    unsigned int id = djb_hash(key) % ht->num_buckets;
    pthread_mutex_lock(&ht->mus[id]);
    node_t *p = ht->buckets[id];
    while (p != NULL && strcmp(p->key, key) != 0) {
        p = p->next;
    }
    pthread_mutex_unlock(&ht->mus[id]);
    if (p == NULL)
        return NULL;
    else
        return p->value;
}

// return 0 for success; 1 for non-exiting "key"
int hash_delete(hash_t *ht, char *key) {
    // Exercise 2: complete the code below:
    // Add your code here:
    unsigned int id = djb_hash(key) % ht->num_buckets;
    pthread_mutex_lock(&ht->mus[id]);
    if (ht->buckets[id] == NULL) {
        pthread_mutex_unlock(&ht->mus[id]);
        return 1;
    } else if (strcmp(ht->buckets[id]->key, key) == 0) {
        free(ht->buckets[id]->key);
        free(ht->buckets[id]->value);
        ht->buckets[id] = ht->buckets[id]->next;
        // ht->size--;
        pthread_mutex_unlock(&ht->mus[id]);
        return 0;
    }
    node_t *p = ht->buckets[id];
    while (p->next != NULL && strcmp(p->next->key, key) != 0) {
        p = p->next;
    }
    if (p->next == NULL) {
        pthread_mutex_unlock(&ht->mus[id]);
        return 1;
    } else {
        free(p->next->key);
        free(p->next->value);
        p->next = p->next->next;
        ht->size--;
        pthread_mutex_unlock(&ht->mus[id]);
        return 0;
    }
}

int hash_size(hash_t *ht) {
    int size = 0;
    // Exercise 2: complete the code below:
    // Add your code here:
    // size = ht->size;
    for (int i = 0; i < ht->num_buckets; i++) {
        node_t *p = ht->buckets[i];
        while (p != NULL) {
            size++;
            p = p->next;
        }
    }
    return size;
}