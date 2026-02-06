#ifndef STACK_H
#define STACK_H

#include <inttypes.h>

typedef struct {
    size_t size;
    uint32_t  count;
    uint32_t cap;
    void *data;
} Stack;

Stack *Stack_create (size_t size);
void   Stack_destroy(Stack *s);
void  *Stack_pop    (Stack *s);
void   Stack_push   (Stack *s, void* data);

#endif