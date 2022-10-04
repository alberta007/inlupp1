#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hash_table.h"
#include "linked_list.h"


#define Success(v) (option_t){.success = true, .value = v};
#define Failure() (option_t){.success = false};
#define Successful(o) (o.success == true)
#define Unsuccessful(o) (o.success == false)
#define No_Buckets 17

typedef struct entry entry_t;

struct entry
{
  elem_t key;       // holds the key
  elem_t value;   // holds the value
  entry_t *next; // points to the next entry (possibly NULL)
};

struct hash_table
{
  ioopm_hash_function hash;
  ioopm_eq_function eq;
  entry_t *buckets[No_Buckets];
};


static int mod(int key)
{
  while (key < 0)
  {
    key = key + 17;
  }
  return key;
}


static entry_t *entry_create(elem_t key, elem_t value, entry_t *next)
{
  entry_t *created_entry = calloc(1, sizeof(entry_t));
  created_entry->key = key;
  created_entry->value = value;
  created_entry->next = next;
  return created_entry;
}

ioopm_hash_table_t *ioopm_hash_table_create(ioopm_hash_function hash, ioopm_eq_function eq)
{
  ioopm_hash_table_t *result = calloc(1, sizeof(ioopm_hash_table_t));
  elem_t dummy_key;
  elem_t dummy_value;
  //DUMMY Key and Value:
  dummy_key.i = 0;
  dummy_value.p = NULL;
  for(int i=0; i<No_Buckets; i++)
  {
    result->buckets[i] = entry_create(dummy_key, dummy_value, NULL);
  }
  result->hash = hash;
  result->eq = eq;
  return result;
}

static void entry_destroy(entry_t *dummy_entry)
{
  entry_t *current = dummy_entry->next;
  entry_t *tmp;

  while (current != NULL)
  {
    tmp = current;
    current = tmp->next;
    free(tmp);
  }
  dummy_entry->next = NULL;
}


void ioopm_hash_table_destroy(ioopm_hash_table_t *ht)
{
  for (int i = 0; i < No_Buckets; i++)
  {
    entry_destroy(ht->buckets[i]);
    free(ht->buckets[i]);
  }
  free(ht);
}

static entry_t *find_previous_entry_for_key(ioopm_hash_table_t *ht, entry_t *entry, elem_t key){

  entry_t *current = entry;
  entry_t *next = current->next;

  while (next != NULL){
    if (ht->hash(next->key) >= ht->hash(key)){
      return current;
    }
    current = next;
    next = next->next;
  }
  return current;
}


static int reform_key(ioopm_hash_table_t *ht, elem_t key)
{
  int bucket = 0;
  if(ht->hash(key) < 0)
  {
    bucket = mod(ht->hash(key)) % No_Buckets;
  }
  else
  {
    bucket = ht->hash(key) % No_Buckets;
  }
  return bucket;
}

void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value)
{
  int bucket = reform_key(ht, key);
  /// Search for an existing entry for a key
  entry_t *entry = find_previous_entry_for_key(ht, ht->buckets[bucket], key);
  entry_t *next = entry->next;

  /// Check if the next entry should be updated or not
  if (next != NULL && (ht->hash(next->key) == ht->hash(key)))
    {
      next->value = value;
    }
  else
  {
    entry->next = entry_create(key, value, next);
  }
}

option_t ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key)
{
  int bucket = reform_key(ht, key);
  /// Find the previous entry for key
  entry_t *tmp = find_previous_entry_for_key(ht, ht->buckets[bucket], key);
  entry_t *next = tmp->next;

    if (next && (ht->hash(next->key) == ht->hash(key)))
    {
      return Success(ht->hash(next->value));
    }
    else
    {
      return Failure();
    }
}

option_t ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key)
{
  int bucket = reform_key(ht, key);

  option_t entry_exists = ioopm_hash_table_lookup(ht, key); //Check if the key exists
  entry_t *previous = find_previous_entry_for_key(ht, ht->buckets[bucket], key);
  entry_t *remove = previous->next;

    if (entry_exists.success) // If the entry exists then:
    {
      entry_t *after_remove = remove->next;
      previous->next = after_remove;
      free(remove);
      return Success(entry_exists.value);
    }
    else
    {
      return Failure();
    }
  }


size_t ioopm_hash_table_size(ioopm_hash_table_t *ht)
{
  size_t entry_count = 0;

  for (int i = 0; i < No_Buckets; i++)
  {
    entry_t *dummy = ht->buckets[i];
    entry_t *next = dummy->next;
    while (next != NULL)
    {
      entry_count++;
      next = next->next;
    }
  }
  return entry_count;
}


bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht)
{
  return ioopm_hash_table_size(ht) == 0;
}


void ioopm_hash_table_clear(ioopm_hash_table_t *ht)
{
  for (int k=0; k<No_Buckets; k++)
  {
    entry_destroy(ht->buckets[k]);
  }
}

ioopm_list_t *ioopm_hash_table_keys(ioopm_hash_table_t *ht)
{
  ioopm_list_t *list = ioopm_linked_list_create(ht->eq);
  for (int i = 0; i < No_Buckets; i++)
  {
    entry_t *current = ht->buckets[i]->next;

    while (current != NULL)
    {
      ioopm_linked_list_append(list, current->key);
      current = current->next;
    }
  }
  return list;
}


ioopm_list_t *ioopm_hash_table_values(ioopm_hash_table_t *ht)
{
  ioopm_list_t *list = ioopm_linked_list_create(ht->eq);
  for (int i = 0; i < No_Buckets; i++)
  {

    entry_t *current = ht->buckets[i]->next;
    while (current != NULL)
    {
      ioopm_linked_list_append(list, current->value);
      current = current->next;
    }
  }
  return list;
}

bool ioopm_hash_table_all(ioopm_hash_table_t *ht, ioopm_predicate pred, void *arg){
  bool result = true;
  if (ioopm_hash_table_is_empty(ht)){
    return false;
  }
  for(int i =0; i<No_Buckets && result; i++) //Så länge i <bucket och result==true
  {
    entry_t *dummy_entry = ht->buckets[i];
    entry_t *current = dummy_entry->next;
    while(current!= NULL && result) //Medans current inte pekar på NULL och True
    {
      result = pred(current->key, current->value, arg); //Updaterar result beroende på vad pred returnar
      current = current->next;
    }
  }
  return result;
}

bool ioopm_hash_table_any(ioopm_hash_table_t *ht, ioopm_predicate pred, void *arg)
{
  bool result = false;

  for(int i = 0; i<No_Buckets && !result; i++) //Så länge i <bucket och result != false
  {
    entry_t *dummy_entry = ht->buckets[i];
    entry_t *current = dummy_entry->next;
    while (current != NULL && !result) // Medans current inte pekar på NULL och False
    {
      result = pred(current->key, current->value, arg); // Updaterar result beroende på vad pred returnar
      current = current->next;
    }
  }
  return result;
}

bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key)
{
  bool key_equiv(elem_t key, elem_t value_ignored, void *arg)
  {
    elem_t *other_key_ptr = arg;
    elem_t other_key = *other_key_ptr;
    return ht->hash(key) == ht->hash(other_key);
  }
  bool result = ioopm_hash_table_any(ht, key_equiv, &key);
  return result;
}

bool ioopm_hash_table_has_value(ioopm_hash_table_t *ht, elem_t value)
{
  bool value_equiv(elem_t key, elem_t value, void *arg)
 {
   elem_t *other_value = arg;
   return ht->eq(*other_value, value);
 }
  bool result = ioopm_hash_table_any(ht, value_equiv, &value);
  return result;
}

void ioopm_hash_table_apply_to_all(ioopm_hash_table_t *ht, ioopm_apply_function apply_fun, void *arg){
  for(int i =0; i<No_Buckets; i++)
  {
    entry_t *dummy_entry = ht->buckets[i];
    entry_t *current = dummy_entry->next;
    while (current != NULL)
    {
      apply_fun(current->key, &current->value, arg);
      current = current->next;
    }
  }
}
