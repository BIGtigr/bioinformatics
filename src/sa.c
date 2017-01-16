# include <stdlib.h>
# include <string.h>
# include "src.h"

/* does not return len(ch), because len(ch) == strlen(text) */
struct ch* right_pass(char *text) {
    struct ch* coded_text = malloc(sizeof(struct ch) * strlen(text));
    
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

    return coded_text;
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
