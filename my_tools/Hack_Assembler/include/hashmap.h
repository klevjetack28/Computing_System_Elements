#ifndef HASHMAP_H
#define HASHMAP_H

typedef struct node 
{
    char* key;
    int value;
    struct node* next;
} node_t;


typedef struct hashMap
{
    int numNodes, capacity;
    node_t** arr;  
} hashmap_t;

void setNode(node_t* node, char* key, int value);

void initializeHashMap(hashmap_t* map, int numNodes, int capacity);

int hashFunction(int capacity, unsigned char* key);

void insert(hashmap_t** map, char* key, int value);

void delete(hashmap_t* map, char* key);

int search(hashmap_t* map, char* key);

void freeHashMap(hashmap_t* map);

hashmap_t* rehash(hashmap_t* oldMap);

#endif // HASHMAP_H
