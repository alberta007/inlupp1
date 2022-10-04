#Makefile test for hashtable
comp: hash_table_tests.c hash_table.c common.c linked_list.c
	gcc hash_table_tests.c common.c linked_list.c hash_table.c -lcunit -o hash_table_tests
	valgrind ./hash_table_tests

clear_comp: hash_table_tests.c hash_table.c common.c linked_list.c
	clear
	gcc hash_table_tests.c linked_list.c common.c hash_table.c -lcunit -o hash_table_tests
	valgrind ./hash_table_tests

list: linked_list.c linked_list_tests.c common.c
	gcc linked_list_tests.c linked_list.c -lcunit -o linked_list_tests
	valgrind ./linked_list_tests


list_iter: linked_list.c linked_list_tests.c iterator.h common.c
	gcc linked_list_tests.c linked_list.c -lcunit -o linked_list_tests
	valgrind ./linked_list_tests
 # .PHONY: test där test inte är en riktig fil.
