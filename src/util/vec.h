#ifndef VEC_H__
#define VEC_H__

#include <stdlib.h>

#define VEC_INIT_CAPACITY 8

typedef struct vec {
    void** items;
    size_t count;
    size_t capacity;
} vec;

void vec_init(vec*);
int vec_count(vec*);
void vec_resize(vec*, size_t);
void vec_add(vec*, void*);
void* vec_get(vec*, size_t);
void vec_set(vec*, void*, size_t);
void vec_del(vec*, size_t);
void vec_free(vec*);
void vec_check_resize(vec*);
void vec_print(vec*);
void** vec_to_array(vec*);


#endif
