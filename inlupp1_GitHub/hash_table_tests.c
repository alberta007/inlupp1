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
   ioopm_hash_table_t *ht = ioopm_hash_table_create(); //Skapa ht
   CU_ASSERT_PTR_NULL(ioopm_hash_table_lookup(ht, 1)); //Kolla om key 1 är NULL
   ioopm_hash_table_insert(ht,1, "hej"); //Lägg till value "hej" i key 1
   CU_ASSERT_PTR_NOT_NULL(ioopm_hash_table_lookup(ht, 1)); //TODO vad key 1 mappar till
   ioopm_hash_table_destroy(ht); //destroy ht
}


int main () {

}
