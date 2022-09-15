#include "hash_table.h"
#include <stddef.h>
#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <CUnit/Basic.h>


void test_create_destroy()
{
   ioopm_hash_table_t *ht = ioopm_hash_table_create();
   CU_ASSERT_PTR_NOT_NULL(ht);
   ioopm_hash_table_destroy(ht);
}

void test_insert_once() {
   ioopm_hash_table_t *ht = ioopm_hash_table_create();
   CU_ASSERT_PTR_NULL(ioopm_hash_table_lookup(ht, 1));
   ioopm_hash_table_insert(ht,1, "hej");
   
   CU_ASSERT_PTR_NOT_NULL(ioopm_hash_table_lookup(ht, 1));

   ioopm_hash_table_destroy(ht);
}


int main () {
   
}
