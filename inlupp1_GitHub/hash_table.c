#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hash_table.h"

#define No_Buckets 17
#define Success(v)      (option_t) { .success = true, .value = v };
#define Failure()       (option_t) { .success = false };
#define Successful(o)   (o.success == true)
#define Unsuccessful(o) (o.success == false)

typedef struct entry entry_t;



struct entry
{
  int key;       // holds the key
  char *value;   // holds the value
  entry_t *next; // points to the next entry (possibly NULL)
};

struct hash_table
{
  entry_t *buckets[No_Buckets];
};

static int mod(int a) {
  while(a<0)
  {
    a = a + 17;
  }
  return a;
}

static entry_t *entry_create(int key, char *value, entry_t *next)
{
  entry_t *created_entry = calloc(1, sizeof(entry_t));
  created_entry->key = key;
  created_entry->value = value;
  created_entry->next = next;
  return created_entry;
}

ioopm_hash_table_t *ioopm_hash_table_create(void)
{
  /// Allocate space for a ioopm_hash_table_t = No_Buckets pointers to
  /// entry_t's, which will be set to NULL
  ioopm_hash_table_t *result = calloc(1, sizeof(ioopm_hash_table_t));
  for(int i=0; i<No_Buckets; i++)
  {
    result->buckets[i]= entry_create(0,NULL,NULL);
  }
  return result;
}

static void entry_destroy(entry_t *dummy_entry){
  entry_t *first_entry = dummy_entry->next;

  if(first_entry != NULL){
    entry_t *current = first_entry;
    entry_t *next_entry;

    while(current != NULL){
      next_entry = current->next;
      free(current);
      current = next_entry;
    }
  }
  dummy_entry->next=NULL;
}

void ioopm_hash_table_destroy(ioopm_hash_table_t *ht) {
  for(int i=0; i<No_Buckets;i++){
    entry_destroy(ht->buckets[i]);
    free(ht->buckets[i]);
  }
  free(ht);
}

static entry_t *find_previous_entry_for_key(entry_t *entry, int key){
  int k1 = entry->key;
  entry_t *current= entry;

  if (k1>key && key>0) //För att kunna hitta negativa värden i buckets.
  {
    return entry->next;
  }
  else
  {
    while (current->next != NULL)
    {
      if (current->next->key < key)
      {
        current = current->next;
      }
      else
      {
        return current;
      }
    }
    return current;
  }
}

static int negative_key(int key)
{
  int bucket = 0;
  if(key<0)
  {
    bucket = mod(key) % No_Buckets;
  }
  else
  {
    bucket = key % No_Buckets;
  }
  return bucket;
}

void ioopm_hash_table_insert(ioopm_hash_table_t *ht, int key, char *value)
{
  int bucket = negative_key(key);
  /// Search for an existing entry for a key
  entry_t *entry = find_previous_entry_for_key(ht->buckets[bucket], key);
  entry_t *next = entry->next;

  /// Check if the next entry should be updated or not
  if (next != NULL && next->key == key)
    {
      next->value = value;
    }
  else
    {
      entry->next = entry_create(key, value, next);
    }
}

option_t ioopm_hash_table_lookup(ioopm_hash_table_t *ht, int key)
{
  int bucket = negative_key(key);
  /// Find the previous entry for key
  entry_t *tmp = find_previous_entry_for_key(ht->buckets[bucket], key);
  entry_t *next = tmp->next;

    if (next && (next->key == key))
    {
      return Success(next->value);
    }
    else
    {
      return Failure();
    }
  }
//}

option_t ioopm_hash_table_remove(ioopm_hash_table_t *ht, int key)
{
  int bucket = negative_key(key);
  option_t entry_exists = ioopm_hash_table_lookup(ht, key); //Check if the key exists
  entry_t *previous = find_previous_entry_for_key(ht->buckets[bucket], key);
  entry_t *remove = previous->next;

  if (entry_exists.success) //If the entry exists then:
  {
    entry_t *after_remove = remove->next;
    free(remove);
    previous->next = after_remove;
    return Success(entry_exists.value);
  }
  else
  {
    return Failure();
  }
}

int ioopm_hash_table_size(ioopm_hash_table_t *ht)
{
  int counter = 0;

  for (int k=0; k<No_Buckets; k++)
  {
    entry_t *dummy_entry = ht->buckets[k];
    entry_t *next = dummy_entry->next;
    while(next != NULL)
    {
        counter++;
        next = next->next;
    }
  }
  return counter;
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

int *ioopm_hash_table_keys(ioopm_hash_table_t *ht){
  int size = ioopm_hash_table_size(ht);
  int n = 0;
  int *array = calloc(size,sizeof(int));

  for (int k=0; k<No_Buckets; k++)
  {
    entry_t *dummy_entry = ht->buckets[k];
    entry_t *next = dummy_entry->next;
    while(next != NULL)
    {
      array[n] = next->key;
      n++;
      next = next->next;
    }
  }
  return array;
}

char **ioopm_hash_table_values(ioopm_hash_table_t *ht){
  int size = ioopm_hash_table_size(ht);
  int n = 0;
  char **array = calloc(size,sizeof(char *));

  for (int k=0; k<No_Buckets; k++)
  {
    entry_t *dummy_entry = ht->buckets[k];
    entry_t *next = dummy_entry->next;
    while(next != NULL)
    {
      array[n] = next->value;
      n++;
      next = next->next;
    }
  }
  return array;
}

static bool key_equiv(int key, char *value_ignored, void *arg)
{
  int *other_key_ptr = arg;
  int other_key = *other_key_ptr;
  return key == other_key;
}

static bool value_equiv(int key, char *value, void *arg)
{
  char **other_value_ptr = arg;
  char *other_value = *other_value_ptr;
  return (strcmp(other_value, value)==0);
}

bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, int key)
{
  bool result = ioopm_hash_table_any(ht, key_equiv, &key);
  return result;
}

bool ioopm_hash_table_has_value(ioopm_hash_table_t *ht, char *value)
{
  bool result = ioopm_hash_table_any(ht, value_equiv, &value);
  return result;
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


bool ioopm_hash_table_any(ioopm_hash_table_t *ht, ioopm_predicate pred, void *arg){
  bool result = false;

  for(int i = 0; i<No_Buckets && !result; i++) //Så länge i <bucket och result != false
  {
    entry_t *dummy_entry = ht->buckets[i];
    entry_t *current = dummy_entry->next;
    while(current!= NULL && !result) //Medans current inte pekar på NULL och False
    {
      result = pred(current->key, current->value, arg); //Updaterar result beroende på vad pred returnar
      current = current->next;
    }
  }
 return result;
}

void ioopm_hash_table_apply_to_all(ioopm_hash_table_t *ht, ioopm_apply_function apply_fun, void *arg){
  for(int i =0; i<No_Buckets; i++)
  {
    entry_t *dummy_entry = ht->buckets[i];
    entry_t *current = dummy_entry->next;
    while(current!= NULL)
    {
      apply_fun(current->key, &current->value, arg);
      current = current->next;
    }
  }
}

/*
int main(void) {
/*  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  ioopm_hash_table_insert(ht,1,"bucket 1"); //Lägg till value "bucket 1" i key 1
  ioopm_hash_table_insert(ht,2,"bucket 2");
  ioopm_hash_table_insert(ht,3,"bucket 3");
  char *value = ioopm_hash_table_lookup(ht, -1).value;
  printf("Value: %s\n", value);
    return 0;
}
*/
