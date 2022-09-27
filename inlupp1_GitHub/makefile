#Makefile test for hashtable
comp: hash_table_tests.c hash_table.c
	gcc hash_table_tests.c hash_table.c -lcunit -o hash_table_tests
	valgrind ./hash_table_tests

clear_comp: hash_table_tests.c hash_table.c
	clear
	gcc hash_table_tests.c hash_table.c -lcunit -o hash_table_tests
	valgrind ./hash_table_tests

 # .PHONY: test där test inte är en riktig fil.
