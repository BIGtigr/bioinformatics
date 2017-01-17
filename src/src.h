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

    long* indices;
    long length;
    long indices_position;
};

struct bucket_suite {
    struct bucket* buckets;
    int length;
};

struct sstar_substring {
    int start;
    int end;
    long id;
};

struct sstar_substring_suite {
    struct sstar_substring* substring;
    int length;
};

struct sstar_substring_suite* find_sstar_substrings(struct ch_suite* ch_suite);
struct ch_suite* right_pass(char* text);
struct ch_suite* left_pass(struct ch_suite* text);
struct bucket_suite* init_buckets(char *t);
void buckets_place_sstar(struct ch_suite* ch_suite,
			 struct bucket_suite* bucket_suite);
void name_sstar_substrings(char *text, struct sstar_substring_suite* ss_suite); 
void induce_l_suffixes(struct ch_suite* ch_suite, struct bucket_suite *buckets);

int* suffix_array(char* t);

char* burrows_wheeler_transform(char* t, int* sa, int n);

char* read_file(char* file_name);
int write_file(char* file_content, char* file_name);

#endif
