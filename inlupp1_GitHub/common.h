#pragma once
#include <stdlib.h>
#include <stdbool.h>


#define int_elem(x) (elem_t) { .i=(x) }
#define ptr_elem(x) (elem_t) { .p=(x) }
#define str_elem(x) (elem_t) { .str=(x) }

typedef struct option option_t;
typedef union elem elem_t;
typedef struct list ioopm_list_t; /// Meta: struct definition goes in C file

typedef unsigned long(*ioopm_hash_function)(elem_t elem);
typedef bool (*ioopm_predicate)(elem_t key, elem_t value, void *extra);
typedef void (*ioopm_apply_function)(elem_t key, elem_t *value, void *extra);

unsigned long string_knr_hash(const char *str);

bool cmp_str(elem_t str1, elem_t str2);

bool cmp_int(elem_t int1, elem_t int2);


union elem
{
  int i;
  unsigned int u;
  bool b;
  float f;
  void *p;
  char *str;
};
/// Compares two elements and returns true if they are equal
//typedef bool(*ioopm_eq_function)(elem_t a, elem_t b);

struct option
{
  bool success;
  elem_t value;
};
