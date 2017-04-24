#include <stdio.h>
#include "vec.h"

void vec_init(vec* v) {
    v->capacity = VEC_INIT_CAPACITY;
    v->count = 0;
    v->items = malloc(sizeof(void*) * v->capacity);
}

int vec_count(vec* v) {
    return v->count;
}

void vec_resize(vec* v, size_t new_capacity) {
    void** items = realloc(v->items, new_capacity * sizeof(void*));
    // if we get new capacity 0, free() will be called, thus we have to check if items is still present
    if (items) {
        v->items = items;
        v->capacity = new_capacity;
    }
}

void vec_add(vec* v, void* item) {
    if (v->count >= v->capacity) {
        vec_resize(v, VEC_INIT_CAPACITY + v->capacity);
    }
    v->items[v->count] = item;
    v->count += 1;
}

void* vec_get(vec* v, size_t idx) {
    if (idx < v->count) {
        return v->items[idx];
    }
    return NULL;
}

void vec_set(vec* v, void* item, size_t idx) {
    if (idx < v->count) {
        v->items[idx] = item;
    }
}

void vec_del(vec* v, size_t idx) {
    if (idx >= v->count) {
        return;
    }

    for (size_t i = idx; i < v->count; i++) {
        v->items[i] = v->items[i+1];
    }
    v->items[v->count] = NULL;
    v->count--;

    vec_check_resize(v);
}

void vec_check_resize(vec* v) {
    if (v->capacity - v->count > VEC_INIT_CAPACITY) {
        vec_resize(v, v->capacity - VEC_INIT_CAPACITY);
    }
}

void vec_free(vec* v) {
    free(v->items);
}

void vec_print(vec* v) {
    puts("VECTOR STRUCT:");
    printf("Current item count: %d\n", v->count);
    printf("Current capacity: %d\n", v->capacity);
    printf("Current char interpreted items:\n");
    for (size_t i = 0; i < v->count; i++) {
        printf("Index: %d: \"%s\"\n", i, vec_get(v, i));
    }
    printf("\n");
}

void** vec_to_array(vec* v) {
    void** array = malloc((v->count+1) * sizeof(void*));
    for (size_t i = 0; i < v->count; i++) {
        array[i] = vec_get(v, i);
    }
    array[v->count] = NULL;
    return array;
}
