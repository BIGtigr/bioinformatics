# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include "src.h"

/* does not return len(ch), because len(ch) == strlen(text) */
struct ch_suite* right_pass(char *text) {
    struct ch* coded_text = malloc(sizeof(struct ch) * strlen(text));
    struct ch_suite* ch_suite =
      malloc(sizeof(struct ch_suite));

    ch_suite->text = coded_text;
    ch_suite->length = strlen(text);
    
    coded_text[strlen(text) - 1].ch = '$';
    coded_text[strlen(text) - 1].ct = S;
    
    for (int i = strlen(text) - 2; i >= 0; --i) {
	coded_text[i].ch = text[i];
	char c1 = coded_text[i].ch;
	char c2 = coded_text[i + 1].ch;

	if (c1 < c2 || (c1 == c2 && coded_text[i + 1].ct == S)) {
	    coded_text[i].ct = S;
	} else {
	    coded_text[i].ct = L;
	}
    }

    return ch_suite;
}

struct ch_suite* left_pass(struct ch_suite* ct) {
    for (int i = 1; i < ct->length; ++i) {
	enum char_type ct1 = ct->text[i].ct;
	enum char_type ct2 = ct->text[i - 1].ct;

	if (ct1 == S && ct2 == L) {
	    ct->text[i].ct = SSTAR;
	}
    }

    return ct;
}

struct bucket* init_buckets(char *text) {
    int alphabet_size = 26;
    long *characters = calloc(alphabet_size + 1, sizeof(long*));

    for (unsigned int i = 0; i < strlen(text) - 1; ++i) {
	++characters[text[i] - 'A']; 
    }

    int counter = 0;
    for (int i = 0; i < alphabet_size; ++i) {
	if (characters[i] != 0) {
	    ++counter;
	}
    }

    struct bucket* buckets = malloc(sizeof(struct bucket) * (counter + 1));
    int current_bucket = 0;

    for (int i = 0; i < alphabet_size; ++i) {
	if (characters[i] != 0) {
	    struct bucket* b = &buckets[current_bucket];

	    b->character = i + 'A';
	    b->indices_start = calloc(characters[i], sizeof(long));
	    b->length = characters[i];
	    b->indices_position = b->indices_start + b->length - 1;

	    ++current_bucket;
	}
    }

    struct bucket bucket$ = { '$', NULL, 1, NULL };
    buckets[current_bucket] = bucket$;

    return buckets;
}

/**
* Creates suffix array from given text.
*
* args:
*   t: input text, must end with $ character.
* return:    
*   suffix array representation of given text or null if error occured.
*/

int* suffix_array(char* t) {
    int sa[] = {0};
    return sa;
}
