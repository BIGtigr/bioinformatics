# include <assert.h>
# include <limits.h>
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

void name_sstar_substrings(long *text,
			   struct bucket_suite* bucket_suite,
			   struct sstar_substring_suite* ss_suite) {
    long current_id = 1;

    // first index in SA is always for '$', that's the last in ss_suite
    ss_suite->substring[ss_suite->length - 1].id = current_id;
    ++current_id;

    // iterate over SA
    for (int i = 1; i < bucket_suite->length; ++i) {
	for (int j = 0; j < bucket_suite->buckets[i].length; ++j) {
	    bool ss_found = false;
	    int current_ss;
	    long* s1;
	    long* s2;
	    int start1, end1;
	    int start2, end2;
	    int length1, length2;

	    // for given index from SA, find if there is S* in ss_suite with same starting index
	    for (int k = 0; k < ss_suite->length - 1; ++k) {
		if (ss_suite->substring[k].start == bucket_suite->buckets[i].indices[j]) {
		    printf("Found substring %d\n", k);
		    ss_found = true;
		    start1 = ss_suite->substring[k].start;
		    end1 = ss_suite->substring[k].end;
		    length1 = end1 - start1;

		    s1 = text + start1;
		    current_ss = k;

		    if (ss_suite->substring[current_ss].id == -1) {
			ss_suite->substring[current_ss].id = current_id;
			printf("Named substring %d: %ld\n", k, current_id);
			++current_id;
		    }

		    break;
		}
	    }

	    // If S* substring is found, name it.
	    // Compare with every other S* substring. If other identical is found, assign it
	    // its name, otherwise assign new name
	    if (ss_found) {
		for (int k = 0; k < ss_suite->length - 1; ++k) {
		    if (ss_suite->substring[k].id != -1 || k == current_ss) {
			continue;
		    }

		    start2 = ss_suite->substring[k].start;
		    end2 = ss_suite->substring[k].end;
		    length2 = end2 - start2;

		    int length = length1 < length2 ? length1 : length2;

		    bool arrays_are_identical = true;
		    s2 = text + start2;
		    for (int l = 0; l < length; ++l) {
			if (s1[l] != s2[l]) {
			    arrays_are_identical = false;
			    break;
			}
		    }

		    if (arrays_are_identical) {
			ss_suite->substring[k].id = ss_suite->substring[current_ss].id;
			printf("Named substring %d after %ld: %ld\n", k, current_ss, ss_suite->substring[current_ss].id);
		    }
		    /* else { */
		    /* 	ss_suite->substring[k].id = current_id; */
		    /* 	printf("Named substring %d: %ld\n", k, current_id); */
		    /* 	++current_id; */
		    /* } */

		    /* break; */
		}
	    }
	}
    }
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
 * Induces L, S, S* from given text, and names resulting S* substrings
 *
 * args:
 *   text: input text, must end with $ character
 *   text_length: length of text array
 *   alphabet_size: number of characters in text alphabet, including '$'
 * return:
 *   shortened text T, a result of naming S* substrings. T is terminated
 *   by $ character, so no need to return length of it.
 */
static long* 
induce(long *text, long text_length,
       struct bucket_suite* bucket_suite, struct sstar_substring_suite* ss_suite)
{
    struct ch_suite* ch_suite = left_pass(right_pass(text, text_length));

    buckets_place_sstar(ch_suite, bucket_suite);
    induce_l_suffixes(ch_suite, bucket_suite);
    induce_s_suffixes(ch_suite, bucket_suite);

    struct sstar_substring_suite* new_ss_suite = find_sstar_substrings(ch_suite);
    name_sstar_substrings(text, bucket_suite, new_ss_suite); 

    long *new_text = malloc(sizeof(long) * ss_suite->length);

    // sort new_text by order of appearance of S* in original text
    long smallest_index = LONG_MAX;
    long text_index = 0;
    long current_index = 0;

    for (int i = 0; i < bucket_suite->length; ++i) {
	for (int j = 0; j < bucket_suite->buckets[i].length; ++j) {
	    current_index = bucket_suite->buckets[i].indices[j];

	    if (ch_suite->text[current_index].ct != SSTAR) {
		continue;
	    }

	    if (smallest_index == LONG_MAX &&  // smallest not found yet
		current_index < smallest_index)
	    {
		smallest_index = current_index;
	    } else if (current_index > new_text[i - 1] &&
		       current_index < smallest_index) {
		smallest_index = current_index;
	    }
	}
    }

    // find S* name corresponding to found current_index
    for (int n = 0; n < new_ss_suite->length; ++n) {
	if (new_ss_suite->substring[n].start == smallest_index) {
	    new_text[text_index] = new_ss_suite->substring[n].id;
	    ++text_index;
	}
    }

    new_text[text_index] = code_char('$');

    free_ch_suite(&ch_suite);
    free_sstar_substring_suite(&new_ss_suite);

    return new_text;
}


static bool
characters_are_unique(long* text) {
    // assuming non-unique characters are always adjecent
    int i = 0;

    while(text[i + 1] != code_char('$')) {
	if (text[i] == text[i + 1]) {
	    return false;
	}
    }

    return true;
}

/**
 * Text ends with '$'
 */
static long get_text_length(long* text) {
    int i = 0;

    while (text[i] != code_char('$')) {
	++i;
    }

    return i + 1;
}

/**
 * alphabet characters are from 1 to n, so count is n
 * does not work for original text, of course
 */
static int count_alphabet_characters(long* text) {
    long largest_char = 0;
    int i = 0;

    while(text[i] != code_char('$')) {
	if (text[i] > largest_char) {
	    largest_char = text[i];
	}

	++i;
    }

    return largest_char;
}
    

/**
* Creates suffix array from given text.
*
* args:
*   t: input text, must end with $ character.
* return:    
*   suffix array representation of given text or null if error occured.
*/

int* suffix_array(long* t) {
}

struct bucket_suite*
sais(long* text, long text_length, int alphabet_size) {
    struct bucket_suite* bucket_suite =
	init_buckets(text, text_length, alphabet_size);
    struct sstar_substring_suite* ss_suite = malloc(sizeof(struct sstar_substring_suite));

    long* shortened_text = induce(text, text_length, bucket_suite, ss_suite);

    if (characters_are_unique(shortened_text)) {
	free_sstar_substring_suite(&ss_suite);
	
	return bucket_suite;
    }
    /*
    // generates a copy of text, at most half the size of original,
    // hence O(n*log(n)) space complexity
    long new_text_length = get_text_length(shortened_text);
    long new_text_alphabet = count_alphabet_characters(shortened_text);

    struct bucket_suite* shuffled_bucket_suite =
	sais(shortened_text, new_text_length, new_text_alphabet);

    free(shortened_text);

    // determine new order of S* from new_bucket_suite
    // go into recursion with bucket_suite initialized with S*
    shuffle_bucket(bucket_suite, shuffled_bucket_suite);
	
    reinitialize_bucket_suite(bucket_suite, ss_suite);

    induce(text, text_length, bucket_suite, NULL);

    free_sstar_substring_suite(&ss_suite);
    free_bucket_suite(&bucket_suite);

    return bucket_suite;
    */
    return NULL;
}

