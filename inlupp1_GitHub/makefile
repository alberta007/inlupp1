#Makefile test for hashtable
# .PHONY: test där test inte är en riktig fil.
comp: hash_table_tests.c hash_table.c common.c linked_list.c
	gcc hash_table_tests.c common.c linked_list.c hash_table.c -lcunit -o hash_table_tests
	valgrind ./hash_table_tests

clear_comp: hash_table_tests.c hash_table.c common.c linked_list.c
	clear
	gcc hash_table_tests.c linked_list.c common.c hash_table.c -lcunit -o hash_table_tests
	valgrind ./hash_table_tests

list: linked_list.c linked_list_tests.c common.c
	gcc linked_list_tests.c linked_list.c common.c -lcunit -o linked_list_tests
	valgrind ./linked_list_tests

test_small: freq-count.c hash_table.c linked_list.c common.c small.txt
	gcc -pedantic -g freq-count.c hash_table.c linked_list.c common.c -o freq_count
	valgrind ./freq_count small.txt

test1k: freq-count.c hash_table.c linked_list.c common.c 1k-long-words.txt
	gcc -pedantic -g freq-count.c hash_table.c linked_list.c common.c -o freq_count
	valgrind ./freq_count 1k-long-words.txt

test10k: freq-count.c hash_table.c linked_list.c common.c 10k-words.txt
	gcc -pedantic -g freq-count.c hash_table.c linked_list.c common.c -o freq_count
	valgrind ./freq_count 10k-words.txt

test16k: freq-count.c hash_table.c linked_list.c common.c 16k-words.txt
	gcc -pedantic -g freq-count.c hash_table.c linked_list.c common.c -o freq
	valgrind ./freq 16k-words.txt

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

gcov_hash:	hash_table_tests.c hash_table.c linked_list.c common.c
	gcc -Wall -fprofile-arcs -ftest-coverage hash_table_tests.c hash_table.c linked_list.c common.c -lcunit
	./a.out
	gcov hash_table_tests.c

gcov_list:	linked_list_tests.c hash_table.c linked_list.c common.c
	gcc -Wall -fprofile-arcs -ftest-coverage linked_list_tests.c hash_table.c linked_list.c common.c -lcunit
	./a.out
	gcov linked_list_tests.c

gprof_linked_list:
	gcc -pg linked_list_tests.c linked_list.c common.c -lcunit -o linked_list_tests
	./linked_list_tests
	gprof linked_list_tests gmon.out > analysis.txt

gprof1k: freq-count.c 1k-long-words.txt
	gcc -pg -Wall freq-count.c linked_list.c hash_table.c common.c -o freq.exe
	./freq.exe 1k-long-words.txt
	gprof ./freq.exe gmon.out > gprof1k-long-words.txt

gprof10k: freq-count.c 10k-words.txt linked_list.c hash_table.c common.c
	gcc -pg -Wall freq-count.c linked_list.c hash_table.c common.c -o freq.exe
	./freq.exe 10k-words.txt
	gprof ./freq.exe gmon.out > gprof10k-words.txt

gprof16k: freq-count.c 16k-words.txt linked_list.c hash_table.c common.c
	gcc -pg -Wall freq-count.c linked_list.c hash_table.c common.c -o freq.exe
	./freq.exe 16k-words.txt
	gprof ./freq.exe gmon.out > gprof16k-words.txt
