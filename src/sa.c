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
    
    coded_text[text_length - 1].ch = text[text_length - 1];
    coded_text[text_length - 1].ct = S;
    
    for (int i = text_length - 2; i >= 0; --i) {
	coded_text[i].ch = text[i];
	long c1 = coded_text[i].ch;
	long c2 = coded_text[i + 1].ch;

	if (c1 < c2 || (c1 == c2 && coded_text[i + 1].ct == S)) {
	    coded_text[i].ct = S;
	} else {
	    coded_text[i].ct = L;
	}
    }

    return ch_suite;
}

struct ch_suite* left_pass(struct ch_suite* ct) {
    // special case: one character in ct, name it S* automatically
    if (ct->length == 1) {
	ct->text[0].ct = SSTAR;
	return ct;
    }
    
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
    long *characters = calloc(alphabet_size, sizeof(long*) + 1);

    for (unsigned int i = 0; i < text_length; ++i) {
	++characters[text[i]]; 
    }

    int counter = 0;
    for (int i = 0; i < alphabet_size + 1; ++i) {
	if (characters[i] != 0) {
	    ++counter;
	}
    }

    struct bucket* buckets = malloc(sizeof(struct bucket) * counter);
    int current_bucket = 0;

    for (int i = 0; i < alphabet_size + 1; ++i) {
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

void name_sstar_substrings(struct ch_suite* ch_suite,
			   struct bucket_suite* bucket_suite,
			   struct sstar_substring_suite* ss_suite) {
    long current_id = 0;

    // first index in SA is always for '$', that's the last in ss_suite
    ss_suite->substring[ss_suite->length - 1].id = current_id;
    ++current_id;

    // iterate over SA
    for (int i = 0; i < bucket_suite->length; ++i) {
	for (int j = 0; j < bucket_suite->buckets[i].length; ++j) {
	    bool ss_found = false;
	    int current_ss;
	    struct ch *s1, *s2;
	    int start1, end1;
	    int start2, end2;
	    int length1, length2;

	    // for given index from SA, find if there is S* in ss_suite with same starting index
	    for (int k = 0; k < ss_suite->length - 1; ++k) {
		if (ss_suite->substring[k].start == bucket_suite->buckets[i].indices[j]) {
		    ss_found = true;
		    start1 = ss_suite->substring[k].start;
		    end1 = ss_suite->substring[k].end;
		    length1 = end1 - start1;

		    s1 = &ch_suite->text[start1];
		    current_ss = k;

		    if (ss_suite->substring[current_ss].id == -1) {
			ss_suite->substring[current_ss].id = current_id;
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

		    int length = length1 < length2 ? length2 : length1;

		    bool arrays_are_identical = true;
		    s2 = &ch_suite->text[start2];
		    for (int l = 0; l <= length; ++l) {
			if (s1[l].ch != s2[l].ch || s1[l].ct != s2[l].ct) {
			    arrays_are_identical = false;
			    break;
			}
		    }

		    if (arrays_are_identical) {
			ss_suite->substring[k].id = ss_suite->substring[current_ss].id;
		    }
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
    for (int i = 0; i < ch_suite->length; ++i) {
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
    /*
    for (int i = 0; i < ch_suite->length; ++i) {
	printf("[%ld %d] ", ch_suite->text[i].ch, ch_suite->text[i].ct);
    }
    printf("\n");

    for (int i = 0; i < bucket_suite->length; ++i) {
    	printf("Bucket for %ld:\n", (bucket_suite->buckets[i].character));
    	for (int j = 0; j < bucket_suite->buckets[i].length; ++j) {
    	    printf(" %ld\n", bucket_suite->buckets[i].indices[j]);
    	}
    }
    */

    for (int i = bucket_suite->length - 1; i >= 0; --i) {
	struct bucket* b = &bucket_suite->buckets[i];

	for (int j = b->length - 1; j >= 0; --j) {
	    /*
	    if (b->indices[j] == -1) {
		printf("invalid: %ld, %ld\n", b->character, b->indices_position);
		for (int n = 0; n < b->length; ++n) {
		    printf("%ld\n", b->indices[n]);
		}
	    }
	    */ 
	    if (b->indices[j] == -1) {
		continue;
	    }

	    //	    assert(b->indices[j] != -1);
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
generate_shortened_text(struct ch_suite* ch_suite, struct bucket_suite* bucket_suite,
       struct sstar_substring_suite* ss_suite)
{
    long* new_text = malloc(sizeof(long) * ch_suite->length);

    // sort new_text by order of appearance of S* in original text
    long text_index = 0;
    long current_index = 0;
    long last_filled_index = 0;

    for (int k = 0; k < ss_suite->length; ++k) {
	long smallest_index = LONG_MAX;

	for (int i = 0; i < bucket_suite->length; ++i) {
	    for (int j = 0; j < bucket_suite->buckets[i].length; ++j) {
		current_index = bucket_suite->buckets[i].indices[j];

		if (ch_suite->text[current_index].ct != SSTAR) {
		    continue;
		}

		if (text_index == 0 && current_index < smallest_index) {
		    smallest_index = current_index;
		} else if (current_index > last_filled_index &&
			   current_index < smallest_index) {
		    smallest_index = current_index;
		}

	    }
	}

	// find S* name corresponding to found current_index
	for (int n = 0; n < ss_suite->length; ++n) {
	    if (ss_suite->substring[n].start == smallest_index) {
		new_text[text_index] = ss_suite->substring[n].id;
		++text_index;
	    }
	}

	last_filled_index = smallest_index;
    }

    return new_text;
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

static bool
characters_are_unique(long* text, long length) {
    long *characters = calloc(length, sizeof(long));

    for (int i = 0; i < length; ++i) {
	++characters[text[i]];
    }

    for (int i = 0; i < length; ++i) {
	if (characters[i] != 1) {
	    free(characters);
	    return false;
	}
    }

    free(characters);
    return true;
}


/**
 *
 */
static int count_alphabet_characters(long* text, long length) {
    long largest_character = 0;

    for (int i = 0; i < length; ++i) {
	if (text[i] > largest_character) {
	    largest_character = text[i];
	}
    }

    long *characters = calloc(largest_character + 1, sizeof(long));
    long count = 0;

    for (int i = 0; i < length; ++i) {
	++characters[text[i]];
    }

    for (int i = 0; i <= largest_character; ++i) {
	if (characters[i] > 0) {
	    ++count;
	}
    }

    free(characters);
    return count;
}

static void buckets_clean_buckets(struct bucket_suite* bs) {
    for (int i = 0; i < bs->length; ++i) {
	for (int j = 0; j < bs->buckets[i].length; ++j) {
	    bs->buckets[i].indices[j] = -1;
	}

	bs->buckets[i].indices_position = bs->buckets[i].length - 1;
    }
}

static void buckets_shuffle_sstar(long* text,
				  struct bucket_suite* bs_shuffled,
				  struct bucket_suite* bs,
				  struct sstar_substring_suite* ss_suite) {
    for (int i = bs_shuffled->length - 1; i >= 0; --i) {
	for (int j = bs_shuffled->buckets[i].length - 1; j >= 0; --j) {
	    long ss_index = bs_shuffled->buckets[i].indices[j];
	    long index = ss_suite->substring[ss_index].start;

	    struct bucket* bucket = find_bucket_for_ch(bs, text[index]);

	    bucket->indices[bucket->indices_position] = index;
	    --bucket->indices_position;
	}
    }

    /*
    for (int i = 0; i < bs->length; ++i) {
    	printf("Bucket for %c:\n", decode_char(bs->buckets[i].character));
    	for (int j = 0; j < bs->buckets[i].length; ++j) {
    	    printf(" %ld\n", bs->buckets[i].indices[j]);
    	}
    }
    */
}
    

/**
* Creates suffix array from given text.
*
* args:
*   t: input text, must end with $ character.
* return:    
*   suffix array representation of given text or null if error occured.
*/

long* suffix_array(long* t) {
    long text_length = get_text_length(t);

    struct bucket_suite* bs = sais(t, text_length, 27);
    long* sa = malloc(sizeof(long) * text_length);

    int n = 0;
    for (int i = 0; i < bs->length; ++i) {
	for (int j = 0; j < bs->buckets[i].length; ++j) {
	    sa[n++] = bs->buckets[i].indices[j];
	}
    }

    free_bucket_suite(&bs);

    return sa;
}

struct bucket_suite*
sais(long* text, long text_length, int alphabet_size) {
    struct ch_suite* ch_suite = left_pass(right_pass(text, text_length));
    struct bucket_suite* bucket_suite =
	init_buckets(text, text_length, alphabet_size);
    struct sstar_substring_suite* ss_suite = find_sstar_substrings(ch_suite);

    buckets_place_sstar(ch_suite, bucket_suite);
    induce_l_suffixes(ch_suite, bucket_suite);
    induce_s_suffixes(ch_suite, bucket_suite);
    name_sstar_substrings(ch_suite, bucket_suite, ss_suite); 

    long* shortened_text =
	generate_shortened_text(ch_suite, bucket_suite, ss_suite);

    if (characters_are_unique(shortened_text, ss_suite->length)) {
	// directly compute from shortened text; no recursion
	long new_text_length = ss_suite->length;
	long new_text_alphabet = count_alphabet_characters(shortened_text, new_text_length);

	struct ch_suite* ch_suite_new =
	    left_pass(right_pass(shortened_text, new_text_length));
	struct bucket_suite* bucket_suite_new =
	    init_buckets(shortened_text, new_text_length, new_text_alphabet);
	struct sstar_substring_suite* ss_suite_new =
	    find_sstar_substrings(ch_suite_new);

	buckets_place_sstar(ch_suite_new, bucket_suite_new);
	induce_l_suffixes(ch_suite_new, bucket_suite_new);
	induce_s_suffixes(ch_suite_new, bucket_suite_new);
	name_sstar_substrings(ch_suite_new, bucket_suite_new, ss_suite_new); 

	buckets_clean_buckets(bucket_suite);
	buckets_shuffle_sstar(text, bucket_suite_new,
			      bucket_suite, ss_suite);

	induce_l_suffixes(ch_suite, bucket_suite);
	induce_s_suffixes(ch_suite, bucket_suite);

	free(shortened_text);
	free_ch_suite(&ch_suite);
	free_sstar_substring_suite(&ss_suite);
	free_ch_suite(&ch_suite_new);
	free_bucket_suite(&bucket_suite_new);
	free_sstar_substring_suite(&ss_suite_new);

	return bucket_suite;
    }
    printf("Recursing\n");

    long new_text_length = ss_suite->length;
    long new_text_alphabet = count_alphabet_characters(shortened_text, new_text_length);

    struct bucket_suite* shuffled_bucket_suite =
	sais(shortened_text, new_text_length, new_text_alphabet);

    buckets_clean_buckets(bucket_suite);
    buckets_shuffle_sstar(text, shuffled_bucket_suite, bucket_suite, ss_suite);

    induce_l_suffixes(ch_suite, bucket_suite);
    induce_s_suffixes(ch_suite, bucket_suite);

    free(shortened_text);
    free_ch_suite(&ch_suite);
    //free_bucket_suite(&shuffled_bucket_suite);
    free_sstar_substring_suite(&ss_suite);

    return bucket_suite;
}

