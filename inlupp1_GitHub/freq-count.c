#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "hash_table.h"
#include "linked_list.h"
#include "iterator.h"
#include "common.h"

#define Delimiters "+-#@()[]{}.,:;!? \t\n\r"

static int cmpstringp(const void *p1, const void *p2)
{
  return strcmp(*(char *const *)p1, *(char *const *)p2);
}

void sort_keys(elem_t keys[], size_t no_keys)
{
  qsort(keys, no_keys, sizeof(char *), cmpstringp);
}

void process_word(char *word, ioopm_hash_table_t *ht)
{
  if(ioopm_hash_table_has_key(ht, str_elem(word)) == false) //If the word doesn't exist yet, set the count to 1.
  {
    char *word_copy = strdup(word);
    ioopm_hash_table_insert(ht, str_elem(word_copy), int_elem(1));
  }
  else //Otherwise update the value.
  {
    int count = ioopm_hash_table_lookup(ht, str_elem(word)).value.i;
    count++;
    ioopm_hash_table_insert(ht, str_elem(word), int_elem(count));
  }
}

void process_file(char *filename, ioopm_hash_table_t *ht)
{
  FILE *f = fopen(filename, "r");

  while (true)
  {
    char *buf = NULL;
    size_t len = 0;
    getline(&buf, &len, f);

    if (feof(f))
    {
      free(buf);
      break;
    }

    for (char *word = strtok(buf, Delimiters);
         word && *word;
         word = strtok(NULL, Delimiters))
    {
      process_word(word, ht);
    }

    free(buf);
  }

  fclose(f);
}

int main(int argc, char *argv[])
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create((ioopm_hash_function) string_knr_hash, cmp_str); /*, NULL);*/

  if (argc > 1)
  {
    for (int i = 1; i < argc; ++i)
    {
      process_file(argv[i], ht);
    }
    //Insert all keys from the list into an array using the iterator:
    ioopm_list_t *list = ioopm_hash_table_keys(ht);
    size_t size = ioopm_hash_table_size(ht);
    ioopm_list_iterator_t *iter = ioopm_list_iterator(list);
    elem_t key_arr[size];
    for(int k=0; k<size; k++){
      key_arr[k] = ioopm_iterator_next(iter);
    }
    sort_keys(key_arr, size);   //Sorting the array in lexicographic order

    //Prints out all elements in the array (in order):
    for (int k = 0; k < size; k++)
    {
      int freq = ioopm_hash_table_lookup(ht, key_arr[k]).value.i;
      printf("%s: %d\n", key_arr[k].str, freq);
    }

    //Free the list, array and the iterator:
    for(int k = 0; k < size; k++)
    {
      free(key_arr[k].str);
    }
    ioopm_linked_list_destroy(list);
    ioopm_iterator_destroy(iter);
  }
  else
  {
    puts("Usage: freq-count file1 ... filen");
  }
  ioopm_hash_table_destroy(ht);
}
