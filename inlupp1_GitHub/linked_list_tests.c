
//#include "hash_table.h"
#include "linked_list.h"
#include <stddef.h>
#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <CUnit/Basic.h>
#include "iterator.h"
//#include "common.h"

static bool cmp_int(elem_t int1, elem_t int2)
{
  return ((int1.i - int2.i) == 0);
}

static bool cmp_str(elem_t str1, elem_t str2)
{
  return (strcmp(str1.str, str2.str) == 0);
}



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
  ioopm_list_t *list = ioopm_linked_list_create(cmp_int);
  CU_ASSERT_PTR_NOT_NULL(list);
  ioopm_linked_list_destroy(list);

  ioopm_list_t *list2 = ioopm_linked_list_create(cmp_int);
  CU_ASSERT_PTR_NOT_NULL(list2);
  //ioopm_linked_list_append(list2, 55);
  ioopm_linked_list_destroy(list2);
}

void test_append(){
  ioopm_list_t *list = ioopm_linked_list_create(cmp_int);
  CU_ASSERT_PTR_NOT_NULL(list);
  ioopm_linked_list_append(list, int_elem(55));
  ioopm_linked_list_append(list, int_elem(425));
  ioopm_linked_list_append(list, int_elem(1115));
  CU_ASSERT_TRUE(ioopm_linked_list_contains(list, int_elem(55)));
  CU_ASSERT_TRUE(ioopm_linked_list_contains(list, int_elem(425)));
  CU_ASSERT_TRUE(ioopm_linked_list_contains(list, int_elem(1115)));
  CU_ASSERT_EQUAL(55, ioopm_linked_list_get(list, 1).i);
  CU_ASSERT_EQUAL(425, ioopm_linked_list_get(list, 2).i);
  CU_ASSERT_EQUAL(1115, ioopm_linked_list_get(list, 3).i);
  ioopm_linked_list_destroy(list);
}

void test_prepend(){
  ioopm_list_t *list = ioopm_linked_list_create(cmp_int);
  CU_ASSERT_PTR_NOT_NULL(list);
  ioopm_linked_list_prepend(list, int_elem(55));
  ioopm_linked_list_prepend(list, int_elem(425));
  ioopm_linked_list_prepend(list, int_elem(1115));
  CU_ASSERT_TRUE(ioopm_linked_list_contains(list, int_elem(55)));
  CU_ASSERT_TRUE(ioopm_linked_list_contains(list, int_elem(425)));
  CU_ASSERT_TRUE(ioopm_linked_list_contains(list, int_elem(1115)));
  CU_ASSERT_EQUAL(1115, ioopm_linked_list_get(list, 1).i);
  CU_ASSERT_EQUAL(425, ioopm_linked_list_get(list, 2).i);
  CU_ASSERT_EQUAL(55, ioopm_linked_list_get(list, 3).i);
  ioopm_linked_list_destroy(list);
}

void test_insertion(){
  ioopm_list_t *list = ioopm_linked_list_create(cmp_int);
  ioopm_linked_list_insert(list,1,int_elem(11111));
  CU_ASSERT_EQUAL(11111, ioopm_linked_list_get(list, 1).i);
  ioopm_linked_list_insert(list,2,int_elem(22222));
  CU_ASSERT_EQUAL(22222, ioopm_linked_list_get(list, 2).i);
  ioopm_linked_list_insert(list,3,int_elem(33333));
  ioopm_linked_list_insert(list,4,int_elem(44444));
  CU_ASSERT_EQUAL(44444, ioopm_linked_list_get(list, 4).i);
  CU_ASSERT_EQUAL(33333, ioopm_linked_list_get(list, 3).i);
  ioopm_linked_list_destroy(list);
}

void test_remove_link(){
  ioopm_list_t *list = ioopm_linked_list_create(cmp_int);
  ioopm_linked_list_append(list, int_elem(55));
  ioopm_linked_list_prepend(list, int_elem(45));
  ioopm_linked_list_prepend(list, int_elem(35));
  ioopm_linked_list_prepend(list, int_elem(25));
  //printf("SIZE: %d\n", ioopm_linked_list_size(list));
  CU_ASSERT_TRUE(ioopm_linked_list_contains(list, int_elem(25)));
  CU_ASSERT_EQUAL(35, ioopm_linked_list_remove(list, 2).i); //Borde ta bort 35 från listans
  CU_ASSERT_FALSE(ioopm_linked_list_contains(list, int_elem(35)));
  ioopm_linked_list_destroy(list);
}

void test_clear(){
  ioopm_list_t *list = ioopm_linked_list_create(cmp_int);
  ioopm_linked_list_append(list, int_elem(55));
  ioopm_linked_list_prepend(list, int_elem(45));
  ioopm_linked_list_prepend(list, int_elem(35));
  ioopm_linked_list_prepend(list, int_elem(25));
  ioopm_linked_list_insert(list,1,int_elem(11111));
  ioopm_linked_list_insert(list,3,int_elem(22222));  //DUMMY-> [11111]->[25]->[22222]->[35]->[45]->[55]
  CU_ASSERT_EQUAL(6, ioopm_linked_list_size(list));
  CU_ASSERT_EQUAL(35, ioopm_linked_list_get(list,4).i); //Om insert, append,prepend är rätt ska pos 4 ha value 35
  CU_ASSERT_EQUAL(11111, ioopm_linked_list_get(list, 1).i);
  CU_ASSERT_EQUAL(25, ioopm_linked_list_get(list, 2).i);
  ioopm_linked_list_clear(list);
  CU_ASSERT_EQUAL(0, ioopm_linked_list_size(list));
  ioopm_linked_list_destroy(list);
}

void test_get(){
  ioopm_list_t *list = ioopm_linked_list_create(cmp_int);
  ioopm_linked_list_append(list, int_elem(55));
  ioopm_linked_list_append(list, int_elem(75));
  ioopm_linked_list_prepend(list, int_elem(45));
  ioopm_linked_list_prepend(list, int_elem(-35));
  ioopm_linked_list_append(list, int_elem(6));   //order: [-35]->[45]->[55]->[75]->[6]
  //printf("Size: %d\n", ioopm_linked_list_size(list));
  CU_ASSERT_EQUAL(-35, ioopm_linked_list_get(list, 1).i);
  CU_ASSERT_EQUAL(45, ioopm_linked_list_get(list, 2).i);
  CU_ASSERT_EQUAL(55, ioopm_linked_list_get(list, 3).i);
  CU_ASSERT_TRUE(ioopm_linked_list_contains(list, int_elem(75)));
  CU_ASSERT_EQUAL(6, ioopm_linked_list_get(list, 5).i);
  ioopm_linked_list_destroy(list);
}

void test_linked_list_all(){
   bool rtrn_small_values(int key, elem_t value, void *x)
    {
      return value.i<100;
    }

  ioopm_list_t *list = ioopm_linked_list_create(cmp_int);
  // Test with empty ht
  CU_ASSERT_FALSE(ioopm_linked_list_all(list, rtrn_small_values , NULL));
  ioopm_linked_list_append(list, int_elem(2));
  ioopm_linked_list_append(list, int_elem(-7));
  ioopm_linked_list_prepend(list, int_elem(1));
  ioopm_linked_list_prepend(list, int_elem(-35));//index:   1     2    3     4    5
  ioopm_linked_list_append(list, int_elem(4));   //order: [-35]->[1]->[2]->[-7]->[4]
  CU_ASSERT_TRUE(ioopm_linked_list_all(list, rtrn_small_values , NULL));
  ioopm_linked_list_append(list, int_elem(200));
  ioopm_linked_list_append(list, int_elem(2233));
  ioopm_linked_list_append(list, int_elem(102));
  CU_ASSERT_FALSE(ioopm_linked_list_all(list, rtrn_small_values , NULL));
  ioopm_linked_list_destroy(list);
}

void test_linked_list_any(){
  bool retrn_big_values(int key, elem_t value, void *x)
   {
     return value.i>100;
   }

  ioopm_list_t *list = ioopm_linked_list_create(cmp_int);
  // Test with empty ht
  CU_ASSERT_FALSE(ioopm_linked_list_any(list, retrn_big_values , NULL));
  ioopm_linked_list_append(list, int_elem(2));
  ioopm_linked_list_append(list, int_elem(-7));
  ioopm_linked_list_prepend(list, int_elem(1));
  ioopm_linked_list_prepend(list, int_elem(-35));//index:   1     2    3     4    5
  ioopm_linked_list_append(list, int_elem(4));   //order: [-35]->[1]->[2]->[-7]->[4]
  CU_ASSERT_FALSE(ioopm_linked_list_any(list, retrn_big_values , NULL)); //Test with only keys>values
  ioopm_linked_list_append(list, int_elem(200));
  ioopm_linked_list_append(list, int_elem(100));
  ioopm_linked_list_append(list, int_elem(99));
  CU_ASSERT_TRUE(ioopm_linked_list_any(list, retrn_big_values , NULL)); //Test with some values>keys == return TRUE
  ioopm_linked_list_destroy(list);
}

void test_linked_list_apply_to_all(){
  void change_small_keys (int key, elem_t *value, void *x)
  {
    if (key <= 5)
    {
        value->i = 500;
    }
  }
  ioopm_list_t *list = ioopm_linked_list_create(cmp_int);
  //Test to apply to an empty list:
  ioopm_linked_list_apply_to_all(list, change_small_keys, NULL);

  ioopm_linked_list_insert(list,1,int_elem(1));
  ioopm_linked_list_insert(list,2,int_elem(2));
  ioopm_linked_list_insert(list,3,int_elem(3));
  ioopm_linked_list_insert(list,4,int_elem(4));
  ioopm_linked_list_insert(list,5,int_elem(5));
  ioopm_linked_list_insert(list,6,int_elem(100));
  ioopm_linked_list_insert(list,7,int_elem(101));
  ioopm_linked_list_insert(list,8,int_elem(102));

  //ALL the values in the list should be the same as they wee at insertion:
  CU_ASSERT_EQUAL(1, ioopm_linked_list_get(list, 1).i);
  CU_ASSERT_EQUAL(2, ioopm_linked_list_get(list, 2).i);
  CU_ASSERT_EQUAL(100, ioopm_linked_list_get(list, 6).i);

  //Use the  apply to all function:
  ioopm_linked_list_apply_to_all(list, change_small_keys, NULL);

  //Check that value has changed for keys <= 5:
  CU_ASSERT_EQUAL(500, ioopm_linked_list_get(list, 1).i);
  CU_ASSERT_EQUAL(500, ioopm_linked_list_get(list, 2).i);
  CU_ASSERT_EQUAL(500, ioopm_linked_list_get(list, 3).i);
  CU_ASSERT_EQUAL(500, ioopm_linked_list_get(list, 4).i);
  CU_ASSERT_EQUAL(500, ioopm_linked_list_get(list, 5).i);

  //Check that value is unchanged for keys > 5:
  CU_ASSERT_EQUAL(100, ioopm_linked_list_get(list, 6).i);
  CU_ASSERT_EQUAL(101, ioopm_linked_list_get(list, 7).i);
  CU_ASSERT_EQUAL(102, ioopm_linked_list_get(list, 8).i);

  // Destroy list:
  ioopm_linked_list_destroy(list);

/* EJ FUNGERANDE TODO???
ioopm_list_t *str_list = ioopm_linked_list_create(cmp_str);
ioopm_linked_list_insert(list,1,char_elem("Abbe"));
ioopm_linked_list_insert(list,2,char_elem("Ollibobbo"));
ioopm_linked_list_insert(list,3,char_elem("Kangas"));
ioopm_linked_list_insert(list,4,char_elem("Faffe"));
ioopm_linked_list_insert(list,5,char_elem("LUUDI"));
ioopm_linked_list_apply_to_all(str_list, change_small_keys, NULL);
CU_ASSERT_EQUAL("KINGS", ioopm_linked_list_get(list, 1).c);
CU_ASSERT_EQUAL("KINGS", ioopm_linked_list_get(list, 2).c);
CU_ASSERT_EQUAL("KINGS", ioopm_linked_list_get(list, 3).c);
CU_ASSERT_EQUAL("KINGS", ioopm_linked_list_get(list, 4).c);
CU_ASSERT_EQUAL("KINGS", ioopm_linked_list_get(list, 5).c);
  ioopm_linked_list_destroy(str_list);
  */
}

////////////////////////////////////////////////////////////////////////////////
                      /*    ITERATOR TESTINGFUNCTIONS   */
////////////////////////////////////////////////////////////////////////////////

void test_has_next_iter(){
  ioopm_list_t *list = ioopm_linked_list_create(cmp_int);
  ioopm_list_iterator_t *iter = ioopm_list_iterator(list);
  //Check if empty list as next
  CU_ASSERT_FALSE(ioopm_iterator_has_next(iter));
  ioopm_linked_list_prepend(list,int_elem(233));
  ioopm_linked_list_insert(list,1,int_elem(2));
  ioopm_linked_list_append(list, int_elem(100));
  ioopm_linked_list_append(list, int_elem(200));   // [2]->[233]->[100]->[200]
//Check if non-empty list has next
  ioopm_iterator_reset(iter);
  CU_ASSERT_TRUE(ioopm_iterator_has_next(iter));
  ioopm_iterator_destroy(iter);
  ioopm_linked_list_destroy(list);
}

void test_next_iter(){
  ioopm_list_t *list = ioopm_linked_list_create(cmp_int);
  ioopm_list_iterator_t *iter = ioopm_list_iterator(list);
  ioopm_linked_list_prepend(list,int_elem(233));
  ioopm_linked_list_insert(list,1,int_elem(2));
  ioopm_linked_list_append(list, int_elem(100));
  ioopm_linked_list_append(list, int_elem(200));   // [2]->[233]->[100]->[200]

  //Go through first three links in the list with the iterator next
  ioopm_iterator_reset(iter);
  CU_ASSERT_EQUAL(2, ioopm_iterator_next(iter).i);
  CU_ASSERT_EQUAL(233, ioopm_iterator_next(iter).i);
  CU_ASSERT_EQUAL(100, ioopm_iterator_next(iter).i);
    //Reset the cursor to the first position, ie next should be 22.
  ioopm_iterator_reset(iter);
  CU_ASSERT_EQUAL(2, ioopm_iterator_next(iter).i);
  ioopm_iterator_destroy(iter);
  ioopm_linked_list_destroy(list);
}

void test_reset_current(){
  ioopm_list_t *list = ioopm_linked_list_create(cmp_int);
  ioopm_list_iterator_t *iter = ioopm_list_iterator(list);
  ioopm_linked_list_prepend(list,int_elem(-233));
  ioopm_linked_list_insert(list,1,int_elem(0));
  ioopm_linked_list_append(list, int_elem(100));
  ioopm_linked_list_append(list, int_elem(200));   // [0]->[-233]->[100]->[200]

  ioopm_iterator_next(iter);
  ioopm_iterator_next(iter);
  CU_ASSERT_EQUAL(-233, ioopm_iterator_current(iter).i);
  CU_ASSERT_EQUAL(-233, ioopm_iterator_current(iter).i);
  CU_ASSERT_NOT_EQUAL(233, ioopm_iterator_current(iter).i);
  ioopm_iterator_reset(iter);
  CU_ASSERT_EQUAL(0, ioopm_iterator_current(iter).i); //Current cursor is on dummy
  ioopm_iterator_next(iter);
  CU_ASSERT_EQUAL(0, ioopm_iterator_current(iter).i); //The first link is 0 aswell.
  ioopm_iterator_next(iter);
  ioopm_iterator_next(iter);
  CU_ASSERT_EQUAL(100, ioopm_iterator_current(iter).i);
  ioopm_iterator_destroy(iter);
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
    (CU_add_test(my_test_suite, "Test has next iterator", test_has_next_iter)==NULL)||
    (CU_add_test(my_test_suite, "Test next iterator", test_next_iter)==NULL)||
    (CU_add_test(my_test_suite, "Test reset iterator", test_reset_current)==NULL)||
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
