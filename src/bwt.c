# include <stdlib.h>
# include "src.h"
# include "util.h"

/**
* The Burrows–Wheeler transform is an algorithm used in data compression techniques.
* The transformation is reversible, without needing to store any additional data.
*
* args:
*   t: coded input text, must end with $ character.
*   sa: text converted into suffix_array.
*   n: size of both arrays array.
* return:    
*   bwt transformation of input text or null if error occured.
*/
long* burrows_wheeler_transform(long* t, long* sa, long n) {
    if (*(t + n - 1) != code_char('$')) {
        return NULL;
    }

    long* bwt = malloc(sizeof(long) * n);
    for (long i = 0; i < n; i++) {
        *(bwt + i) = *(sa + i) > 0 ? *(t + (*(sa + i) - 1)) : *(t + n - 1); 
    }

    return bwt;
}
