# include <stdlib.h>
# include "src.h"

/**
* The Burrows–Wheeler transform is an algorithm used in data compression techniques.
* The transformation is reversible, without needing to store any additional data.
*
* args:
*   t: input text, must end with $ character.
*   sa: text converted into suffix_array.
*   n: size of both arrays array.
* return:    
*   bwt transformation of input text or null if error occured.
*/
char* burrows_wheeler_transform(char* t, int* sa, int n) {
    if (*(t + n - 1) != '$') {
        return NULL;
    }

    int i;
    char* bwt = (char*) malloc(sizeof(char) * n);
    for (i = 0; i < n; i++) {
        *(bwt + i) = *(sa + i) > 0 ? *(t + (*(sa + i) - 1)) : *(t + n - 1); 
    }

    return bwt;
}