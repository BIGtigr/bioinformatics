# include <assert.h>
# include <stdbool.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>

# include "src.h"
# include "util.h"

struct ch_suite* right_pass(long *text, long text_length) {
    struct ch* coded_text = malloc(sizeof(struct ch) * text_length);
    struct ch_suite* ch_suite = malloc(sizeof(struct ch_suite));

    ch_suite->text = coded_text;
    ch_suite->length = text_length;
    
    coded_text[text_length - 1].ch = code_char('$');
    coded_text[text_length - 1].ct = S;
    
    for (int i = text_length - 2; i >= 0; --i) {
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


/**
 * args:
 *   text: coded text to be put into buckets; must end with '$'/0
 *   text_length: length of text array
 *   alphabet_size: number of different characters/codes in text; *do* account for '$'/0
 *
 *   WARNING: when running init_buckets for initial text, specify alphabet_size = 27,
 *            not the actual number of characters;
 *            when recursing, specify actual number of characters
 * return:
 *   initialized bucket suite
 */
struct bucket_suite* init_buckets(long *text, long text_length, int alphabet_size) {
    long *characters = calloc(alphabet_size, sizeof(long*));

    for (unsigned int i = 0; i < text_length; ++i) {
	++characters[text[i]]; 
    }

    int counter = 0;
    for (int i = 0; i < alphabet_size; ++i) {
	if (characters[i] != 0) {
	    ++counter;
	}
    }

    struct bucket* buckets = malloc(sizeof(struct bucket) * counter);
    int current_bucket = 0;

    for (int i = 0; i < alphabet_size; ++i) {
	if (characters[i] != 0) {
	    struct bucket* b = &buckets[current_bucket];

	    b->character = i;
	    b->indices = malloc(sizeof(long) * characters[i]);
	    b->length = characters[i];
	    b->indices_position = b->length - 1;

	    // initialize b->indices array elements to -1
	    for (int j = 0; j < b->length; ++j) {
		b->indices[j] = -1;
	    }

	    ++current_bucket;
	}
    }

    struct bucket_suite* bucket_suite = malloc(sizeof(bucket_suite));
    bucket_suite->buckets = buckets;
    bucket_suite->length = current_bucket;

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

	if (ch_suite->text[i].ch == code_char('$')) {
	    ss_substrings[ss_index].end = i;
	    ss_substrings[ss_index].id = -1;
	}
    }

    struct sstar_substring_suite* ss = malloc(sizeof(struct sstar_substring_suite));
    ss->substring = ss_substrings;
    ss->length = n;

    return ss;
}

void name_sstar_substrings(long *text, struct sstar_substring_suite* ss_suite) {
    int current_id = 1;

    for (int i = 0; i < ss_suite->length - 1; ++i) {
	struct sstar_substring* ss_i = &ss_suite->substring[i];

	if (ss_i->id != -1) {
	    continue;
	}

	ss_i->id = current_id;
	++current_id;

	long* s1 = text + ss_i->start;
	int length = ss_i->end + 1 - ss_i->start;
	
	for (int j = i + 1; j < ss_suite->length; ++j) {
	    struct sstar_substring* ss_j = &ss_suite->substring[j];

	    if (ss_j->id != -1) {
		// already found identical substring
		continue;
	    }

	    long* s2 = text + ss_j->start;
	    bool arrays_are_identical = true;

	    for (int k = 0; k < length; ++k) {
		if (s1[k] != s2[k]) {
		    arrays_are_identical = false;
		}
	    }

	    if (arrays_are_identical) {
		ss_j->id = ss_i->id;
	    }
	}
    }

    ss_suite->substring[ss_suite->length - 1].id = current_id;
}

static struct bucket*
find_bucket_for_ch(struct bucket_suite* bucket_suite, long c) {
    for (int i = 0; i < bucket_suite->length; ++i) {
	if (bucket_suite->buckets[i].character == c) {
	    return &bucket_suite->buckets[i];
	}
    }

    fprintf(stderr, "WARNING: bucket for character not found\n");

    return NULL;
}

void
buckets_place_sstar(struct ch_suite* ch_suite,
		    struct bucket_suite* bucket_suite) {
    for (int i = ch_suite->length - 1; i >= 0; --i) {
	long c = ch_suite->text[i].ch;
	enum char_type ct = ch_suite->text[i].ct;
	
	if (ct == SSTAR) {
	    struct bucket* bucket = find_bucket_for_ch(bucket_suite, c);

	    if (bucket == NULL) {
		// TODO cleanup
		exit(1);
	    }

	    bucket->indices[bucket->indices_position] = i;
	    --bucket->indices_position;
	}
    }
}

void
induce_l_suffixes(struct ch_suite* ch_suite, struct bucket_suite *bucket_suite) {
    struct ch* text = ch_suite->text;

    // reset buckets' indices_position to 0;
    for (int i = 0; i < bucket_suite->length; ++i) {
	bucket_suite->buckets[i].indices_position = 0;
    }

    for (int i = 0; i < bucket_suite->length; ++i) {
	struct bucket* b = &bucket_suite->buckets[i];

	for (int j = 0; j < b->length; ++j) {
	    // skip uninitialized elements in bucket->indices
	    if (b->indices[j] == -1) {
		continue;
	    }

	    struct ch* current_ch = &text[b->indices[j] - 1];

	    if (current_ch->ct == L) {
		struct bucket* dest_b = find_bucket_for_ch(bucket_suite, current_ch->ch);

		assert(dest_b->indices_position != -1);

		dest_b->indices[dest_b->indices_position] = b->indices[j] - 1;
		++dest_b->indices_position;
	    }
	}
    }
}

void
induce_s_suffixes(struct ch_suite* ch_suite, struct bucket_suite *bucket_suite) {
    struct ch* text = ch_suite->text;

    // reset buckets' indices_position to bucket->length - 1;
    for (int i = 0; i < bucket_suite->length; ++i) {
	bucket_suite->buckets[i].indices_position = bucket_suite->buckets[i].length - 1;
    }

    for (int i = bucket_suite->length - 1; i >= 0; --i) {
	struct bucket* b = &bucket_suite->buckets[i];

	for (int j = b->length - 1; j >= 0; --j) {
	    // there should be no more uninitialized indices
	    assert (b->indices[j] != -1);

	    // there is no need to do anything for index 0; there is no predecessor for
	    // it in the original text
	    if (b->indices[j] == 0) {
		continue;
	    }

	    struct ch* current_ch = &text[b->indices[j] - 1];

	    if (current_ch->ct == S || current_ch->ct == SSTAR) {
		struct bucket* dest_b = find_bucket_for_ch(bucket_suite, current_ch->ch);

		assert(dest_b->indices_position != -1);

		dest_b->indices[dest_b->indices_position] = b->indices[j] - 1;
		--dest_b->indices_position;
	    }
	}
    }
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
