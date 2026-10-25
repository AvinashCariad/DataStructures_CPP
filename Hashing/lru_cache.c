#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 10000  // simple fixed-size hash table

typedef struct Node {
    int key;
    int value;
    struct Node* next;
    struct Node* prev;
} Node;

typedef struct HashEntry {
    int key;
    Node* node;
    struct HashEntry* next;
} HashEntry;

typedef struct {
    int cap;
    int n;
    Node* start;
    Node* end;
    HashEntry* table[HASH_SIZE];
} LRUCache;

// ------------------ Utility: Hash Map ------------------

unsigned int hashFunction(int key) {
    return (unsigned int) key % HASH_SIZE;
}

void hashInsert(LRUCache* cache, int key, Node* node) {
    unsigned int hashIndex = hashFunction(key);
    HashEntry* entry = (HashEntry*) malloc(sizeof(HashEntry));
    entry->key = key;
    entry->node = node;
    entry->next = cache->table[hashIndex];
    cache->table[hashIndex] = entry;
}



Node* hashGet(LRUCache* cache, int key) {
    unsigned int hashIndex = hashFunction(key);
    HashEntry* entry = cache->table[hashIndex];
    while (entry) {
        if (entry->key == key)
            return entry->node;
        entry = entry->next;
    }
    return NULL;
}



void hashDelete(LRUCache* cache, int key) {
    unsigned int hashIndex = hashFunction(key);
    HashEntry* entry = cache->table[hashIndex];
    HashEntry* prev = NULL;

    while (entry) {
        if (entry->key == key) {
            if (prev)
                prev->next = entry->next;
            else
                cache->table[hashIndex] = entry->next;
            free(entry);
            return;
        }
        prev = entry;
        entry = entry->next;
    }
}




// ------------------ Doubly Linked List ------------------

void insertNode(int key, int value, Node* start) {
    Node* p = start->next;
    Node* q = (Node*) malloc(sizeof(Node));
    q->key = key;
    q->value = value;
    q->next = p;
    q->prev = start;
    start->next = q;
    p->prev = q;
}

void deleteNode(Node* p) {
    Node* left = p->prev;
    Node* right = p->next;
    left->next = right;
    right->prev = left;
    free(p);
}

// ------------------ LRU Cache ------------------

LRUCache* lRUCacheCreate(int capacity) {
    LRUCache* cache = (LRUCache*) malloc(sizeof(LRUCache));
    cache->cap = capacity;
    cache->n = 0;

    cache->start = (Node*) malloc(sizeof(Node));
    cache->end = (Node*) malloc(sizeof(Node));

    cache->start->key = -1;
    cache->start->value = -1;
    cache->end->key = -1;
    cache->end->value = -1;

    cache->start->next = cache->end;
    cache->end->prev = cache->start;

    for (int i = 0; i < HASH_SIZE; i++)
        cache->table[i] = NULL;

    return cache;
}

int lRUCacheGet(LRUCache* obj, int key) {
    Node* node = hashGet(obj, key);
    if (!node)
        return -1;

    int value = node->value;

    // Move accessed node to front (most recently used)
    hashDelete(obj, key);
    deleteNode(node);
    insertNode(key, value, obj->start);
    hashInsert(obj, key, obj->start->next);

    return value;
}

void lRUCachePut(LRUCache* obj, int key, int value) {
    Node* node = hashGet(obj, key);
    if (node) {
        // Key exists: update
        hashDelete(obj, key);
        deleteNode(node);
        insertNode(key, value, obj->start);
        hashInsert(obj, key, obj->start->next);
        return;
    }

    if (obj->n < obj->cap) {
        insertNode(key, value, obj->start);
        hashInsert(obj, key, obj->start->next);
        obj->n++;
    } else {
        // Remove least recently used (end->prev)
        Node* lru = obj->end->prev;
        hashDelete(obj, lru->key);
        deleteNode(lru);
        insertNode(key, value, obj->start);
        hashInsert(obj, key, obj->start->next);
    }
}

void lRUCacheFree(LRUCache* obj) {
    Node* curr = obj->start;
    while (curr) {
        Node* temp = curr;
        curr = curr->next;
        free(temp);
    }

    for (int i = 0; i < HASH_SIZE; i++) {
        HashEntry* entry = obj->table[i];
        while (entry) {
            HashEntry* temp = entry;
            entry = entry->next;
            free(temp);
        }
    }
    free(obj);
}

// ------------------ Example Usage ------------------

// main code harness:
int main(int argc, char** argv) {
    LRUCache* cache = lRUCacheCreate(2);

    lRUCachePut(cache, 1, 1);
    lRUCachePut(cache, 2, 2);
    printf("%d\n", lRUCacheGet(cache, 1)); // returns 1

    lRUCachePut(cache, 3, 3); // evicts key 2
    printf("%d\n", lRUCacheGet(cache, 2)); // returns -1

    lRUCachePut(cache, 4, 4); // evicts key 1
    printf("%d\n", lRUCacheGet(cache, 1)); // returns -1
    printf("%d\n", lRUCacheGet(cache, 3)); // returns 3
    printf("%d\n", lRUCacheGet(cache, 4)); // returns 4

    lRUCacheFree(cache);
    return 0;
}
