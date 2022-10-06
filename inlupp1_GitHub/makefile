#Makefile test for hashtable --leak-check=full --show-leak-kinds=all
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


freq_comp16k: freq-count.c hash_table.c linked_list.c common.c 16k-words.txt
	gcc -pedantic -g freq-count.c hash_table.c linked_list.c common.c -o freq
	valgrind ./freq 16k-words.txt

freq_comp_small: freq-count.c hash_table.c linked_list.c common.c small.txt
	gcc -pedantic -g freq-count.c hash_table.c linked_list.c common.c -o freq_count
	valgrind ./freq_count small.txt


freq_comp1k: freq-count.c hash_table.c linked_list.c common.c 1k-long-words.txt
	gcc -pedantic -g freq-count.c hash_table.c linked_list.c common.c -o freq_count
	valgrind ./freq_count 1k-long-words.txt

freq_comp10k: freq-count.c hash_table.c linked_list.c common.c 10k-words.txt
	gcc -pedantic -g freq-count.c hash_table.c linked_list.c common.c -o freq_count
	valgrind ./freq_count 10k-words.txt

time16k: freq-count.c hash_table.c linked_list.c common.c 16k-words.txt
	time --verbose ./freq_count 16k-words.txt

time10k: freq-count.c hash_table.c linked_list.c common.c 10k-words.txt
	gcc -pedantic -g freq-count.c hash_table.c linked_list.c common.c -o freq_count
	time --verbose ./freq_count 10k-words.txt

time1k:	freq-count.c hash_table.c linked_list.c common.c 1k-long-words.txt
	gcc -pedantic -g freq-count.c hash_table.c linked_list.c common.c -o freq_count
	time --verbose ./freq_count 1k-long-words.txt

time_small: freq-count.c hash_table.c linked_list.c common.c small.txt
	gcc -pedantic -g freq-count.c hash_table.c linked_list.c common.c -o freq_count
	time --verbose ./freq_count small.txt

gcov:	linked_list_tests.c hash_table.c linked_list.c common.c
	gcc --coverage linked_list_tests.c hash_table.c linked_list.c common.c -lcunit -o list_test
	gcov -abcfu linked_list_tests.c

gprof:	hash_table_tests.c hash_table.c linked_list.c common.c
	gcc -o unittest hash_table_tests.c hash_table.c linked_list.c common.c -lcunit -g -pg -Q

#gcov:	hash_table_tests.c hash_table.c linked_list.c common.c
#	gcc -Wall -fprofile-arcs -ftest-coverage hash_table_tests.c hash_table.c linked_list.c common.c
#	gcov hash_table_tests.c
