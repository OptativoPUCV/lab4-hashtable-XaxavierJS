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
  enlarge_called = 1; // no borrar (testing purposes)
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

void eraseMap(HashMap *map, char *key) {}



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
