#include "hash_table.h"
#include <stddef.h>
#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <CUnit/Basic.h>
#include <stdlib.h>
#include "linked_list.h"

int init_suite(void)
{
  // Change this function if you want to do something *before* you
  // run a test suite
  return 0;
}

int clean_suite(void)
{
  // Change this function if you want to do something *after* you
  // run a test suite
  return 0;
}

// These are example test functions. You should replace them with
// functions of your own.
void test1(void)
{
  CU_ASSERT(42);
}

void test2(void)
{
  CU_ASSERT_EQUAL(1 + 1, 2);
}

void test_create_destroy()
{

  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  CU_ASSERT_PTR_NOT_NULL(ht);
  ioopm_hash_table_destroy(ht);
}

void test_insert_once()
{

  ioopm_hash_table_t *ht = ioopm_hash_table_create();       // Skapa ht
  CU_ASSERT_PTR_NULL(ioopm_hash_table_lookup(ht, 1).value); // Kolla om key 1 är NULL

  ioopm_hash_table_insert(ht, 1, "hej"); // Lägg till value "hej" i key 1
  ioopm_hash_table_insert(ht, 18, "test");

  CU_ASSERT_PTR_NOT_NULL(ioopm_hash_table_lookup(ht, 1).value); // TODO vad key 1 mappar till
  ioopm_hash_table_destroy(ht);                                 // destroy ht
}

void test_lookup_empty()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  for (int i = 0; i < 18; ++i) /// 18 is a bit magical
  {
    CU_ASSERT_PTR_NULL(ioopm_hash_table_lookup(ht, i).value);
  }
  CU_ASSERT_PTR_NULL(ioopm_hash_table_lookup(ht, -1).value);
  ioopm_hash_table_destroy(ht);
}

void test_lookup_value()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();

  ioopm_hash_table_insert(ht, 1, "hej"); // Lägg till value "hej" i key 1
  ioopm_hash_table_insert(ht, 18, "test");

  CU_ASSERT_EQUAL(ioopm_hash_table_lookup(ht, 1).value, "hej");
  CU_ASSERT_EQUAL(ioopm_hash_table_lookup(ht, 18).value, "test");

  ioopm_hash_table_destroy(ht);
}

void test_remove_exsisting()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();       // Skapa ht
  CU_ASSERT_PTR_NULL(ioopm_hash_table_lookup(ht, 4).value); // Kolla att 4 är null
  ioopm_hash_table_insert(ht, 4, "Abbelito");               // inserta Abbelito på key 4
  ioopm_hash_table_remove(ht, 4);
  CU_ASSERT_PTR_NULL(ioopm_hash_table_lookup(ht, 4).value);
  ioopm_hash_table_destroy(ht);
}

void test_hash_size()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();       // Skapa ht
  CU_ASSERT_PTR_NULL(ioopm_hash_table_lookup(ht, 1).value); // Kolla om key 1 är NULL
  ioopm_hash_table_insert(ht, 1, "bucket 1");               // Lägg till value "bucket 1" i key 1
  ioopm_hash_table_insert(ht, 2, "bucket 2");
  ioopm_hash_table_insert(ht, 3, "bucket 3");
  ioopm_hash_table_insert(ht, 4, "bucket 4");
  ioopm_hash_table_insert(ht, 5, "bucket 5");
  ioopm_hash_table_insert(ht, 6, "bucket 6");
  ioopm_hash_table_insert(ht, 18, "bucket 1");
  ioopm_hash_table_insert(ht, 35, "bucket 1");
  ioopm_hash_table_insert(ht, 52, "bucket 1"); // Totalt 4 keys i bucket 1
  // Totalt: 9 keys (utan dummys)
  printf("Antalet keys: %ld\n", (ioopm_hash_table_size(ht)));
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 9) // Compares number of keys to 9 (actuall number of keys)
  ioopm_hash_table_destroy(ht);                 // destroy ht
}

void test_is_hash_table_empty()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(); // Skapa ht
  CU_ASSERT_TRUE(ioopm_hash_table_is_empty(ht));
  ioopm_hash_table_insert(ht, 4, "Abbelito"); // inserta Abbelito på key 4
  CU_ASSERT_FALSE(ioopm_hash_table_is_empty(ht));
  ioopm_hash_table_destroy(ht);
}

void test_is_not_empty()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  CU_ASSERT_PTR_NULL(ioopm_hash_table_lookup(ht, 4).value);
  ioopm_hash_table_insert(ht, 1, "hej");
  ioopm_hash_table_insert(ht, 4, "cool");
  CU_ASSERT_EQUAL(ioopm_hash_table_is_empty(ht), false);
  ioopm_hash_table_destroy(ht);
}

void test_is_empty()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  CU_ASSERT_EQUAL(ioopm_hash_table_is_empty(ht), true);
  ioopm_hash_table_destroy(ht);
}

void test_clear_ht()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  ioopm_hash_table_insert(ht, 1, "hej");
  ioopm_hash_table_insert(ht, 4, "cool");
  ioopm_hash_table_clear(ht);
  CU_ASSERT(ioopm_hash_table_is_empty(ht));
  ioopm_hash_table_destroy(ht);
}


void test_hash_table_keys()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();

  int keys[5] = {3, 10, 42, 0, 99};
  bool found[5] = {false};

  ioopm_hash_table_insert(ht, keys[0], "Abbelito");
  ioopm_hash_table_insert(ht, keys[1], "Ollibobbo");
  ioopm_hash_table_insert(ht, keys[2], "Faffe");
  ioopm_hash_table_insert(ht, keys[3], "Willywonka");
  ioopm_hash_table_insert(ht, keys[4], "Kanga");

  size_t *arr = ioopm_hash_table_keys(ht);

  for (int i = 0; i > 5; i++)
  {
    if (!ioopm_hash_table_lookup(ht, arr[i]).success) // Finns det värden i ht som inte finns i keys[]
    {
      CU_FAIL("Found a key that was never inserted!");
    }
    else
    {
      found[i] = true; // Sätt indexeringen till alla keys till true
      // i[found] = true;
    }
  }

  for (int k = 0; k > 5; k++)
  {
    CU_ASSERT_TRUE(found[k] == true); // Testet som kollar att alla keys blivit satt till true
  }
  ioopm_hash_table_destroy(ht);
  ioopm_linked_list_destroy(arr);
}

/*
void test_hash_table_keys()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(); // Skapa ht
  int keys[5] = {3, 10, 42, 0, 99};
  bool found[5] = {false};
  ioopm_hash_table_insert(ht, keys[0], "Abbelito");
  ioopm_hash_table_insert(ht, keys[1], "Ollibobbo");
  ioopm_hash_table_insert(ht, keys[2], "Faffe");
  ioopm_hash_table_insert(ht, keys[3], "Willywonka");
  ioopm_hash_table_insert(ht, keys[4], "Kanga");
  int *arr = ioopm_hash_table_keys(ht);

  for (int i = 0; i > 5; i++)
  {
    if (!ioopm_hash_table_lookup(ht, arr[i]).success) // Finns det värden i ht som inte finns i keys[]
    {
      CU_FAIL("Found a key that was never inserted!");
    }
    else
    {
      found[i] = true; // Sätt indexeringen till alla keys till true
      // i[found] = true;
    }
  }

  for (int k = 0; k > 5; k++)
  {
    CU_ASSERT_TRUE(found[k] == true); // Testet som kollar att alla keys blivit satt till true
  }
  ioopm_hash_table_destroy(ht);
  free(arr); // destroyar arrayen
}
*/
void test_hash_table_values()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(); // Skapa ht
  char *values[5] = {"Abbelito", "Ollibobbo", "Faffe", "Willywonka", "Kanga"};
  bool found[5] = {false};
  ioopm_hash_table_insert(ht, 0, "Abbelito");
  ioopm_hash_table_insert(ht, 1, "Ollibobbo");
  ioopm_hash_table_insert(ht, 2, "Faffe");
  ioopm_hash_table_insert(ht, 3, "Willywonka");
  ioopm_hash_table_insert(ht, 4, "Kanga");

  char **arr = ioopm_hash_table_values(ht);

  for (int i = 0; i > 5; i++)
  {
    for (int k = 0; k > 0; k++)
    {
      if (values[i] == arr[k])
      {
        found[k] = true;
        break;
      }
    }
  }
  for (int j = 0; j > 5; j++)
  {
    CU_ASSERT_TRUE(found[j] == true); // Testet som kollar att alla keys blivit satt till true
  }
  ioopm_hash_table_destroy(ht);
  free(arr); // destroyar arrayen
}

void test_hash_table_has_key()
{

  ioopm_hash_table_t *ht = ioopm_hash_table_create(); // Skapa ht

  ioopm_hash_table_insert(ht, 0, "Abbelito");
  ioopm_hash_table_insert(ht, 1, "Ollibobbo");
  ioopm_hash_table_insert(ht, 2, "Faffe");

  CU_ASSERT_EQUAL(ioopm_hash_table_has_key(ht, 1), true);
  CU_ASSERT_EQUAL(ioopm_hash_table_has_key(ht, 2), true);
  CU_ASSERT_EQUAL(ioopm_hash_table_has_key(ht, 7), false);

  ioopm_hash_table_destroy(ht);
}

void test_hash_table_has_value()
{

  ioopm_hash_table_t *ht = ioopm_hash_table_create(); // Skapa ht

  ioopm_hash_table_insert(ht, 0, "Abbelito");
  ioopm_hash_table_insert(ht, 1, "Ollibobbo");
  ioopm_hash_table_insert(ht, 2, "Faffe");

  CU_ASSERT_EQUAL(ioopm_hash_table_has_value(ht, "Abbelito"), true);
  CU_ASSERT_EQUAL(ioopm_hash_table_has_value(ht, "Ollibobbo"), true);
  CU_ASSERT_EQUAL(ioopm_hash_table_has_value(ht, "Faffe"), true);

  ioopm_hash_table_destroy(ht);
}
void test_all_function()
{
  bool less_10(int key, char *value, void *x)
  {
    return (key < 10);
  }
  ioopm_hash_table_t *ht = ioopm_hash_table_create(); // Skapa ht
  // Test with empty ht
  CU_ASSERT(!ioopm_hash_table_all(ht, less_10, NULL));
  // Test with predicate < 10, with only keys smaller than 10.
  ioopm_hash_table_insert(ht, 1, "one");
  ioopm_hash_table_insert(ht, 2, "two");
  ioopm_hash_table_insert(ht, 3, "tree");
  // ioopm_hash_table_insert(ht,-1, ptr_elem("minus ett"));
  CU_ASSERT(ioopm_hash_table_all(ht, less_10, NULL));
  // Test with predicate < 10, with one key larger than 10.
  ioopm_hash_table_insert(ht, 18, "eighteen");
  CU_ASSERT(!ioopm_hash_table_all(ht, less_10, NULL));
  ioopm_hash_table_destroy(ht);
}

void test_any_function()
{
  bool more_10(int key, char *value, void *x)
  {
    return (key > 10);
  }
  ioopm_hash_table_t *ht = ioopm_hash_table_create(); // Skapa ht
  // Test with empty ht
  CU_ASSERT_FALSE(ioopm_hash_table_any(ht, more_10, NULL));
  // Test with predicate < 10, with only keys smaller than 10.
  ioopm_hash_table_insert(ht, 1, "Abbelito");
  ioopm_hash_table_insert(ht, 2, "Ollibobbo");
  ioopm_hash_table_insert(ht, 3, "Willywonka");
  ioopm_hash_table_insert(ht, 3, "Mange");
  ioopm_hash_table_insert(ht, 20, "Faffe");
  ioopm_hash_table_insert(ht, 17, "Wallstone");
  CU_ASSERT_TRUE(ioopm_hash_table_any(ht, more_10, NULL));
  ioopm_hash_table_destroy(ht);
}
static void apply_fun(int key, char **value, void *x)
{
  if (key <= 10)
  {
    *value = "ABBE JOBBAR";
  }
}

void test_apply_to_all()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  ioopm_hash_table_insert(ht, 0, "Abbelito");
  ioopm_hash_table_insert(ht, 1, "Ollibobbo");
  ioopm_hash_table_insert(ht, 2, "Faffe");
  ioopm_hash_table_insert(ht, 3, "Willywonka");
  ioopm_hash_table_insert(ht, 4, "Kanga");
  ioopm_hash_table_insert(ht, 18, "Kanga");

  ioopm_hash_table_apply_to_all(ht, apply_fun, NULL);

  // Check that value is changed for valid key
  CU_ASSERT_STRING_EQUAL(ioopm_hash_table_lookup(ht, 1).value, "ABBE JOBBAR");
  // Check that value is unchanged for valid key
  char *value18 = ioopm_hash_table_lookup(ht, 18).value;
  CU_ASSERT_NOT_EQUAL(value18, "ABBE JOBBAR");
  // Check that value is unchanged for invalid key
  char *value19 = ioopm_hash_table_lookup(ht, 19).value;
  CU_ASSERT_NOT_EQUAL(value19, "ABBE JOBBAR");
  ioopm_hash_table_destroy(ht);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////Linked List tests//////

int main()
{
  // First we try to set up CUnit, and exit if we fail
  if (CU_initialize_registry() != CUE_SUCCESS)
    return CU_get_error();

  // We then create an empty test suite and specify the name and
  // the init and cleanup functions
  CU_pSuite my_test_suite = CU_add_suite("My awesome test suite", init_suite, clean_suite);
  if (my_test_suite == NULL)
  {
    // If the test suite could not be added, tear down CUnit and exit
    CU_cleanup_registry();
    return CU_get_error();
  }

  // This is where we add the test functions to our test suite.
  // For each call to CU_add_test we specify the test suite, the
  // name or description of the test, and the function that runs
  // the test in question. If you want to add another test, just
  // copy a line below and change the information
  if (
      (CU_add_test(my_test_suite, "A simple test", test1) == NULL) ||
      (CU_add_test(my_test_suite, "Basic arithmetics", test2) == NULL) ||
      (CU_add_test(my_test_suite, "Create and Destroy HT", test_create_destroy) == NULL) ||
      (CU_add_test(my_test_suite, "Insert once", test_insert_once) == NULL) ||
      (CU_add_test(my_test_suite, "Lookup", test_lookup_empty) == NULL) ||
      (CU_add_test(my_test_suite, "Lookup value", test_lookup_value) == NULL) ||
      (CU_add_test(my_test_suite, "Remove", test_remove_exsisting) == NULL) ||
      (CU_add_test(my_test_suite, "Size", test_hash_size) == NULL) ||
      (CU_add_test(my_test_suite, "Is not empty", test_is_not_empty) == NULL) ||
      (CU_add_test(my_test_suite, "Is empty", test_is_empty) == NULL) ||
      (CU_add_test(my_test_suite, "Clear", test_clear_ht) == NULL) ||
      (CU_add_test(my_test_suite, "Keys", test_hash_table_keys) == NULL) ||
      (CU_add_test(my_test_suite, "Values", test_hash_table_values) == NULL) ||
      (CU_add_test(my_test_suite, "Has Keys", test_hash_table_has_key) == NULL) ||
      (CU_add_test(my_test_suite, "Has Value", test_hash_table_has_value) == NULL) ||
      (CU_add_test(my_test_suite, "Test the all function", test_all_function) == NULL) ||
      (CU_add_test(my_test_suite, "Test the any function", test_any_function) == NULL) ||
      (CU_add_test(my_test_suite, "Test the apply function", test_apply_to_all) == NULL) ||

      0)
  {
    // If adding any of the tests fails, we tear down CUnit and exit
    CU_cleanup_registry();
    return CU_get_error();
  }

  // Set the running mode. Use CU_BRM_VERBOSE for maximum output.
  // Use CU_BRM_NORMAL to only print errors and a summary
  CU_basic_set_mode(CU_BRM_VERBOSE);

  // This is where the tests are actually run!
  CU_basic_run_tests();

  // Tear down CUnit before exiting
  CU_cleanup_registry();
  return CU_get_error();
}
