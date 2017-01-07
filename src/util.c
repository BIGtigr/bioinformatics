# include <malloc.h>
# include <string.h>
# include <stdio.h>

#include "src.h"

/**
* Reads file content.
*
* args:
*   file_name: path to file.
* return:    
*   content of file in char* representation or null if error occured.
*/
char* read_file(char* file_name) {
    FILE *f = fopen(file_name, "rb");
    fseek(f, 0, SEEK_END);
    long n = ftell(f);
    fseek(f, 0, SEEK_SET);  //same as rewind(f);

    char *t = malloc(n + 1);
    fread(t, n, 1, f);
    fclose(f);

    *(t + n) = '\0';
    return t;
}

/**
* Writes content to file.
* Replaces existing content in file.
*
* args:
*   file_content: content to be written in file.
*   file_name: path to file.
* return:    
*   Value 1 if writting to file had been done successfully, 0 otherwise.
*/
int write_file(char* file_content, char* file_name) {
    FILE* f = fopen(file_name, "w");
    if(!f) return 0;
    fprintf(f, "%s", file_content);
    fclose(f);
    return 1;
}