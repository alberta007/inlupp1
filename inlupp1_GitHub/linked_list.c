#include "linked_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
/**
* @file linked_list.c
* @author Elias Castegren and Oliver Hansson
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
  int value;
  struct link *next; //varför inte link_t???
};

// The list contains a pointer to its first link, and its size
struct list
{
  link_t *head;
  link_t *tail;
  int size;
};



static link_t *link_create(int value, link_t *next)
{
  link_t *link = calloc(1, sizeof(link_t));
  link->value = value;
  link->next = next;
  return link;
}


ioopm_list_t *ioopm_linked_list_create(void)
{
  ioopm_list_t *dummy_link = calloc(1, sizeof(struct list));
  dummy_link->head = link_create(0,NULL);
  return dummy_link;
}

static void links_destroy(link_t *dummy_link){
  link_t *first_link = dummy_link->next;
  if (first_link != NULL)
  {
    link_t *current = first_link;
    link_t *next_link;
    while(current != NULL)
    {
      next_link = current->next;
      free(current);
      current = next_link;
    }
  }
  dummy_link->next = NULL; //Sätter dummyns next pekare till NULL (tom lista)
}

void ioopm_linked_list_destroy(ioopm_list_t *list)
{
  list->size = 0;
  links_destroy(list->head);
  free(list->head);
  free(list->tail);
  free(list);
}

void ioopm_linked_list_prepend(ioopm_list_t *list, int value)
{
  if(list->head->next == NULL)  //om första linken är NULL
  {
    list->tail = link_create(value, list->tail);  //listans tail är nu den nya linken
  }
  list->head->next = link_create(value, list->head->next);  //list->head == dummy
  list->size++;
}


void ioopm_linked_list_append(ioopm_list_t *list, int value)
{
  link_t *dummy = list->head;
  link_t *current = dummy->next;
  if (current == NULL) //Om listan är tom
  {
     ioopm_linked_list_prepend(list, value); //Använd prepend
  }
  else
  {
    link_t *previous = list->tail;
    link_t *last = link_create(value, previous->next); //Skapa en ny link med position current (alltså sist)
    previous->next = last;  //Kopplar på last till listans tidigare tails nextpointer
  //  list->tail = last;  //Sätter last till listans tail
    list->size++;  //Ökar sizen med 1
  }
}


static link_t *list_find_previous(ioopm_list_t *list, int index)
{
  link_t *current = list->head;
//  if(index>0 || list->size>index){
    while(index>0)
    {
      current = current->next;
      index--;
    }
    return current;
}


void ioopm_linked_list_insert(ioopm_list_t *list, int index, int value)
{
  if(index == 0){
    ioopm_linked_list_prepend(list, value);
  }
  else if(list->size == 0){
    ioopm_linked_list_append(list, value);
  }
  else{
    link_t *previous = list_find_previous(list, index-1);
    previous->next = link_create(value, previous->next);
    list->size++;
  }
}

int ioopm_linked_list_remove(ioopm_list_t *list, int index)
{
  assert(list);
  assert(list->head);
  if (index == 0)
  {
    link_t *tmp = list->head;
    int value = tmp->value;
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
  int value = current->value;
  free(current);
  list->size--;
  return value;
}

int ioopm_linked_list_get(ioopm_list_t *list, int index)
{
  assert(list->head->next);
  link_t *current = list->head->next;
  for (int i = 1 ; i < index; i++)
  {
    current = current->next;
  //  assert(current);
  }
  return current->value;
}

bool ioopm_linked_list_contains(ioopm_list_t *list, int element)
{
  link_t *cursor = list->head->next;
  while (cursor != NULL)
  {
    if(cursor->value == element)
    {
      return true;
    }
    cursor = cursor->next;
  }
  return false;
}

int ioopm_linked_list_size(ioopm_list_t *list)
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
/*
bool ioopm_linked_list_all(ioopm_list_t *list, ioopm_char_predicate prop, void *extra)
{
  NOT_SUPPORTED();
}

bool ioopm_linked_list_any(ioopm_list_t *list, ioopm_char_predicate prop, void *extra)
{
  NOT_SUPPORTED();
}
void ioopm_linked_apply_to_all(ioopm_list_t *list, ioopm_apply_char_function fun, void *extra)
{
  NOT_SUPPORTED();
}
*/
