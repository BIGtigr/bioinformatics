# include "../src/src.h"
# include "test.h"

int main()
{
    burrows_wheeler_transform_test();
    bwt_no_text_end_symbol_test();

    right_pass_test();

    /* suffix_array_test(); */
    /* suffix_array_no_text_end_symbol_test(); */
    /* suffix_array_from_file_test(); */

    read_file_test();
    read_big_file_test();
    write_file_test();

    return 0;
}
