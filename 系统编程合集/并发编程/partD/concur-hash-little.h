#ifndef CONCUR_HASH_LITTLE_H
#define CONCUR_HASH_LITTLE_H
// 重新设计数据结构
#include <pthread.h>

typedef struct Node_t {
    char *key;
    char *value;
    struct Node_t *next;
} node_t;

typedef struct Hash_t {
    pthread_mutex_t *mus; // 互斥量数组
    node_t **buckets;     // 桶数组
    int num_buckets;
    int size; // 不应该使用size，在hash_size遍历 哈希表 可以判断正确性
} hash_t;

void hash_init(hash_t *hash, int num_buckets);
void hash_insert(hash_t *ht, char *key, char *value);
char *hash_lookup(hash_t *ht, char *key);
int hash_delete(hash_t *ht, char *key);
int hash_size(hash_t *ht);

#endif
