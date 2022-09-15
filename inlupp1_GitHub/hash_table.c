#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "hash_table.h"
#include <stdlib.h>

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

ioopm_hash_table_t *ioopm_hash_table_create()
{
  /// Allocate space for a ioopm_hash_table_t = 17 pointers to
  /// entry_t's, which will be set to NULL
  ioopm_hash_table_t *result = calloc(1, sizeof(ioopm_hash_table_t));
  for(int i=0; i<17; i++){
    result->buckets[i]= entry_create(0,NULL,NULL);
  }
  return result;
}
void ioopm_hash_table_destroy(ioopm_hash_table_t *ht) {
    free(ht);
}

void ioopm_hash_table_insert(ioopm_hash_table_t *ht, int key, char *value)
{
  /// Calculate the bucket for this entry
  int bucket = key % 17;
  /// Search for an existing entry for a key
  entry_t *existing_entry = find_entry_for_key(ht->buckets[bucket], key);

  if (existing_entry != NULL) /// i.e., it exists
    {
      existing_entry->value = value;
    }
  else
    {
      /// Get a pointer to the first entry in the bucket
      entry_t *first_entry = ht->buckets[bucket];
      /// Create a new entry
      entry_t *new_entry = entry_create(key, value, first_entry);
      /// Make the new entry the first entry in the bucket
      ht->buckets[bucket] = new_entry;
    }
}
static entry_t *entry_create(int key, char *value, entry_t next)
{
  entry_t *created_entry = calloc(1, sizeof(entry_t));
  created_entry->key = key;
  created_entry->value = value;
  created_entry->next = next;
  return created_entry;
}


static entry_t find_previous_entry_for_key(entry_t *entry, int key){
  int k1 = entry->key;
  entry_t *next = entry->next;
  entry_t *current= entry;

if (k1==0){
  return entry->next;
}
else{
  while(true){
    if (current->next->key < key){
    current = current->next;
    }
    else{
      return current->next;
    }
  }
}
}

int main () {
    return 0;
}
