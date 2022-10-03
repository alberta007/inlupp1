#pragma once

#include <stdbool.h>
#include <stdlib.h>

#define int_elem(x) \
    (elem_t) { .i = (x) }
#define ptr_elem(x) \
    (elem_t) { .p = (x) }
#define str_elem(x) \
    (elem_t) { .str = (x)}

typedef union elem elem_t;

union elem
{
    int i;
    char chr; 
    char *str;
    float f;
    void *p;
    /// other choices certainly possible
};


typedef unsigned long (*ioopm_hash_function)(elem_t elem);

typedef bool (*ioopm_eq_function)(elem_t a, elem_t b);
