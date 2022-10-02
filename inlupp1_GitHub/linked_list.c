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
  ioopm_list_t *list = calloc(1, sizeof(struct list));
  list->head = link_create(0,NULL);
  return list;
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
  free(list);
}

void ioopm_linked_list_prepend(ioopm_list_t *list, int value)
{
  list->head->next = link_create(value, list->head->next);  //list->head == dummy
  list->size++;
  /*
  if(list->head->next == NULL)  //om första linken är NULL
  {
  //  list->head = link_create(value, NULL);  //listans tail är nu den nya linken
  }
  */
}


void ioopm_linked_list_append(ioopm_list_t *list, int value)
{
  link_t *cursor = list->head->next;
  link_t *last = link_create(value, NULL);
  list->size++;
  if (list->head->next == NULL) //Om listan är tom
  {
    list->head->next = last;
    list->tail = last;
  }
  else{
  list->tail->next = last;
  list->tail = last;
 }
}

/*
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
*/

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
  link_t *previous = list_find_previous(list, index-1); //To find previous == index-1
  if((index < 0) || (index > list->size+1)){
    printf("Kan inte lägga till på (%d) plats i listan, listans storlek (%d)\n", index , list->size);
  }
  else if(index ==0){
    ioopm_linked_list_prepend(list,value);
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
  assert(list->head);
  link_t *current = list->head;
  for (int i = 0 ; i < index; i++)
  {
    current = current->next;
    assert(current);
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
      result = prop(current_key, current->value, extra);
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
    result = prop(current_key, current->value, extra);
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
