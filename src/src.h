#ifndef HEADER_SRC
#define HEADER_SRC

char* burrows_wheeler_transform(char* t, int* sa, int n);

int* suffix_array(char* t);

char* read_file(char* file_name);
int write_file(char* file_content, char* file_name);

#endif