#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "../include/hashmap.h"

void setNode(node_t* node, char* key, int value)
{
    node->key = malloc(strlen(key) + 1);
    strcpy(node->key, key);
    node->value = value;
    node->next = NULL;
}

void initializeHashMap(hashmap_t* map, int numNodes, int capacity)
{
    map->numNodes = numNodes;
    map->capacity = capacity;
    map->arr = calloc(capacity, sizeof(*map->arr));
}

int hashFunction(int capacity, char* key)
{
    unsigned long hash = 5381;
    int c;

    while (c = *key++)
    {
        // Uses n = 33 because for unknown reason works well with string hashes.
        hash = ((hash << 5) + hash) + c;
    } 
    return (int)(hash % capacity);
}

void insert(hashmap_t** mapPtr, char* key, int value)
{
    int hash = hashFunction((*mapPtr)->capacity, key);
    
    // Create a newNode to add to a list
    node_t* newNode = malloc(sizeof *newNode);
    setNode(newNode, key, value);

    // If first index is NULL set it to newNode
    if (!(*mapPtr)->arr[hash])
    {
        (*mapPtr)->arr[hash] = newNode;
    } 
    else
    {
        newNode->next = (*mapPtr)->arr[hash];
        (*mapPtr)->arr[hash] = newNode;
    }

    (*mapPtr)->numNodes++;
    if ((*mapPtr)->numNodes >= ((*mapPtr)->capacity / 2))
    {
        *mapPtr = rehash(*mapPtr);
    }   
}

void delete(hashmap_t* map, char* key)
{
    // Get the hash index
    int hash = hashFunction(map->capacity, key);

    // Previous node for deletion
    node_t* prevNode = NULL;
    node_t* currNode = map->arr[hash];

    while (currNode != NULL)
    {
        if (strcmp(currNode->key, key) == 0)
        {
            // If it is the first index of the list there is no previous
            if (currNode == map->arr[hash])
            {
                currNode = currNode->next;
            }
            else
            {
                prevNode->next = currNode->next;
            }
            free(currNode->key);
            free(currNode);
            map->numNodes--;
            return;
        }
        prevNode = currNode;
        currNode = currNode->next;
    }
}

int search(hashmap_t* map, char* key)
{
    // Get the hash value
    int hash = hashFunction(map->capacity, key);

    // The first node in the list the value is stored
    node_t* currNode = map->arr[hash];

    while (currNode != NULL)
    {    
        if (strcmp(currNode->key, key) == 0)
        {
            return currNode->value;
        }
        currNode = currNode->next;
    }

    // Return -1 means value cannot be found
    return -1;
}

void freeHashMap(hashmap_t* map)
{
    // If map is empty return
    if (!map) return;

    // Free every node of old map
    for (int i = 0; i < map->capacity; i++)
    {
        node_t* currNode = map->arr[i];
        while (currNode != NULL)
        {
            node_t* next = currNode->next;
            free(currNode->key);
            free(currNode);
            currNode = next;
        }
    }
    free(map->arr);
    free(map);
}

hashmap_t* rehash(hashmap_t* oldMap)
{
    // Create new hashmap with double the capacity
    hashmap_t* newMap = malloc(sizeof *newMap);
    initializeHashMap(newMap, 0, oldMap->capacity * 2);    

    // Insert all nodes into newMap
    for (int i = 0; i < oldMap->capacity; i++)
    {
        node_t* currNode = oldMap->arr[i];
        while (currNode != NULL)
        {
            insert(&newMap, currNode->key, currNode->value);
            currNode = currNode->next;
        }
    }

    freeHashMap(oldMap);
    return newMap;
}
