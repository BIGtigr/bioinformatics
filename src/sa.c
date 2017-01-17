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

struct bucket_suite* init_buckets(char *text) {
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
    struct bucket bucket$ = { '$', calloc(1, sizeof(long)), 1, 0 };

    buckets[current_bucket] = bucket$;
    ++current_bucket;

    for (int i = 0; i < alphabet_size; ++i) {
	if (characters[i] != 0) {
	    struct bucket* b = &buckets[current_bucket];

	    b->character = i + 'A';
	    b->indices = calloc(characters[i], sizeof(long));
	    b->length = characters[i];
	    b->indices_position = b->length - 1;

	    ++current_bucket;
	}
    }

    struct bucket_suite* bucket_suite = malloc(sizeof(bucket_suite));
    bucket_suite->buckets = buckets;
    bucket_suite->length = current_bucket + 1;

    return bucket_suite;
}

struct sstar_substring_suite*
find_sstar_substrings(struct ch_suite* ch_suite) {
    int n = 0;

    for (int i = 0; i < ch_suite->length; ++i) {
	if (ch_suite->text[i].ct == SSTAR) {
	    ++n;
	}
    }

    int ss_index = -1;
    struct sstar_substring* ss_substrings = malloc(sizeof(struct sstar_substring) * n);

    for (int i = 0; i < ch_suite->length; ++i) {
	if (ch_suite->text[i].ct != SSTAR) {
	    continue;
	} else {
	    ++ss_index;
	}

	if (ss_index == 0) {
	    ss_substrings[ss_index].start = i;
	} else {
	    ss_substrings[ss_index].start = i;
	    ss_substrings[ss_index - 1].end = i;
	    ss_substrings[ss_index - 1].id = -1;
	}

	if (ch_suite->text[i].ch == '$') {
	    ss_substrings[ss_index].end = i;
	    ss_substrings[ss_index].id = -1;
	}
    }

    struct sstar_substring_suite* ss = malloc(sizeof(struct sstar_substring_suite));
    ss->substring = ss_substrings;
    ss->length = n;

    return ss;
}

void name_sstar_substrings(char *text, struct sstar_substring_suite* ss_suite) {
    int current_id = 1;

    for (int i = 0; i < ss_suite->length - 1; ++i) {
	struct sstar_substring* ss_i = &ss_suite->substring[i];

	if (ss_i->id != -1) {
	    continue;
	}

	ss_i->id = current_id;
	++current_id;

	char* s1 = text + ss_i->start;
	int length = ss_i->end + 1 - ss_i->start;
	
	for (int j = i + 1; j < ss_suite->length; ++j) {
	    struct sstar_substring* ss_j = &ss_suite->substring[j];

	    if (ss_j->id != -1) {
		continue;
	    }

	    char* s2 = text + ss_j->start;

	    if (strncmp(s1, s2, length) == 0) {
		ss_j->id = ss_i->id;
	    }
	}
    }

    ss_suite->substring[ss_suite->length - 1].id = current_id;
}

static struct bucket*
find_bucket_for_ch(struct bucket_suite* bucket_suite, char c) {
    for (int i = 0; i < bucket_suite->length; ++i) {
	if (bucket_suite->buckets[i].character == c) {
	    return &bucket_suite->buckets[i];
	}
    }

    return NULL;
}

void
buckets_place_sstar(struct ch_suite* ch_suite,
		    struct bucket_suite* bucket_suite) {
    for (int i = ch_suite->length - 1; i >= 0; --i) {
	char c = ch_suite->text[i].ch;
	enum char_type ct = ch_suite->text[i].ct;
	
	if (ct == SSTAR) {
	    struct bucket* bucket = find_bucket_for_ch(bucket_suite, c);

	    if (bucket == NULL) {
		printf("Bucket for character %c not found.\n", c);
		// TODO cleanup
		exit(1);
	    }

	    bucket->indices[bucket->indices_position] = i;
	    --bucket->indices_position;
	}
    }
}

void
induce_l_suffixes(struct ch_suite* ch_suite, struct bucket_suite *buckets) {
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
