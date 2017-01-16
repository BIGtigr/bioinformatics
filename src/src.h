#include <stdlib.h>

#ifndef HEADER_SRC
#define HEADER_SRC

enum char_type { L, S, SSTAR };

struct ch {
    char ch;
    enum char_type ct;
};

struct ch_suite {
  struct ch* text;
  int length;
};

struct bucket {
    char character;

    long* indices_start;
    int length;
    long* indices_position;
};

struct bucket_suite {
    struct bucket* buckets;
    int length;
};

struct ch_suite* right_pass(char* text);
struct ch_suite* left_pass(struct ch_suite* text);
struct bucket* init_buckets(char *t);
int* suffix_array(char* t);

char* burrows_wheeler_transform(char* t, int* sa, int n);

char* read_file(char* file_name);
int write_file(char* file_content, char* file_name);

#endif
