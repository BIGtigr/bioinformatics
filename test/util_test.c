# include <stdio.h>
# include "../src/src.h"
# include "test.h"

void read_file_test() {
    int test_pass = 1;
    
    char* path = "data/util_test_file1.txt";
    char* file_content = read_file(path);
    char* expect_content = "Hello world,\n\nThis is demo test file1.\0";

    if(strcmp(file_content, expect_content) != 0) {
        test_pass = 0;
    }

    printf("util_test.c :: read_file_test(): %d\n", test_pass);
}

void read_big_file_test() {
    int test_pass = 1;
    
    char* path = "data/input_test_file1.txt";
    char* t = read_file(path);
    if(t == NULL) {
        test_pass = 0;
    }
    
    printf("util_test.c :: read_big_file_test(): %d\n", test_pass);
}

void write_file_test() {
    int test_pass = 1;

    char* path = "data/util_test_file2.txt";
    char* file_content;
    char* expect_content = "Hello world\n\tThis is demo #@!@!~ expect_content.";

    write_file(expect_content, path);
    file_content = read_file(path);

    if(strcmp(file_content, expect_content) != 0) {
        test_pass = 0;
    }

    printf("util_test.c :: write_file_test(): %d\n", test_pass);
}
