#include "hash_table.h"
//#include "linked_list.h"
//#include "iterator.h"
#include "common.h"
#include <stddef.h>
#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <CUnit/Basic.h>



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

static bool cmp_int(elem_t int1, elem_t int2)
{
  return ((int1.i - int2.i) == 0);
}

static bool cmp_str(elem_t str1, elem_t str2)
{
  return (strcmp(str1.str, str2.str) == 0);
}


////////////////////////////////////////////////////////////////////////////////

void test_create_destroy()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, cmp_int);
  CU_ASSERT_PTR_NOT_NULL(ht);
  ioopm_hash_table_destroy(ht);
}

void test_insert_once() {
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, cmp_int); //Skapa ht
  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, int_elem(1)).success); //Kolla om key 1 är NULL
  ioopm_hash_table_insert(ht,int_elem(1), ptr_elem("hej")); //Lägg till value "hej" i key 1
  ioopm_hash_table_insert(ht,int_elem(-18),ptr_elem("test"));
  CU_ASSERT(ioopm_hash_table_lookup(ht, int_elem(1)).success);
  //Looking up negative key:
  CU_ASSERT_EQUAL("test", ioopm_hash_table_lookup(ht, int_elem(-18)).value.str);
  ioopm_hash_table_destroy(ht); //destroy ht
}

void test_lookup_empty(){
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, cmp_int);
  for (int i = 0; i < 18; ++i) /// 18 is a bit magical
  {
    CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, int_elem(i)).success);
    ioopm_hash_table_insert(ht,int_elem(i), ptr_elem("bucket"));
    CU_ASSERT_EQUAL("bucket", ioopm_hash_table_lookup(ht, int_elem(i)).value.str);
  }
  CU_ASSERT(ioopm_hash_table_lookup(ht, int_elem(1)).success); //Kolla om key -1 existerar
  ioopm_hash_table_destroy(ht);
}

void test_remove_exsisting(){
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, cmp_int); //Skapa ht
  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, int_elem(4)).success); //Kolla att 4 är null
  ioopm_hash_table_insert(ht , int_elem(4), str_elem("Abbelito")); //inserta Abbelito på key 4
  ioopm_hash_table_insert(ht , int_elem(-5), str_elem("Ollibobbo")); //inserta Abbelito på key 4
  ioopm_hash_table_remove(ht, int_elem(4)).value;
  ioopm_hash_table_remove(ht, int_elem(-5)).value;
  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, int_elem(4)).success); //Kolla att 4 är null
  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, int_elem(-5)).success); //Kolla att 4 är null
  ioopm_hash_table_destroy(ht);
}

void test_remove_nonexsisting(){
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, cmp_int); //Skapa ht
  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, int_elem(1)).success); //Kolla att 4 är null
  CU_ASSERT_FALSE(ioopm_hash_table_remove(ht, int_elem(-5)).success);
  ioopm_hash_table_destroy(ht);
}

void test_counter_one() {
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, cmp_int); //Skapa ht
  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, int_elem(1)).success); //Kolla om key 1 är NULL
  ioopm_hash_table_insert(ht,int_elem(1),str_elem("Bucket 1")); //Lägg till value "bucket 1" i key 1
  ioopm_hash_table_insert(ht,int_elem(2),str_elem("Bucket 2"));
  ioopm_hash_table_insert(ht,int_elem(3),str_elem("Bucket 3"));
  ioopm_hash_table_insert(ht,int_elem(4),str_elem("Bucket 4"));
  ioopm_hash_table_insert(ht,int_elem(5),str_elem("Bucket 5"));
  ioopm_hash_table_insert(ht,int_elem(6),str_elem("Bucket 6"));
  ioopm_hash_table_insert(ht,int_elem(18),str_elem("Bucket 1"));
  ioopm_hash_table_insert(ht,int_elem(35),str_elem("Bucket 1"));
  ioopm_hash_table_insert(ht,int_elem(52),str_elem("Bucket 1")); //Totalt 4 keys i bucket 1
  ioopm_hash_table_insert(ht,int_elem(-18),str_elem("Bucket 16"));
  ioopm_hash_table_insert(ht,int_elem(-35),str_elem("Bucket 16"));
  ioopm_hash_table_insert(ht,int_elem(-52),str_elem("Bucket 16")); //Totalt 3 keys i bucket 16 (negativa)
  //Totalt: 12 keys (utan dummys)
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 12) //Compares number of keys to 9 (actuall number of keys)
  ioopm_hash_table_destroy(ht); //destroy ht
}

void test_is_hash_table_empty(){
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, cmp_int); //Skapa ht
  CU_ASSERT_TRUE(ioopm_hash_table_is_empty(ht));
  ioopm_hash_table_insert(ht , int_elem(4), str_elem("Abbelito")); //inserta Abbelito på key 4
  CU_ASSERT_FALSE(ioopm_hash_table_is_empty(ht));
  ioopm_hash_table_destroy(ht);
}

void test_clear_hash_table(){
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, cmp_int); //Skapa ht
  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, int_elem(1)).success); //Kolla om key 1 är NULL
  ioopm_hash_table_insert(ht,int_elem(1),str_elem("Bucket 1")); //Lägg till value "bucket 1" i key 1
  ioopm_hash_table_insert(ht,int_elem(2),str_elem("Bucket 2"));
  ioopm_hash_table_insert(ht,int_elem(3),str_elem("Bucket 3"));
  ioopm_hash_table_insert(ht,int_elem(18),str_elem("Bucket 1"));
  ioopm_hash_table_insert(ht,int_elem(35),str_elem("Bucket 1"));
  ioopm_hash_table_insert(ht,int_elem(52),str_elem("Bucket 1")); //Totalt 4 keys i bucket 1
  ioopm_hash_table_clear(ht);
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 0) //Compares number of keys to 9 (actuall number of keys)
  ioopm_hash_table_destroy(ht); //destroy ht
}

void test_hash_table_keys(){
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, cmp_int); //Skapa ht
  int keys[5] = {3, 10, 42, 0, 99};
  bool found[5] = {false};
  ioopm_hash_table_insert(ht,int_elem(keys[0]),str_elem("Abbelito"));
  ioopm_hash_table_insert(ht,int_elem(keys[1]),str_elem("Ollibobbo"));
  ioopm_hash_table_insert(ht,int_elem(keys[2]),str_elem("Faffe"));
  ioopm_hash_table_insert(ht,int_elem(keys[3]),str_elem("Willywonka"));
  ioopm_hash_table_insert(ht,int_elem(keys[4]),str_elem("Kanga"));

  ioopm_list_t *list = ioopm_hash_table_keys(ht);

  for (int i = 0; i>5; i++)
  {
    if(ioopm_linked_list_get(list, i).i != keys[i]) //Finns det värden i ht som inte finns i keys[]
    {
      CU_FAIL("Found a key that was never inserted!");
    }
    else
    {
      found[i] = true;  //Sätt indexeringen till alla keys till true
      //i[found] = true;
    }
  }

  for(int k = 0; k>5; k++)
  {
    CU_ASSERT_TRUE(found[k]==true); //Testet som kollar att alla keys blivit satt till true
  }
  ioopm_linked_list_destroy(list);
  ioopm_hash_table_destroy(ht);
}

void test_hash_table_values()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, cmp_int); //Skapa ht
  char *values[5] = {"Abbelito", "Ollibobbo", "Faffe", "Willywonka", "Kanga"};
  bool found[5] = {false};
  ioopm_hash_table_insert(ht, int_elem(0), str_elem("Abbelito"));
  ioopm_hash_table_insert(ht, int_elem(1), str_elem("Ollibobbo"));
  ioopm_hash_table_insert(ht, int_elem(2), str_elem("Faffe"));
  ioopm_hash_table_insert(ht, int_elem(3), str_elem("Willywonka"));
  ioopm_hash_table_insert(ht, int_elem(4), str_elem("Kanga"));

  ioopm_list_t *list = ioopm_hash_table_values(ht);

  for (int i = 0; i>5; i++)
  {
    for(int k=0; k>0; k++)
    {
      if (values[i] == ioopm_linked_list_get(list, i).str)
      {
        found[k] = true;
        break;
      }
    }
  }
  for(int j = 0; j>5; j++)
  {
    CU_ASSERT_TRUE(found[j]==true); //Testet som kollar att alla keys blivit satt till true
  }
  ioopm_hash_table_destroy(ht);
  ioopm_linked_list_destroy(list);
}

void test_has_key(){
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, cmp_int); //Skapa ht
  ioopm_hash_table_insert(ht, int_elem(3), str_elem("Willywonka"));
  CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, int_elem(18)));
  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, int_elem(3)));
  ioopm_hash_table_destroy(ht);
}


void test_has_value(){
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, cmp_str); //Skapa ht
  ioopm_hash_table_insert(ht, int_elem(3), str_elem("Willywonka"));
  CU_ASSERT_TRUE(ioopm_hash_table_has_value(ht, str_elem("Willywonka"))); //Same string (original or identity)

  // Test empty non-existing value
  CU_ASSERT_FALSE(ioopm_hash_table_has_value(ht, str_elem("")));
  // Test empty existing value
  ioopm_hash_table_insert(ht, int_elem(1), str_elem(""));
  CU_ASSERT_TRUE(ioopm_hash_table_has_value(ht, str_elem("")));
  // Test non-existing value in non-empty ht
  CU_ASSERT_FALSE(ioopm_hash_table_has_value(ht, str_elem("tio")));
  ioopm_hash_table_destroy(ht);
}

/*  TESTET FUNKAR MEN KLAGAR PÅ string_knr_hash
void test_has_value_string_key(){
ioopm_hash_table_t *ht = ioopm_hash_table_create(string_knr_hash, cmp_str); //Skapa ht
ioopm_hash_table_insert(ht, str_elem("ett"), str_elem("Willywonka"));
CU_ASSERT_TRUE(ioopm_hash_table_has_value(ht, str_elem("Willywonka"))); //Same string (original or identity)

// Test empty non-existing value
CU_ASSERT_FALSE(ioopm_hash_table_has_value(ht, str_elem("")));
// Test empty existing value
ioopm_hash_table_insert(ht, str_elem("två"), str_elem("två"));
CU_ASSERT_TRUE(ioopm_hash_table_has_value(ht, str_elem("två")));
// Test non-existing value in non-empty ht
CU_ASSERT_FALSE(ioopm_hash_table_has_value(ht, str_elem("tio")));
ioopm_hash_table_destroy(ht);
}
*/

void test_all_function(){
  bool less_100(elem_t key, elem_t value, void *x)
  {
    return (key.i < 100);
  }
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, cmp_int); //Skapa ht
  // Test with empty ht

  CU_ASSERT(!ioopm_hash_table_all(ht, less_100, NULL));
  // Test with predicate < 10, with only keys smaller than 10.
  ioopm_hash_table_insert(ht, int_elem(0), str_elem("Abbelito"));
  ioopm_hash_table_insert(ht, int_elem(10), str_elem("Ollibobbo"));
  ioopm_hash_table_insert(ht, int_elem(20), str_elem("Faffe"));
  //ioopm_hash_table_insert(ht,-1, ptr_elem("minus ett"));
  CU_ASSERT(ioopm_hash_table_all(ht, less_100, NULL));
  // Test with predicate < 10, with one key larger than 10.
  ioopm_hash_table_insert(ht, int_elem(102), str_elem("Abbelito"));
  CU_ASSERT(!ioopm_hash_table_all(ht, less_100, NULL));
  ioopm_hash_table_destroy(ht);
}

void test_any_function(){
  bool more_100(elem_t key, elem_t value, void *x)
  {
    return (key.i > 100);
  }
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, cmp_int); //Skapa ht
  // Test with empty ht
  CU_ASSERT_FALSE(ioopm_hash_table_any(ht, more_100 , NULL));
  // Test with predicate < 10, with only keys smaller than 10.
  ioopm_hash_table_insert(ht, int_elem(0), str_elem("Abbelito"));
  ioopm_hash_table_insert(ht, int_elem(10), str_elem("Ollibobbo"));
  ioopm_hash_table_insert(ht, int_elem(20), str_elem("Faffe"));
  ioopm_hash_table_insert(ht, int_elem(300), str_elem("Willywonka"));
  ioopm_hash_table_insert(ht, int_elem(200), str_elem("Kanga"));
  ioopm_hash_table_insert(ht, int_elem(170), str_elem("Kanga"));
  CU_ASSERT_TRUE(ioopm_hash_table_any(ht, more_100 , NULL));
  ioopm_hash_table_destroy(ht);
}


static void apply_fun (elem_t key, elem_t *value, void *x)
{
  if (key.i <= 10)
  {
    *value = str_elem("ABBE JOBBAR");
  }
  else{
    *value = str_elem("Olli knegar");
  }
}

void test_apply_to_all(){
  ioopm_hash_table_t *ht = ioopm_hash_table_create(extract_int_hash_key, cmp_str); //Skapa ht
  ioopm_hash_table_insert(ht, int_elem(0), str_elem("Abbelito"));
  ioopm_hash_table_insert(ht, int_elem(1), str_elem("Ollibobbo"));
  ioopm_hash_table_insert(ht, int_elem(2), str_elem("Faffe"));
  ioopm_hash_table_insert(ht, int_elem(3), str_elem("Willywonka"));
  ioopm_hash_table_insert(ht, int_elem(18), str_elem("Kanga"));

  ioopm_hash_table_apply_to_all(ht, apply_fun, NULL);

  // Check that value is changed for valid key
  CU_ASSERT_EQUAL(ioopm_hash_table_lookup(ht, int_elem(1)).value.str, "ABBE JOBBAR");
  // Check that value is unchanged for valid key

  //cmp_str(ioopm_hash_table_lookup(ht, int_elem(18)).value.str, str_elem("ABBE JOBBAR"));
  CU_ASSERT_EQUAL(ioopm_hash_table_lookup(ht, int_elem(18)).value.str, "Olli knegar");

  // Check that value is unchanged for invalid key
  CU_ASSERT_NOT_EQUAL(ioopm_hash_table_lookup(ht, int_elem(19)).value.str, "ABBE JOBBAR");
  ioopm_hash_table_destroy(ht);
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
    (CU_add_test(my_test_suite, "Create and Destroy HT", test_create_destroy)==NULL)||
    (CU_add_test(my_test_suite, "Insert once", test_insert_once)==NULL)||
    (CU_add_test(my_test_suite, "Test lookup", test_lookup_empty)==NULL)||
    (CU_add_test(my_test_suite, "Remove exsisting entry", test_remove_exsisting)==NULL)||
    (CU_add_test(my_test_suite, "Remove nonexsisting entry", test_remove_nonexsisting)==NULL)||
    (CU_add_test(my_test_suite, "Size of the hash_table", test_counter_one)==NULL)||
    (CU_add_test(my_test_suite, "Test hash table empty", test_is_hash_table_empty)==NULL)||
    (CU_add_test(my_test_suite, "Test clear", test_clear_hash_table)==NULL)||
    (CU_add_test(my_test_suite, "Test keys", test_hash_table_keys)==NULL)||
    (CU_add_test(my_test_suite, "Test values", test_hash_table_values)==NULL)||
    (CU_add_test(my_test_suite, "Test has key", test_has_key)==NULL)||
    (CU_add_test(my_test_suite, "Test has value", test_has_value)==NULL)||
    //(CU_add_test(my_test_suite, "Test has value (string key)", test_has_value_string_key)==NULL)||
    (CU_add_test(my_test_suite, "Test the all function", test_all_function)==NULL)||
    (CU_add_test(my_test_suite, "Test the any function", test_any_function)==NULL)||
    (CU_add_test(my_test_suite, "Test the apply function", test_apply_to_all)==NULL)||
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
