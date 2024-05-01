#include "hashmap.h"
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct HashMap HashMap;
int enlarge_called = 0;

struct HashMap {
  Pair **buckets;
  long size;     // cantidad de datos/pairs en la tabla
  long capacity; // capacidad de la tabla
  long current;  // indice del ultimo dato accedido
};

Pair *createPair(char *key, void *value) {
  Pair *new = (Pair *)malloc(sizeof(Pair));
  new->key = key;
  new->value = value;
  return new;
}

long hash(char *key, long capacity) {
  unsigned long hash = 0;
  char *ptr;
  for (ptr = key; *ptr != '\0'; ptr++) {
    hash += hash * 32 + tolower(*ptr);
  }
  return hash % capacity;
}

int is_equal(void *key1, void *key2) {
  if (key1 == NULL || key2 == NULL)
    return 0;
  if (strcmp((char *)key1, (char *)key2) == 0)
    return 1;
  return 0;
}

void insertMap(HashMap *map, char *key, void *value) {
  if (map == NULL || key == NULL){
    return;
  }
  long charmander = hash(key, map->capacity);
  while(map->buckets[charmander] != NULL && map->buckets[charmander]->key != NULL){
    if (is_equal(map->buckets[charmander], key)){
      map->buckets[charmander]->value = value;
    }
    charmander = (charmander + 1) % map->capacity;
  }
  if (map->size == map->capacity){
    enlarge(map);
    charmander = hash(key, map->capacity);
  }
  Pair *charizar = createPair(key, value);
  map->buckets[charmander] = charizar;
  map->size++;
  map->current = charmander;
}

void enlarge(HashMap *map) {
    if (map == NULL) return;

    int oldCapacity = map->capacity;
    Pair **oldBuckets = map->buckets;

    map->capacity *= 2;
    map->buckets = (Pair **) calloc(map->capacity, sizeof(Pair *));
    if (map->buckets == NULL) {
        map->buckets = oldBuckets;
        map->capacity = oldCapacity;
        return;
    }

    map->size = 0;

    for (int i = 0; i < oldCapacity; i++) {
        if (oldBuckets[i] != NULL && oldBuckets[i]->key != NULL) {
            insertMap(map, oldBuckets[i]->key, oldBuckets[i]->value);
        }
    }

    free(oldBuckets);
}


HashMap *createMap(long capacity) {
  HashMap *map = (HashMap *)malloc(sizeof(HashMap));
  map->buckets = (Pair **)malloc(capacity * sizeof(Pair *));
  map->size = 0;
  map->capacity = capacity;
  map->current = -1;
  for (int i = 0; i < map->capacity; i++) {
    map->buckets[i] = NULL;
  }
  return map;
}

void eraseMap(HashMap *map, char *key) {
    if (map == NULL || key == NULL) return;

    long pos = hash(key, map->capacity);
    long initialPos = pos;

    while (map->buckets[pos] != NULL) {
        if (map->buckets[pos]->key != NULL && strcmp(map->buckets[pos]->key, key) == 0) {
            map->buckets[pos]->key = NULL;
            map->size--; 
            return;
        }
        pos = (pos + 1) % map->capacity;
        if (pos == initialPos) break; 
    }
}



Pair *searchMap(HashMap *map, char *key) {
    if (map == NULL || key == NULL) return NULL;
    long pos = hash(key, map->capacity);
    long initialPos = pos;
    while (map->buckets[pos] != NULL) {
        if (map->buckets[pos]->key != NULL && strcmp(map->buckets[pos]->key, key) == 0) {
            map->current = pos;
            return map->buckets[pos];
        }
        pos = (pos + 1) % map->capacity;
        if (pos == initialPos) break;
    }
    return NULL;
}

Pair *firstMap(HashMap *map) {
  long i;
  for (i = 0; i < map->capacity; i++) {
    if (map->buckets[i] != NULL && map->buckets[i]->key != NULL) {
      map->current = i;
      return map->buckets[i];
    }

  }
  map->current = -1;
  return NULL;
}

Pair *nextMap(HashMap *map) { return NULL; }
