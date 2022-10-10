#include "linked_list.h"
#include "common.h"
#include "iterator.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>

/**
* @file linked_list.c
* @author Elias Castegren and Oliver Hansson , Albert Alpsten
* @date 1 Sep 2021
* @brief A simple linked list that implements parts of the interface
* of `linked_list.h`. The functions that are not implemented will fail
* at runtime with an assertion error, printing a message to stderr.
*/
// This is a macro that is used in the functions that are not implemented,
// to have them fail at runtime. You do not have to understand it to use
// this file!
typedef struct link link_t;
// The links of the linked list
struct link
{
  elem_t value;
  struct link *next; // varför inte link_t???
};

// The list contains a pointer to its first link, and its size
struct list
{
  link_t *head;
  link_t *tail;
  size_t size;
  ioopm_eq_function func;
};

struct iter
{
  link_t *cursor;
  ioopm_list_t *list;
};

static link_t *link_create(elem_t value, link_t *next)
{
  link_t *link = calloc(1, sizeof(link_t));
  link->value = value;
  link->next = next;
  return link;
}

ioopm_list_t *ioopm_linked_list_create(ioopm_eq_function func)
{
  ioopm_list_t *list = calloc(1, sizeof(struct list));
  list->head = link_create(int_elem(0), NULL);
  return list;
}

static void links_destroy(link_t *dummy_link)
{
  link_t *first_link = dummy_link->next;
  if (first_link != NULL)
  {
    link_t *current = first_link;
    link_t *next_link;
    while (current != NULL)
    {
      next_link = current->next;
      free(current);
      current = next_link;
    }
  }
  dummy_link->next = NULL; // Sätter dummyns next pekare till NULL (tom lista)
}

void ioopm_linked_list_destroy(ioopm_list_t *list)
{
  list->size = 0;
  links_destroy(list->head);
  free(list->head);
  free(list);
}

void ioopm_linked_list_prepend(ioopm_list_t *list, elem_t value)
{
  if (list->head->next == NULL) // Om listan är tom
  {
    link_t *first = link_create((value), list->head->next);
    list->size++;
    list->head->next = first;
    list->tail = first;
  }
  else
  {
    link_t *cursor = list->head->next;
    link_t *first = link_create(value, cursor);
    list->size++;
    list->head->next = first;
  }
}

void ioopm_linked_list_append(ioopm_list_t *list, elem_t value)
{
  link_t *last = link_create(value, NULL);
  list->size++;
  if (list->head->next == NULL) // Om listan är tom
  {
    list->head->next = last;
    list->tail = last;
  }
  else
  {
    list->tail->next = last;
    list->tail = last;
  }
}


static link_t *list_find_previous(ioopm_list_t *list, int index)
{
  link_t *current = list->head;
  while(index>0)
  {
    current = current->next;
    index--;
  }
  return current;
}


void ioopm_linked_list_insert(ioopm_list_t *list, int index, elem_t value)
{
  link_t *previous = list_find_previous(list, index-1); //To find previous == index-1
  if(index == 1){
    ioopm_linked_list_prepend(list,value);
  }
  else if((index < 0) || (index > list->size+1)){
    printf("Kan inte lägga till på (%d) plats i listan, listans storlek (%zu)\n", index , list->size);
  }
  else if(previous == list->tail){
    ioopm_linked_list_append(list,value);
  }
  else{
    link_t *new_link = link_create(value, previous->next);
    previous->next = new_link;
    list->size++;
  }
}

elem_t ioopm_linked_list_remove(ioopm_list_t *list, int index)
{
  assert(list);
  assert(list->head);
  if (index == 0)
  {
    link_t *tmp = list->head;
    elem_t value = tmp->value;
    list->head = tmp->next;
    free(tmp);
    list->size--;
    return value;
  }
  // index > 0
  assert(list->head);
  link_t *prev = list->head;
  link_t *current = prev->next;
  for (int i = 1; i < index; i++)
  {
    assert(current);
    prev = current;
    current = current->next;
  }
  prev->next = current->next;
  elem_t value = current->value;
  free(current);
  list->size--;
  return value;
}

elem_t ioopm_linked_list_get(ioopm_list_t *list, int index)
{
  assert(list->head);
  link_t *current = list->head;
  for (int i = 0; i < index; i++)
  {
    current = current->next;
    assert(current);
  }
  return current->value;
}

bool ioopm_linked_list_contains(ioopm_list_t *list, elem_t element)
{
  link_t *cursor = list->head->next;
  while (cursor != NULL)
  {
    if ((cursor->value).i == element.i)
    {
      return true;
    }
    cursor = cursor->next;
  }
  return false;
}

size_t ioopm_linked_list_size(ioopm_list_t *list)
{
  return list->size;
}

bool ioopm_linked_list_is_empty(ioopm_list_t *list)
{
  if (list->size == 0){
    return true;
  }
  return false;
}

void ioopm_linked_list_clear(ioopm_list_t *list)
{
  links_destroy(list->head);
  list->size=0;
}

bool ioopm_linked_list_all(ioopm_list_t *list, ioopm_int_predicate prop, void *extra)
{
  bool result = true;
  int current_key = 0;
  if(ioopm_linked_list_size(list) == 0)
  {
    result = false;
    return result;
  }
  else
  {
    link_t *current = list->head->next; //first link in list
    while(current != NULL && result)
    {
      current_key++;
      result = prop(current_key, (current->value), extra);
      current = current->next;
    }
  }
  return result;
}

bool ioopm_linked_list_any(ioopm_list_t *list, ioopm_int_predicate prop, void *extra)
{
  bool result = false;
  int current_key = 0;

  if(ioopm_linked_list_size(list) == 0)
  {
    return false;
  }
  link_t *current = list->head->next;
  while(current != NULL && !result)
  {
    current_key++;
    result = prop(current_key, (current->value), extra);
    current = current->next;
  }
  return result;
}

void ioopm_linked_list_apply_to_all(ioopm_list_t *list, ioopm_apply_int_function fun, void *extra)
{
  int current_key = 0;
  link_t *current = list->head->next;
  while(current){
    current_key++;
    fun(current_key, &current->value, extra);
    current = current->next;
  }
}

////////////////Iterator functions////////////////////////////////////////////////////////////////////////////////

ioopm_list_iterator_t *ioopm_list_iterator(ioopm_list_t *list)
{
  ioopm_list_iterator_t *result = calloc(1, sizeof(ioopm_list_iterator_t));
  result->cursor = list->head;
  result->list = list;
  return result;
}


bool ioopm_iterator_has_next(ioopm_list_iterator_t *iter)
{
  if(iter->cursor != NULL)
  {
    if(iter->cursor->next != NULL) {
      return true;
    }
    else{
      return false;
    }
  }
  else{
    return false;
  }
}

elem_t ioopm_iterator_next(ioopm_list_iterator_t *iter)
{
  if(ioopm_iterator_has_next(iter) == true)
  {
    iter->cursor = iter->cursor->next;
    return (iter->cursor->value);
  }
  else
  {
    return (iter->cursor->value);
  }
}


void ioopm_iterator_reset(ioopm_list_iterator_t *iter)
{
  iter->cursor = (iter->list->head);
}


elem_t ioopm_iterator_current(ioopm_list_iterator_t *iter)
{
  if(ioopm_linked_list_is_empty(iter->list) == true) //Om listan ör tom returna 0.
  {
    return int_elem(0);
  }
  else{
    return (iter->cursor->value);
  }
}

void ioopm_iterator_destroy(ioopm_list_iterator_t *iter)
{
  free(iter);
}
