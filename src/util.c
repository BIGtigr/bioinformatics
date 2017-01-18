# include <stdio.h>
# include <stdlib.h>

# include "src.h"

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


/**
 * Codes character, as follows: '$' -> 0, 'A' -> 1, 'B' -> 2, ...,
 * 'Z' -> 26
 *
 * args:
 *   t: ASCII character to code
 * return:
 *   code of character t
 */
long code_char(char t) {
    long code;

    if (t == '$') {
	code = 0;
    } else if (t >= 'A' && t <= 'Z') {
	code = t - 'A' + 1;
    } else {
	fprintf(stderr, "WARNING: code_char: unsupported character %c\n", t);
    }

    return code;
}


/**
 * Inverse of code_char
 * 
 * args:
 *   code: code to turn into ASCII character
 * return:
 *   ASCII character
 */
char decode_char(long code) {
    char new_char;

    if (code == 0) {
	new_char = '$';
    } else if (code >= 1 && code <= 26) {
	new_char = code + 'A' + -1;
    } else {
	fprintf(stderr, "WARNING: decode_char: unsupported code %ld\n", code);
    }

    return new_char;
}


/*
 * cleanup utility functions
 */
void free_ch_suite(struct ch_suite** cs) {
    free((*cs)->text);
    free(*cs);
}

void free_bucket(struct bucket* b) {
    free(b->indices);
}

void free_bucket_suite(struct bucket_suite** bs) {
    for (int i = 0; i < (*bs)->length; ++i) {
	free_bucket((*bs)->buckets + i);
    }
    free((*bs)->buckets);
    free(*bs);
}

void free_sstar_substring_suite(struct sstar_substring_suite** ss) {
    free((*ss)->substring);
    free(*ss);
}
