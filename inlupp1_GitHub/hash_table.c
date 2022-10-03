#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "hash_table.h"
#include <stdlib.h>
#include "linked_list.h"
#include "linked_list.c"

#define Success(v) (option_t){.success = true, .value = v};
#define Failure() (option_t){.success = false};
#define Successful(o) (o.success == true)
#define Unsuccessful(o) (o.success == false)
#define No_Buckets 17

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

static int mod(int key)
{
  while (key < 0)
  {
    key = key + 17;
  }
  return key;
}

static entry_t *entry_create(int key, char *value, entry_t *next)
{
  entry_t *created_entry = calloc(1, sizeof(entry_t));
  created_entry->key = key;
  created_entry->value = value;
  created_entry->next = next;
  return created_entry;
}

ioopm_hash_table_t *ioopm_hash_table_create()
{
  /// Allocate space for a ioopm_hash_table_t = No_Buckets pointers to
  /// entry_t's, which will be set to NULL
  ioopm_hash_table_t *result = calloc(1, sizeof(ioopm_hash_table_t));
  for (int i = 0; i < No_Buckets; i++)
  {
    result->buckets[i] = entry_create(0, NULL, NULL);
  }
  return result;
}

static void entry_destroy(entry_t *entry)
{
  if (entry->next != NULL)
  {
    entry_t *temp = entry->next;
    entry_destroy(temp);
    free(entry);
  }
  else
  {
    free(entry);
  }
}

void ioopm_hash_table_destroy(ioopm_hash_table_t *ht)
{
  for (int i = 0; i < No_Buckets; i++)
  {
    entry_destroy(ht->buckets[i]);
  }
  free(ht);
}

static entry_t *find_previous_entry_for_key(entry_t *entry, int key)
{
  int k1 = entry->key;
  entry_t *current = entry;

  if (k1 > key)
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

void ioopm_hash_table_insert(ioopm_hash_table_t *ht, int key, char *value)
{
  /// Calculate the bucket for this entry
  int bucket = key % No_Buckets;
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

/////////////////////////////////////////////////////////////////

option_t ioopm_hash_table_lookup(ioopm_hash_table_t *ht, int key)
{
  /// Find the previous entry for key
  if (key < 0)
  {
    return Failure();
  }

  entry_t *tmp = find_previous_entry_for_key(ht->buckets[key % No_Buckets], key);
  entry_t *next = tmp->next;

  if (next && next->value)
  {
    return Success(next->value);
  }
  else
  {
    return Failure();
  }
}

option_t ioopm_hash_table_remove(ioopm_hash_table_t *ht, int key)
{
  if (key < 0)
  {
    return Failure();
  }
  else
  {
    option_t entry_exists = ioopm_hash_table_lookup(ht, key); // Check if the key exists
    entry_t *previous = find_previous_entry_for_key(ht->buckets[key % No_Buckets], key);
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
}

size_t ioopm_hash_table_size(ioopm_hash_table_t *ht)
{
  size_t entry_count = 0;

  for (int i = 0; i < No_Buckets; i++)
  {
    entry_t *next = ht->buckets[i]->next;

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
  for (int i = 0; i < No_Buckets; i++)
  {
    option_t entry_exists = ioopm_hash_table_lookup(ht, i);

    if (entry_exists.success)
    {
      return false;
    }
  }
  return true;
}
void ioopm_hash_table_clear(ioopm_hash_table_t *ht)
{
  for (int i = 0; i < No_Buckets; i++)
  {
    entry_destroy(ht->buckets[i]);
    ht->buckets[i] = entry_create(0, NULL, NULL);
  }
}

ioopm_list_t *ioopm_hash_table_keys(ioopm_hash_table_t *ht)
{
  ioopm_list_t *list = ioopm_linked_list_create();

  for (int i = 0; i < No_Buckets; i++)
  {
    entry_t *current = ht->buckets[i];

    while (current->next != NULL)
    {
      ioopm_linked_list_append(list, current->key);

      current = current->next;

    }
  }
  return list;
}

char **ioopm_hash_table_values(ioopm_hash_table_t *ht)
{
  size_t size = ioopm_hash_table_size(ht);
  int n = 0;
  char **arr = calloc(size, sizeof(char *));

  for (int i = 0; i < No_Buckets; i++)
  {
    entry_t *current = ht->buckets[i];

    while (current->next != NULL)
    {
      arr[n] = current->key;
      n++;
      current = current->next;
    }
  }
  return arr;
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
  return (strcmp(other_value, value) == 0);
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

bool ioopm_hash_table_all(ioopm_hash_table_t *ht, ioopm_predicate pred, void *arg)
{
  bool result = true;

  if (ioopm_hash_table_is_empty(ht))
  {
    return false;
  }

  for (int i = 0; i < No_Buckets && result; i++) // Så länge i <bucket och result==true
  {
    entry_t *dummy_entry = ht->buckets[i];
    entry_t *current = dummy_entry->next;
    while (current != NULL && result) // Medans current inte pekar på NULL och True
    {
      result = pred(current->key, current->value, arg); // Updaterar result beroende på vad pred returnar
      current = current->next;
    }
  }
  return result;
}

bool ioopm_hash_table_any(ioopm_hash_table_t *ht, ioopm_predicate pred, void *arg)
{
  bool result = false;

  for (int i = 0; i < No_Buckets && !result; i++) // Så länge i <bucket och result != false
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

void ioopm_hash_table_apply_to_all(ioopm_hash_table_t *ht, ioopm_apply_function apply_fun, void *arg)
{
  for (int i = 0; i < No_Buckets; i++)
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
