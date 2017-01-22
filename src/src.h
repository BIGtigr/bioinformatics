#ifndef HEADER_SRC
#define HEADER_SRC

enum char_type { L, S, SSTAR };

struct ch {
    long ch;
    enum char_type ct;
};

struct ch_suite {
  struct ch* text;
  int length;
};

struct bucket {
    long character;

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

struct ch_suite* right_pass(long* text, long text_length);

struct ch_suite* left_pass(struct ch_suite* text);

struct bucket_suite* init_buckets(long *t, long text_length, int alphabet_size);

void buckets_place_sstar(struct ch_suite* ch_suite,
			 struct bucket_suite* bucket_suite);

void name_sstar_substrings(long* text, struct bucket_suite* bucket_suite,
			   struct sstar_substring_suite* ss_suite); 

void induce_l_suffixes(struct ch_suite* ch_suite,
		       struct bucket_suite *bucket_suite);

void induce_s_suffixes(struct ch_suite* ch_suite,
		       struct bucket_suite *bucket_suite);

struct bucket_suite* sais(long* text, long text_length, int alphabet_size);

long* suffix_array(long* t);

char* burrows_wheeler_transform(char* t, int* sa, int n);

#endif
