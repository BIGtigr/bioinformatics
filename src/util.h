# include "src.h"

# ifndef HEADER_UTIL
# define HEADER_UTIL

long* read_file(char* file_name);
int write_file(long* file_content, long length, char* file_name);

long code_char(char t);
char decode_char(long code);

void free_ch_suite(struct ch_suite** ch);
void free_bucket(struct bucket* b);
void free_bucket_suite(struct bucket_suite** bs);
void free_sstar_substring_suite(struct sstar_substring_suite** ss);
