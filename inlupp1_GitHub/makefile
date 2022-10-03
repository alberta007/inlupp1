
#Makefile test for hashtable
comp: hash_table_tests.c hash_table.c
	gcc hash_table_tests.c hash_table.c -lcunit -o hash_table_tests
	valgrind ./hash_table_tests

clear_comp: hash_table_tests.c hash_table.c
	clear
	gcc hash_table_tests.c hash_table.c -lcunit -o hash_table_tests
	valgrind ./hash_table_tests

list: linked_list.c linked_lists_tests.c
	gcc linked_lists_tests.c linked_list.c -lcunit -o linked_lists_tests
	valgrind ./linked_lists_tests


list_iter: linked_list.c linked_list_tests.c iterator.h
	gcc linked_lists_tests.c linked_list.c -lcunit -o linked_lists_tests
	valgrind ./linked_lists_tests
 # .PHONY: test där test inte är en riktig fil.