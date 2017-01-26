# include "../src/src.h"
# include "test.h"

int main()
{
    bwt_no_text_end_symbol_test();

    read_file_test();
    //    read_big_file_test();
    //    write_file_test();

    right_pass_test();
    left_pass_test();
    init_buckets_test();
    find_sstar_substrings_test(); 
    buckets_place_sstar_test();
    name_ss_substrings_test();
    induce_l_suffixes_test();
    induce_s_suffixes_test();

    // suffix_array_from_file_test2();
    sais_norecursion_test();
    sais_recursion_test();
    suffix_array_from_file_test();
    suffix_array_test();

    //    suffix_array_no_text_end_symbol_test();

    burrows_wheeler_transform_test();
    burrows_wheeler_transform_file_test();

    return 0;
}
