#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "hash_table.h"
#include <stdlib.h>

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
  entry_t *buckets[17];
};




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
  /// Allocate space for a ioopm_hash_table_t = 17 pointers to
  /// entry_t's, which will be set to NULL
  ioopm_hash_table_t *result = calloc(1, sizeof(ioopm_hash_table_t));
  for(int i=0; i<17; i++)
  {
    result->buckets[i]= entry_create(0,NULL,NULL);
  }
  return result;
}

void entry_destroy(entry_t *dummy_entry){
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
  for(int i=0; i<17;i++){
    entry_destroy(ht->buckets[i]);
    free(ht->buckets[i]);
  }
  free(ht);
}

static entry_t *find_previous_entry_for_key(entry_t *entry, int key){
  int k1 = entry->key;
  entry_t *current= entry;

  if (k1>key){
    return entry->next;
  }
  else{
    while (current->next !=NULL){
      if (current->next->key < key) {
        current = current->next;
      }
      else {
        return current;
      }
    }
 return current;
  }
}

void ioopm_hash_table_insert(ioopm_hash_table_t *ht, int key, char *value)
{
  /// Calculate the bucket for this entry
  int bucket = key % 17;
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
  if (key<0){
    return Failure();
  }
  else{
    /// Find the previous entry for key
    entry_t *tmp = find_previous_entry_for_key(ht->buckets[key % 17], key);
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
}

option_t ioopm_hash_table_remove(ioopm_hash_table_t *ht, int key){
  if (key<0){
    return Failure();
  }
  else{
  option_t entry_exists = ioopm_hash_table_lookup(ht, key); //Check if the key exists
  entry_t *previous = find_previous_entry_for_key(ht->buckets[key % 17], key);
  entry_t *remove = previous->next;

  if (entry_exists.success) //If the entry exists then:
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
}

/*
int main () {
    return 0;
}
*/
