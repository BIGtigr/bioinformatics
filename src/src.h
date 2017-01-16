#ifndef HEADER_SRC
#define HEADER_SRC

enum char_type { L, S, SSTAR };

struct sl_classified_text {
  struct ch* first;
  int length;
};

struct ch {
    char ch;
    enum char_type ct;
};

struct sl_classified_text* right_pass(char* text);
char* burrows_wheeler_transform(char* t, int* sa, int n);

int* suffix_array(char* t);

char* read_file(char* file_name);
int write_file(char* file_content, char* file_name);

#endif
