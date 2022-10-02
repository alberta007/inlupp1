#include "hash_table.h"
#include <stddef.h>
#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <CUnit/Basic.h>
#include "linked_list.h"

int init_suite(void) {
  // Change this function if you want to do something *before* you
  // run a test suite
  return 0;
}

int clean_suite(void) {
  // Change this function if you want to do something *after* you
  // run a test suite
  return 0;
}

void test_create_destroy()
{
  ioopm_list_t *list = ioopm_linked_list_create();
  CU_ASSERT_PTR_NOT_NULL(list);
  ioopm_linked_list_destroy(list);

  ioopm_list_t *list2 = ioopm_linked_list_create();
  CU_ASSERT_PTR_NOT_NULL(list2);
  ioopm_linked_list_append(list2, 55);
  ioopm_linked_list_destroy(list2);

}
void test_append(){
  ioopm_list_t *list = ioopm_linked_list_create();
  CU_ASSERT_PTR_NOT_NULL(list);
  ioopm_linked_list_append(list, 55);
  ioopm_linked_list_append(list, 425);
  ioopm_linked_list_append(list, 1115);
  CU_ASSERT_TRUE(ioopm_linked_list_contains(list, 55));
  CU_ASSERT_TRUE(ioopm_linked_list_contains(list, 425));
  CU_ASSERT_TRUE(ioopm_linked_list_contains(list, 1115));
  CU_ASSERT_EQUAL(55, ioopm_linked_list_get(list, 1));
  CU_ASSERT_EQUAL(425, ioopm_linked_list_get(list, 2));
  CU_ASSERT_EQUAL(1115, ioopm_linked_list_get(list, 3));
  ioopm_linked_list_destroy(list);
}

void test_prepend(){
  ioopm_list_t *list = ioopm_linked_list_create();
  CU_ASSERT_PTR_NOT_NULL(list);
  ioopm_linked_list_prepend(list, 55);
  ioopm_linked_list_prepend(list, 425);
  ioopm_linked_list_prepend(list, 1115);
  CU_ASSERT_TRUE(ioopm_linked_list_contains(list, 55));
  CU_ASSERT_TRUE(ioopm_linked_list_contains(list, 425));
  CU_ASSERT_TRUE(ioopm_linked_list_contains(list, 1115));
  CU_ASSERT_EQUAL(1115, ioopm_linked_list_get(list, 1));
  CU_ASSERT_EQUAL(425, ioopm_linked_list_get(list, 2));
  CU_ASSERT_EQUAL(55, ioopm_linked_list_get(list, 3));
  ioopm_linked_list_destroy(list);
}

void test_insertion(){
  ioopm_list_t *list = ioopm_linked_list_create();
  ioopm_linked_list_insert(list,1,11111);
  CU_ASSERT_EQUAL(11111, ioopm_linked_list_get(list, 1));
  ioopm_linked_list_insert(list,2,22222);
  CU_ASSERT_EQUAL(22222, ioopm_linked_list_get(list, 2));
  ioopm_linked_list_insert(list,3,33333);
  ioopm_linked_list_insert(list,4,44444);
  CU_ASSERT_EQUAL(44444, ioopm_linked_list_get(list, 4));
  CU_ASSERT_EQUAL(33333, ioopm_linked_list_get(list, 3));
  ioopm_linked_list_destroy(list);
}

void test_remove_link(){
  ioopm_list_t *list = ioopm_linked_list_create();
  ioopm_linked_list_append(list, 55);
  ioopm_linked_list_prepend(list, 45);
  ioopm_linked_list_prepend(list, 35);
  ioopm_linked_list_prepend(list, 25);
  //printf("SIZE: %d\n", ioopm_linked_list_size(list));
  CU_ASSERT_TRUE(ioopm_linked_list_contains(list, 35));
  ioopm_linked_list_remove(list, 2); //Borde ta bort 35 från listans
  CU_ASSERT_FALSE(ioopm_linked_list_contains(list, 35));
  ioopm_linked_list_destroy(list);
}

void test_clear(){
  ioopm_list_t *list = ioopm_linked_list_create();
  ioopm_linked_list_append(list, 55);
  ioopm_linked_list_prepend(list, 45);
  ioopm_linked_list_prepend(list, 35);
  ioopm_linked_list_prepend(list, 25);
  ioopm_linked_list_insert(list,1,11111);
  ioopm_linked_list_insert(list,3,22222);  //DUMMY-> [11111]->[25]->[22222]->[35]->[45]->[55]
  CU_ASSERT_EQUAL(6, ioopm_linked_list_size(list));
  CU_ASSERT_EQUAL(35, ioopm_linked_list_get(list, 4)); //Om insert, append,prepend är rätt ska pos 4 ha value 35
  CU_ASSERT_EQUAL(11111, ioopm_linked_list_get(list, 1));
  CU_ASSERT_EQUAL(25, ioopm_linked_list_get(list, 2));
  ioopm_linked_list_clear(list);
  CU_ASSERT_EQUAL(0, ioopm_linked_list_size(list));
  ioopm_linked_list_destroy(list);
}

void test_get(){
  ioopm_list_t *list = ioopm_linked_list_create();
  ioopm_linked_list_append(list, 55);
  ioopm_linked_list_append(list, 75);
  ioopm_linked_list_prepend(list, 45);
  ioopm_linked_list_prepend(list, -35);
  ioopm_linked_list_append(list, 6);   //order: [-35]->[45]->[55]->[75]->[6]
  //printf("Size: %d\n", ioopm_linked_list_size(list));
  CU_ASSERT_EQUAL(-35, ioopm_linked_list_get(list, 1));
  CU_ASSERT_EQUAL(45, ioopm_linked_list_get(list, 2));
  CU_ASSERT_EQUAL(55, ioopm_linked_list_get(list, 3));
  CU_ASSERT_TRUE(ioopm_linked_list_contains(list, 75));
  CU_ASSERT_EQUAL(6, ioopm_linked_list_get(list, 5));
  ioopm_linked_list_destroy(list);
}

void test_linked_list_all(){
   bool keys_bigger_value(int key, int value, void *x)
    {
      return (key > value);
    }

  ioopm_list_t *list = ioopm_linked_list_create();
  // Test with empty ht
  CU_ASSERT_FALSE(ioopm_linked_list_all(list, keys_bigger_value , NULL));
  ioopm_linked_list_append(list, 2);
  ioopm_linked_list_append(list, -7);
  ioopm_linked_list_prepend(list, 1);
  ioopm_linked_list_prepend(list, -35);//index:   1     2    3     4    5
  ioopm_linked_list_append(list, 4);   //order: [-35]->[1]->[2]->[-7]->[4]
  CU_ASSERT_TRUE(ioopm_linked_list_all(list, keys_bigger_value , NULL));
  ioopm_linked_list_append(list, 55);
  ioopm_linked_list_append(list, 56);
  ioopm_linked_list_append(list, 57);
  CU_ASSERT_FALSE(ioopm_linked_list_all(list, keys_bigger_value , NULL));
  ioopm_linked_list_destroy(list);
}

void test_linked_list_any(){
   bool value_bigger_keys(int key, int value, void *x)
    {
      return (value > key);
    }

  ioopm_list_t *list = ioopm_linked_list_create();
  // Test with empty ht
  CU_ASSERT_FALSE(ioopm_linked_list_any(list, value_bigger_keys , NULL));
  ioopm_linked_list_append(list, 3);
  ioopm_linked_list_append(list, -7);
  ioopm_linked_list_prepend(list, -4);
  ioopm_linked_list_prepend(list, -35);
  ioopm_linked_list_append(list, 5);   //order: [-35]->[-4]->[3]->[-7]->[5]
  CU_ASSERT_FALSE(ioopm_linked_list_any(list, value_bigger_keys , NULL)); //Test with only keys>values
  ioopm_linked_list_append(list, 55);
  ioopm_linked_list_append(list, 56);
  ioopm_linked_list_append(list, 57);
  CU_ASSERT_TRUE(ioopm_linked_list_any(list, value_bigger_keys , NULL)); //Test with some values>keys == return TRUE
  ioopm_linked_list_destroy(list);
}

void test_linked_list_apply_to_all(){
  void increase_small_keys (int key, int *value, void *x)
  {
      if (key <= 5)
      {
        *value = 500;
      }
  }
  ioopm_list_t *list = ioopm_linked_list_create();
  //Test to apply to an empty list:
  ioopm_linked_list_apply_to_all(list, increase_small_keys, NULL);

  ioopm_linked_list_insert(list,1,1);
  ioopm_linked_list_insert(list,2,2);
  ioopm_linked_list_insert(list,3,3);
  ioopm_linked_list_insert(list,4,4);
  ioopm_linked_list_insert(list,5,5);
  ioopm_linked_list_insert(list,6,100);
  ioopm_linked_list_insert(list,7,101);
  ioopm_linked_list_insert(list,8,102);

  //ALL the values in the list should be the same as they wee at insertion:
  CU_ASSERT_EQUAL(1, ioopm_linked_list_get(list, 1));
  CU_ASSERT_EQUAL(2, ioopm_linked_list_get(list, 2));
  CU_ASSERT_EQUAL(100, ioopm_linked_list_get(list, 6));

  //Use the  apply to all function:
  ioopm_linked_list_apply_to_all(list, increase_small_keys, NULL);

  //Check that value has changed for keys <= 5:
  CU_ASSERT_EQUAL(500, ioopm_linked_list_get(list, 1));
  CU_ASSERT_EQUAL(500, ioopm_linked_list_get(list, 2));
  CU_ASSERT_EQUAL(500, ioopm_linked_list_get(list, 3));
  CU_ASSERT_EQUAL(500, ioopm_linked_list_get(list, 4));
  CU_ASSERT_EQUAL(500, ioopm_linked_list_get(list, 5));

  //Check that value is unchanged for keys > 5:
  CU_ASSERT_EQUAL(100, ioopm_linked_list_get(list, 6));
  CU_ASSERT_EQUAL(101, ioopm_linked_list_get(list, 7));
  CU_ASSERT_EQUAL(102, ioopm_linked_list_get(list, 8));
  // Destroy list:
  ioopm_linked_list_destroy(list);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main() {
  // First we try to set up CUnit, and exit if we fail
  if (CU_initialize_registry() != CUE_SUCCESS)
    return CU_get_error();

  // We then create an empty test suite and specify the name and
  // the init and cleanup functions
  CU_pSuite my_test_suite = CU_add_suite("My awesome test suite", init_suite, clean_suite);
  if (my_test_suite == NULL) {
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
    (CU_add_test(my_test_suite, "Create and Destroy list", test_create_destroy)==NULL)||
    (CU_add_test(my_test_suite, "Append", test_append)==NULL)||
    (CU_add_test(my_test_suite, "Prepend", test_prepend)==NULL)||
    (CU_add_test(my_test_suite, "Insert", test_insertion)==NULL)||
    (CU_add_test(my_test_suite, "Remove", test_remove_link)==NULL)||
    (CU_add_test(my_test_suite, "Clear", test_clear)==NULL)||
    (CU_add_test(my_test_suite, "Get function", test_get)==NULL)||
    (CU_add_test(my_test_suite, "All function", test_linked_list_all)==NULL)||
    (CU_add_test(my_test_suite, "Any function", test_linked_list_any)==NULL)||
    (CU_add_test(my_test_suite, "Apply to all function", test_linked_list_apply_to_all)==NULL)||
    0
  )
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
