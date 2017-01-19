#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "src.h"

char *text = read_file();

induce(char *R) { // is it really char* here? ch_suite more likely
    right pass;
    left pass;
    buckets_place_sstar;
    induce_l_suffixes;
    induce_s_suffixes;
}

B bwt_is(text) {
    // list of S* substrings
    R[] = find_sstar_substrings(left_pass(right_pass(text)));

    names of *S substrings = induce(R) // for each element of list, or whole list at once?

    if (unique(shortened_text)) { // alphabet of shortened text does not contain repeating characters
	B1 = directly_compute_from_shortened_text();
    } else {
	B1 = bwt_is(shortened_text); // text, text_length, alphabet (number of different characters);
    }

    Decode S* strings B1 into R; // using B1, determine correct order of S* substrings (comment in paper, page 3)
    // result in R is list of S* substrings?
	
    B = induce(R);

    return B;
}
