#include <stddef.h>
#include <stdlib.H>
#include "stack.h"

Stack *Stack_Create(size_t size)
{
    Stack *s = malloc(sizeof(Stack));
    s->size = size;
    s->data = malloc(size);
    s->cap = 1;
    s->count = 0;
    return s;
}

void Stack_Destroy(Stack *s)
{
    free(s->data);
    free(s);
}

void *Stack_pop(Stack *s)
{
    uint8_t *ret = (uint8_t*)s->data;
    ret += (--s->count) * s->size;
    return ret;
}

void Stack_push(Stack *s, void *data)
{
    uint8_t *ptr;

    if (s->count == s->cap) {
        void *data = realloc(s->data, s->cap * 2 * s->size);
        if (data == NULL) return;
        s->data = data;
        s->cap *= 2;
    }

    ptr = (uint8_t*)s->data;
    ptr += (s->count++) * s->size;
    memcpy(ptr, data, s->size);
}