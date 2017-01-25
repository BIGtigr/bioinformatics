# include <assert.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include "../src/src.h"
# include "../src/util.h"
# include "test.h"

void right_pass_test() {
    int test_pass = 1;

    long text[15] = {
	code_char('B'),
	code_char('I'),
	code_char('O'),
	code_char('I'),
	code_char('N'),
	code_char('F'),
	code_char('O'),
	code_char('R'),
	code_char('M'),
	code_char('A'),
	code_char('T'),
	code_char('I'),
	code_char('K'),
	code_char('A'),
	code_char('$')
    };

    struct ch coded_text_valid[15] = {
	{ code_char('B'), S },
	{ code_char('I'), S },
	{ code_char('O'), L },
	{ code_char('I'), S },
	{ code_char('N'), L },
	{ code_char('F'), S },
	{ code_char('O'), S },
	{ code_char('R'), L },
	{ code_char('M'), L },
	{ code_char('A'), S },
	{ code_char('T'), L },
	{ code_char('I'), S },
	{ code_char('K'), L },
	{ code_char('A'), L },
	{ code_char('$'), S }
    };

    struct ch_suite* output = right_pass(text, 15);
    struct ch* coded_text = output->text;

    for (int i = 0; i < 15; ++i) {
	if (coded_text_valid[i].ct != coded_text[i].ct) {
	    printf("%c: expected %d, got %d\n",
		   decode_char(coded_text_valid[i].ch),
		   coded_text_valid[i].ct,
		   coded_text[i].ct);
	    test_pass = 0;
	}
	if (coded_text_valid[i].ch != coded_text[i].ch) {
	    printf("char mismatch: expected %ld, got %ld\n",
		   coded_text_valid[i].ch,
		   coded_text[i].ch);
	    test_pass = 0;
	}
    }

    printf("sa_test.c :: right_pass_test(): %d\n",
	   test_pass);

    free_ch_suite(&output);
}

void left_pass_test() {
    int test_pass = 1;

    long text[15] = {
	code_char('B'),
	code_char('I'),
	code_char('O'),
	code_char('I'),
	code_char('N'),
	code_char('F'),
	code_char('O'),
	code_char('R'),
	code_char('M'),
	code_char('A'),
	code_char('T'),
	code_char('I'),
	code_char('K'),
	code_char('A'),
	code_char('$')
    };

    struct ch coded_text_valid[15] = {
	{ code_char('B'), S },
	{ code_char('I'), S },
	{ code_char('O'), L },
	{ code_char('I'), SSTAR },
	{ code_char('N'), L },
	{ code_char('F'), SSTAR },
	{ code_char('O'), S },
	{ code_char('R'), L },
	{ code_char('M'), L },
	{ code_char('A'), SSTAR },
	{ code_char('T'), L },
	{ code_char('I'), SSTAR },
	{ code_char('K'), L },
	{ code_char('A'), L },
	{ code_char('$'), SSTAR }
    };

    struct ch_suite* output = right_pass(text, 15);
    output = left_pass(output);
    struct ch* coded_text = output->text;

    for (int i = 0; i < 15; ++i) {
	if (coded_text_valid[i].ct != coded_text[i].ct ||
	    coded_text_valid[i].ch != coded_text[i].ch) {
	    test_pass = 0;
	}
    }

    printf("sa_test.c :: left_pass_test(): %d\n",
	   test_pass);

    free_ch_suite(&output);
}

void init_buckets_test() {
    int test_pass = 1;

    long text[15] = {
	code_char('B'),
	code_char('I'),
	code_char('O'),
	code_char('I'),
	code_char('N'),
	code_char('F'),
	code_char('O'),
	code_char('R'),
	code_char('M'),
	code_char('A'),
	code_char('T'),
	code_char('I'),
	code_char('K'),
	code_char('A'),
	code_char('$')
    };

    struct bucket_suite* bs = init_buckets(text, 15, 27);
    struct bucket* buckets = bs->buckets;

    long b$_indices[] = { -1 };
    long bA_indices[] = { -1, -1 };
    long bB_indices[] = { -1 };
    long bF_indices[] = { -1 };
    long bI_indices[] = { -1, -1, -1 };
    long bK_indices[] = { -1 };
    long bM_indices[] = { -1 };
    long bN_indices[] = { -1 };
    long bO_indices[] = { -1, -1 };
    long bR_indices[] = { -1 };
    long bT_indices[] = { -1 };

    struct bucket buckets_valid[11] = {
	{ code_char('$'), b$_indices, 1, 0 },
	{ code_char('A'), bA_indices, 2, 1 },
	{ code_char('B'), bB_indices, 1, 0 },
	{ code_char('F'), bF_indices, 1, 0 },
	{ code_char('I'), bI_indices, 3, 2 },
	{ code_char('K'), bK_indices, 1, 0 },
	{ code_char('M'), bM_indices, 1, 0 },
	{ code_char('N'), bN_indices, 1, 0 },
	{ code_char('O'), bO_indices, 2, 1 },
	{ code_char('R'), bR_indices, 1, 0 },
	{ code_char('T'), bT_indices, 1, 0 }
    };

    for (int i = 0; i < 11; ++i) {
	if (buckets[i].character != buckets_valid[i].character ||
	    buckets[i].length != buckets_valid[i].length ||
	    buckets[i].indices_position != buckets_valid[i].indices_position) {
	    printf("%ld, %ld, %ld\n",
		   buckets[i].character, buckets[i].length, buckets[i].indices_position);
	    test_pass = 0;
	}

	for (int j = 0; j < buckets[i].length; ++j) {
	    if (buckets[i].indices[j] != buckets_valid[i].indices[j]) {
		printf("%c %ld %ld\n", decode_char(buckets[i].character),
		       buckets[i].indices[j], buckets_valid[i].indices[j]);
		test_pass = 0;
	    }
	}
    }

    printf("sa_test.c :: init_bucket_test(): %d\n",
	   test_pass);

    free_bucket_suite(&bs);
}

void find_sstar_substrings_test() {
    int test_pass = 1;

    long text[15] = {
	code_char('B'),
	code_char('I'),
	code_char('O'),
	code_char('I'),
	code_char('N'),
	code_char('F'),
	code_char('O'),
	code_char('R'),
	code_char('M'),
	code_char('A'),
	code_char('T'),
	code_char('I'),
	code_char('K'),
	code_char('A'),
	code_char('$')
    };

    struct ch_suite* ch_suite = left_pass(right_pass(text, 15));
    struct sstar_substring_suite* ss_suite = find_sstar_substrings(ch_suite);

    struct sstar_substring ss_substring[5] = {
	{ 3, 5, -1 },
	{ 5, 9, -1 },
	{ 9, 11, -1 },
	{ 11, 14, -1 },
	{ 14, 14, -1 }
    };

    struct sstar_substring_suite* ss_suite_valid =
	malloc(sizeof(struct sstar_substring_suite));

    ss_suite_valid->substring = ss_substring;
    ss_suite_valid->length = 5;

    for (int i = 0; i < 5; ++i) {
	int start = ss_suite->substring[i].start;
	int end = ss_suite->substring[i].end;
	int valid_start = ss_suite_valid->substring[i].start;
	int valid_end = ss_suite_valid->substring[i].end;

	if (start != valid_start || end != valid_end) {
	    test_pass = 0;
	}
    }

    printf("sa_test.c :: find_sstar_substrings_test(): %d\n",
	   test_pass);

    free_ch_suite(&ch_suite);
    free_sstar_substring_suite(&ss_suite);
    free(ss_suite_valid);
}

void name_ss_substrings_test() {
    int test_pass = 1;

    long text[15] = {
	code_char('M'),
	code_char('M'),
	code_char('I'),
	code_char('S'),
	code_char('S'),
	code_char('I'),
	code_char('S'),
	code_char('S'),
	code_char('I'),
	code_char('I'),
	code_char('P'),
	code_char('P'),
	code_char('I'),
	code_char('I'),
	code_char('$')
    };

    struct sstar_substring ss_substring[4] = {
	{ 2, 5, 2 },
	{ 5, 8, 2 },
	{ 8, 14, 1 },
	{ 14, 14, 0 }
    };

    struct sstar_substring_suite* ss_suite_valid =
	malloc(sizeof(struct sstar_substring_suite));

    ss_suite_valid->substring = ss_substring;
    ss_suite_valid->length = 4;

    struct ch_suite* ch_suite = left_pass(right_pass(text, 15));
    struct bucket_suite* bucket_suite = init_buckets(text, 15, 27);

    buckets_place_sstar(ch_suite, bucket_suite);
    induce_l_suffixes(ch_suite, bucket_suite);
    induce_s_suffixes(ch_suite, bucket_suite);

    struct sstar_substring_suite* ss_suite =
	find_sstar_substrings(left_pass(right_pass(text, 15)));
    
    name_sstar_substrings(ch_suite, bucket_suite, ss_suite); 

    for (int i = 0; i < 4; ++i) {
	struct sstar_substring s1 = ss_suite_valid->substring[i];
	struct sstar_substring s2 = ss_suite->substring[i];

	if (s1.id != s2.id || s1.start != s2.start ||
	    s1.end != s2.end) {
	    printf("%ld, %ld %d %d\n", s1.id, s2.id, s2.start, s2.end);
	    test_pass = 0;
	}
    }

    printf("sa_test.c :: name_ss_substrings_test(): %d\n",
	   test_pass);

    free_ch_suite(&ch_suite);
    free_bucket_suite(&bucket_suite);
    free_sstar_substring_suite(&ss_suite);
    free(ss_suite_valid);
}

void buckets_place_sstar_test() {
    int test_pass = 1;

    long text[15] = {
	code_char('B'),
	code_char('I'),
	code_char('O'),
	code_char('I'),
	code_char('N'),
	code_char('F'),
	code_char('O'),
	code_char('R'),
	code_char('M'),
	code_char('A'),
	code_char('T'),
	code_char('I'),
	code_char('K'),
	code_char('A'),
	code_char('$')
    };

    struct bucket_suite* bucket_suite = init_buckets(text, 15, 27);
    struct bucket* buckets = bucket_suite->buckets;
    struct ch_suite* ch_suite = left_pass(right_pass(text, 15));

    buckets_place_sstar(ch_suite, bucket_suite);

    long indices$[] = { 14 };
    long indicesA[] = { -1, 9 };
    long indicesB[] = { -1 };
    long indicesF[] = { 5 };
    long indicesI[] = { -1, 11, 3 };
    long indicesK[] = { -1 };
    long indicesM[] = { -1 };
    long indicesN[] = { -1 };
    long indicesO[] = { -1, -1 };
    long indicesR[] = { -1 };
    long indicesT[] = { -1 };

    struct bucket buckets_valid[11] = {
	{ code_char('$'), indices$, 1, -1 },
	{ code_char('A'), indicesA, 2, 0 },
	{ code_char('B'), indicesB, 1, 0 },
	{ code_char('F'), indicesF, 1, -1 },
	{ code_char('I'), indicesI, 3, 0 },
	{ code_char('K'), indicesK, 1, 0 },
	{ code_char('M'), indicesM, 1, 0 },
	{ code_char('N'), indicesN, 1, 0 },
	{ code_char('O'), indicesO, 2, 1 },
	{ code_char('R'), indicesR, 1, 0 },
	{ code_char('T'), indicesT, 1, 0 },
    };

    for (int i = 0; i < 11; ++i) {
	struct bucket b1 = buckets[i];
	struct bucket b2 = buckets_valid[i];

	// check single fields of bucket
	if (b1.character != b2.character || b1.length != b2.length ||
	    b1.indices_position != b2.indices_position) {
	    printf("%c, %ld, %ld\n",
		   decode_char(b1.character), b1.length, b1.indices_position);
	    test_pass = 0;
	}

	long* b1_indices = b1.indices;
	long* b2_indices = b2.indices;

	// check bucket.indices array
	for (int j = 0; j < b1.length; ++j) {
	    if (b1_indices[j] != b2_indices[j]) {
		printf("%c: got %ld, expected %ld\n",
		       decode_char(b1.character), b1_indices[j], b2_indices[j]);
		test_pass = 0;
	    }
	}
	   
    }

    printf("sa_test.c :: buckets_place_sstar_test(): %d\n",
	   test_pass);

    free_bucket_suite(&bucket_suite);
    free_ch_suite(&ch_suite);
}

void induce_l_suffixes_test() {
    int test_pass = 1;

    long text[15] = {
	code_char('B'),
	code_char('I'),
	code_char('O'),
	code_char('I'),
	code_char('N'),
	code_char('F'),
	code_char('O'),
	code_char('R'),
	code_char('M'),
	code_char('A'),
	code_char('T'),
	code_char('I'),
	code_char('K'),
	code_char('A'),
	code_char('$')
    };

    struct bucket_suite* bucket_suite = init_buckets(text, 15, 27);
    struct bucket* buckets = bucket_suite->buckets;
    struct ch_suite* ch_suite = left_pass(right_pass(text, 15));

    buckets_place_sstar(ch_suite, bucket_suite);
    induce_l_suffixes(ch_suite, bucket_suite);

    long indices$[] = { 14 };
    long indicesA[] = { 13, 9 };
    long indicesB[] = { -1 };
    long indicesF[] = { 5 };
    long indicesI[] = { -1, 11, 3 };
    long indicesK[] = { 12 };
    long indicesM[] = { 8 };
    long indicesN[] = { 4 };
    long indicesO[] = { 2, -1 };
    long indicesR[] = { 7 };
    long indicesT[] = { 10 };

    struct bucket buckets_valid[11] = {
	{ code_char('$'), indices$, 1, 0 },
	{ code_char('A'), indicesA, 2, 1 },
	{ code_char('B'), indicesB, 1, 0 },
	{ code_char('F'), indicesF, 1, 0 },
	{ code_char('I'), indicesI, 3, 0 },
	{ code_char('K'), indicesK, 1, 1 },
	{ code_char('M'), indicesM, 1, 1 },
	{ code_char('N'), indicesN, 1, 1 },
	{ code_char('O'), indicesO, 2, 1 },
	{ code_char('R'), indicesR, 1, 1 },
	{ code_char('T'), indicesT, 1, 1 },
    };

    for (int i = 0; i < 11; ++i) {
	struct bucket b1 = buckets[i];
	struct bucket b2 = buckets_valid[i];

	// check single fields of bucket
	if (b1.character != b2.character || b1.length != b2.length ||
	    b1.indices_position != b2.indices_position) {
	    printf("%c: expected %ld, got %ld",
		   decode_char(b2.character), b2.indices_position, b1.indices_position);
	    test_pass = 0;
	}

	long* b1_indices = b1.indices;
	long* b2_indices = b2.indices;

	// check bucket.indices array
	for (int j = 0; j < b1.length; ++j) {
	    if (b1_indices[j] != b2_indices[j]) {
		printf("%c: got %ld, expected %ld\n",
		       decode_char(b1.character), b1_indices[j], b2_indices[j]);
		test_pass = 0;
	    }
	}
	   
    }

    printf("sa_test.c :: induce_l_suffixes_test(): %d\n",
	   test_pass);

    free_bucket_suite(&bucket_suite);
    free_ch_suite(&ch_suite);
}

// identical as induce_s_suffixes_test, except expected indices arrays are different
void induce_s_suffixes_test() {
    int test_pass = 1;

    long text[15] = {
	code_char('B'),
	code_char('I'),
	code_char('O'),
	code_char('I'),
	code_char('N'),
	code_char('F'),
	code_char('O'),
	code_char('R'),
	code_char('M'),
	code_char('A'),
	code_char('T'),
	code_char('I'),
	code_char('K'),
	code_char('A'),
	code_char('$')
    };

    struct bucket_suite* bucket_suite = init_buckets(text, 15, 27);
    struct bucket* buckets = bucket_suite->buckets;
    struct ch_suite* ch_suite = left_pass(right_pass(text, 15));

    buckets_place_sstar(ch_suite, bucket_suite);
    induce_l_suffixes(ch_suite, bucket_suite);
    induce_s_suffixes(ch_suite, bucket_suite);

    long indices$[] = { 14 };
    long indicesA[] = { 13, 9 };
    long indicesB[] = { 0 };
    long indicesF[] = { 5 };
    long indicesI[] = { 11, 3, 1 };
    long indicesK[] = { 12 };
    long indicesM[] = { 8 };
    long indicesN[] = { 4 };
    long indicesO[] = { 2, 6 };
    long indicesR[] = { 7 };
    long indicesT[] = { 10 };

    struct bucket buckets_valid[11] = {
	{ code_char('$'), indices$, 1, 0 },
	{ code_char('A'), indicesA, 2, 0 },
	{ code_char('B'), indicesB, 1, -1 },
	{ code_char('F'), indicesF, 1, -1 },
	{ code_char('I'), indicesI, 3, -1 },
	{ code_char('K'), indicesK, 1, 0 },
	{ code_char('M'), indicesM, 1, 0 },
	{ code_char('N'), indicesN, 1, 0 },
	{ code_char('O'), indicesO, 2, 0 },
	{ code_char('R'), indicesR, 1, 0 },
	{ code_char('T'), indicesT, 1, 0 },
    };

    for (int i = 0; i < 11; ++i) {
	struct bucket b1 = buckets[i];
	struct bucket b2 = buckets_valid[i];

	// check single fields of bucket
	if (b1.character != b2.character || b1.length != b2.length ||
	    b1.indices_position != b2.indices_position) {
	    printf("%c: expected %ld, got %ld",
		   decode_char(b2.character), b2.indices_position, b1.indices_position);
	    test_pass = 0;
	}

	long* b1_indices = b1.indices;
	long* b2_indices = b2.indices;

	// check bucket.indices array
	for (int j = 0; j < b1.length; ++j) {
	    if (b1_indices[j] != b2_indices[j]) {
		printf("%c: got %ld, expected %ld\n",
		       decode_char(b1.character), b1_indices[j], b2_indices[j]);
		test_pass = 0;
	    }
	}
	   
    }

    printf("sa_test.c :: induce_s_suffixes_test(): %d\n",
	   test_pass);

    free_bucket_suite(&bucket_suite);
    free_ch_suite(&ch_suite);
}


void suffix_array_test() {
    int test_pass = 1;

    long t[15] = {
	code_char('B'),
	code_char('I'),
	code_char('O'),
	code_char('I'),
	code_char('N'),
	code_char('F'),
	code_char('O'),
	code_char('R'),
	code_char('M'),
	code_char('A'),
	code_char('T'),
	code_char('I'),
	code_char('K'),
	code_char('A'),
	code_char('$')
    };

    int sa_valid[] = {14, 13, 9, 0, 5, 11, 3, 1, 12, 8, 4, 2, 6, 7, 10};
    long* sa = suffix_array(t);
    
    for (int i = 0; i < 15; i++) {
        if(sa[i] != sa_valid[i]) {
            test_pass = 0;
        }
    }

    printf("sa_test.c :: suffix_array_test(): %d\n", test_pass);

    free(sa);
}
/*
void suffix_array_no_text_end_symbol_test() {
    int test_pass = 1;

    char* t = "BIOINFORMATIKA";
    int* sa = suffix_array(t);

    if (sa != NULL) {
        test_pass = 0;
    }

    printf("sa_test.c :: suffix_array_no_text_end_symbol_test(): %d\n", test_pass);
}
*/

void sais_norecursion_test() {
    int test_pass = 1;

    long text[15] = {
	code_char('B'),
	code_char('I'),
	code_char('O'),
	code_char('I'),
	code_char('N'),
	code_char('F'),
	code_char('O'),
	code_char('R'),
	code_char('M'),
	code_char('A'),
	code_char('T'),
	code_char('I'),
	code_char('K'),
	code_char('A'),
	code_char('$')
    };

    long indices$[] = { 14 };
    long indicesA[] = { 13, 9 };
    long indicesB[] = { 0 };
    long indicesF[] = { 5 };
    long indicesI[] = { 11, 3, 1 };
    long indicesK[] = { 12 };
    long indicesM[] = { 8 };
    long indicesN[] = { 4 };
    long indicesO[] = { 2, 6 };
    long indicesR[] = { 7 };
    long indicesT[] = { 10 };

    struct bucket buckets_valid[11] = {
	{ code_char('$'), indices$, 1, 0 },
	{ code_char('A'), indicesA, 2, 0 },
	{ code_char('B'), indicesB, 1, -1 },
	{ code_char('F'), indicesF, 1, -1 },
	{ code_char('I'), indicesI, 3, -1 },
	{ code_char('K'), indicesK, 1, 0 },
	{ code_char('M'), indicesM, 1, 0 },
	{ code_char('N'), indicesN, 1, 0 },
	{ code_char('O'), indicesO, 2, 0 },
	{ code_char('R'), indicesR, 1, 0 },
	{ code_char('T'), indicesT, 1, 0 },
    };

    struct bucket_suite* bs = sais(text, 15, 27);

    for (int i = 0; i < bs->length; ++i) {
	for (int j = 0; j < bs->buckets[i].length; ++j) {
	    if (bs->buckets[i].indices[j] != buckets_valid[i].indices[j]) {
		printf("%c: got %ld, expected %ld\n",
		       decode_char(bs->buckets[i].character),
		       bs->buckets[i].indices[j],
		       buckets_valid[i].indices[j]);
		test_pass = 0;
	    }
	}
    }
   
    printf("sa_test.c :: sais_norecursion_test(): %d\n", test_pass);

    free(bs);
}

void sais_recursion_test() {
    int test_pass = 1;
    
    long text[15] = {
	code_char('M'),
	code_char('M'),
	code_char('I'),
	code_char('S'),
	code_char('S'),
	code_char('I'),
	code_char('S'),
	code_char('S'),
	code_char('I'),
	code_char('I'),
	code_char('P'),
	code_char('P'),
	code_char('I'),
	code_char('I'),
	code_char('$')
    };

    long indices$[] = { 14 };
    long indicesI[] = { 13, 12, 8, 9, 5, 2 };
    long indicesM[] = { 1, 0 };
    long indicesP[] = { 11, 10 };
    long indicesS[] = { 7, 4, 6, 3 };

    struct bucket buckets_valid[5] = {
	{ code_char('$'), indices$, 1, 0 },
	{ code_char('I'), indicesI, 5, 0 },
	{ code_char('M'), indicesM, 2, -1 },
	{ code_char('P'), indicesP, 2, -1 },
	{ code_char('S'), indicesS, 4, -1 }
    };

    struct bucket_suite* bs = sais(text, 15, 27);

    for (int i = 0; i < bs->length; ++i) {
	for (int j = 0; j < bs->buckets[i].length; ++j) {
	    if (bs->buckets[i].indices[j] != buckets_valid[i].indices[j]) {
		printf("%c: got %ld, expected %ld\n",
		       decode_char(bs->buckets[i].character),
		       bs->buckets[i].indices[j],
		       buckets_valid[i].indices[j]);
		test_pass = 0;
	    }
	}
    }
   
    printf("sa_test.c :: sais_recursion_test(): %d\n", test_pass);

    free(bs);
}

void suffix_array_from_file_test() {
    int test_pass = 1;
    
    int i;
    // TODO try with really large files.
    char* in_path = "data/input_test_file1.txt";
    long* t = read_file(in_path);

    long sa_valid[] = {10000, 5204, 4925, 6781, 9280, 4287, 6910, 2643,
    2814, 2260, 1702, 1561, 9589, 5205, 7169, 2724, 4332, 4926, 1451, 6480,
    9248, 4257, 7907, 2986, 6782, 2981, 3523, 3286, 2220, 5968, 6361, 1550,
    2384, 4949, 8762, 9281, 1186, 2550, 8400, 4288, 6911, 1097, 2289, 6476,
    9701, 8784, 2644, 6403, 2815, 5656, 1633, 7507, 2261, 845, 1703, 1567,
    1562, 3927, 5960, 4825, 8457, 9001, 6012, 3292, 4508, 4246, 5458, 1181,
    6, 9590, 6197, 3129, 1049, 4385, 2043, 8548, 6768, 9837, 8287, 9085,
    4394, 3622, 2399, 5206, 5195, 2884, 1465, 3119, 9797, 6787, 2315, 6352,
    8122, 8965, 4345, 2148, 1921, 7021, 4487, 3296, 6963, 4250, 5361, 684,
    393, 7170, 3444, 2265, 4237, 13, 4653, 2725, 4333, 4927, 2144, 1452,
    7762, 960, 6481, 5516, 3539, 9249, 4626, 880, 4885, 9303, 4258, 5922,
    7908, 7223, 6139, 4179, 6335, 3421, 5873, 9610, 1117, 3870, 6103, 5349,
    516, 8415, 5631, 2987, 6783, 7810, 1796, 8349, 2982, 3524, 3849, 6855,
    9393, 3054, 3107, 5014, 2320, 8420, 4241, 3160, 1498, 5869, 6461, 5171,
    702, 8231, 3287, 8838, 6099, 2221, 5969, 6362, 3040, 2659, 5158, 7705,
    3509, 5947, 237, 6259, 9262, 579, 5836, 9913, 9015, 3951, 3076, 1551,
    8410, 705, 1953, 8039, 2385, 7429, 373, 4950, 8190, 6718, 8763, 3249,
    8874, 8558, 8598, 9282, 4891, 2720, 8527, 6925, 8313, 1187, 1293, 1855,
    2949, 4682, 1206, 2551, 8806, 9958, 1089, 46, 5920, 1951, 8401, 1258,
    4289, 8212, 6912, 4077, 4608, 7063, 1098, 7033, 8018, 2290, 1531, 3997,
    1985, 5130, 9040, 2216, 8393, 1358, 2671, 6676, 3223, 1728, 8366, 6477,
    8403, 4061, 5105, 9189, 978, 6695, 1710, 8082, 3453, 7264, 7046, 9702,
    8785, 2645, 4757, 2597, 1913, 7126, 6743, 9724, 3846, 4773, 6972, 6404,
    4398, 2274, 5895, 932, 2816, 5110, 9142, 3050, 7681, 5657, 9270, 1845,
    1634, 7508, 2455, 5336, 292, 788, 7319, 181, 2262, 8160, 3431, 5492,
    4604, 846, 4526, 1704, 1830, 8004, 7003, 7420, 8683, 5144, 1659, 4872,
    2504, 5849, 1568, 8186, 7122, 5082, 1260, 7240, 1563, 2081, 121, 3548,
    3928, 6699, 1969, 8941, 8902, 2435, 5961, 6778, 1910, 6327, 7768, 8045,
    7994, 1509, 9976, 4826, 8458, 6057, 7486, 7370, 3764, 6330, 2876, 8406,
    2945, 396, 9002, 5092, 2023, 7271, 4782, 9357, 4519, 4633, 3313, 6013,
    4291, 3293, 4650, 7293, 6132, 646, 1424, 5980, 7963, 8937, 3409, 6690,
    6950, 3176, 259, 59, 4204, 3883, 4830, 5697, 674, 9742, 2682, 9901,
    4509, 2565, 1756, 564, 4247, 7702, 1907, 2764, 78, 6756, 4378, 9334,
    2405, 5459, 985, 1182, 6620, 7, 9518, 8503, 5181, 860, 9591, 6198,
    3130, 6879, 5611, 6890, 5690, 5521, 8163, 2104, 6274, 6036, 1050, 4386,
    4095, 9614, 2044, 1714, 4302, 3900, 3603, 8549, 6092, 6624, 8383, 8637,
    2966, 6769, 8029, 8086, 1548, 9835, 7019, 9013, 6776, 4226, 327, 7226,
    2639, 1576, 9838, 7113, 7407, 9150, 8288, 7872, 4228, 9086, 9670, 8320,
    9752, 7865, 6814, 7460, 350, 4395, 1348, 3623, 2400, 9113, 8214, 1214,
    5207, 3821, 3280, 2577, 4081, 5148, 5177, 9708, 7072, 7888, 5196, 503,
    8726, 6590, 1461, 7619, 6295, 3045, 1601, 8118, 4275, 2252, 5720, 7947,
    7275, 2738, 9314, 9582, 1073, 2885, 8452, 4430, 8301, 7357, 1466, 329,
    3120, 7520, 3782, 1975, 5739, 9501, 3345, 9798, 6788, 9409, 6552, 2316,
    972, 9388, 5236, 7572, 5412, 9692, 2767, 2853, 2500, 7771, 6914, 5246,
    5356, 4415, 2441, 945, 81, 6353, 9953, 4568, 9585, 4504, 1351, 3909,
    9443, 269, 6633, 2344, 8123, 4728, 6146, 8444, 3203, 7549, 4657, 4375,
    8966, 5328, 2035, 4920, 1200, 7449, 1527, 4346, 2149, 898, 4747, 1696,
    2173, 3065, 1922, 4325, 4811, 7925, 8136, 5259, 7564, 2559, 2654, 2606,
    586, 1134, 361, 2357, 8397, 929, 4554, 3718, 708, 4989, 2649, 6390,
    8048, 7022, 1445, 6531, 9019, 1898, 5556, 7411, 6088, 8067, 8234, 7099,
    8493, 4073, 4498, 8734, 6960, 6425, 4488, 3297, 197, 5400, 6964, 7228,
    1490, 9096, 1956, 3916, 1416, 4251, 8633, 9534, 5362, 5926, 164, 685,
    5220, 1323, 6452, 174, 6163, 613, 2518, 6611, 2641, 4947, 3290, 3074,
    9038, 1726, 7001, 5847, 1967, 394, 8318, 1346, 4079, 8699, 5341, 6240,
    1578, 7104, 9434, 8272, 9078, 93, 2858, 1803, 7483, 453, 7171, 8331,
    7042, 7997, 3152, 3937, 3445, 6465, 4610, 5320, 6185, 6734, 8701, 231,
    1301, 3358, 4447, 2266, 490, 339, 4238, 5343, 2960, 6996, 9166, 5114,
    3796, 14, 2169, 1849, 4820, 6542, 1340, 5487, 8148, 3986, 9931, 801,
    8570, 6156, 1273, 3528, 32, 6142, 528, 3367, 5368, 8617, 4654, 6153,
    2726, 4279, 8277, 7065, 1068, 9840, 909, 8362, 2770, 9683, 4334, 1428,
    5324, 9780, 4928, 5385, 3664, 2145, 8042, 5417, 0, 8626, 2698, 379,
    4514, 7296, 6242, 1100, 558, 9242, 7395, 303, 4453, 1580, 4985, 1453,
    9062, 954, 6759, 4709, 7763, 961, 9486, 3081, 4663, 3156, 9138, 7587,
    6482, 2898, 4933, 1842, 8183, 9475, 5051, 9981, 5517, 542, 3540, 7130,
    1011, 2905, 9250, 4627, 4104, 881, 448, 8258, 840, 2824, 6934, 9789,
    5616, 3853, 9117, 5829, 8097, 4886, 6371, 9304, 6827, 4600, 1406, 4761,
    4259, 5917, 8723, 5923, 6993, 3607, 1837, 456, 8975, 7106, 5604, 5939,
    5058, 8581, 7115, 9633, 7035, 733, 4350, 8020, 8997, 2921, 7623, 6682,
    1512, 3955, 8281, 1267, 7909, 5393, 8768, 6663, 914, 1816, 9436, 1076,
    2258, 6853, 5012, 8556, 3429, 2874, 4300, 7224, 1443, 7409, 9076, 1271,
    6140, 2256, 6851, 4180, 3966, 1809, 7653, 6747, 8574, 1877, 3019, 5088,
    5731, 7938, 1016, 2124, 7697, 618, 5841, 6980, 2292, 9892, 4549, 6968,
    6336, 8754, 9923, 1610, 9106, 4381, 546, 1367, 3422, 5874, 9337, 7216,
    2846, 7152, 1533, 5284, 7793, 9047, 534, 5909, 7335, 4182, 2243, 3199,
    7248, 311, 3642, 3502, 9611, 347, 8972, 5751, 1375, 1329, 1118, 3871,
    6489, 8655, 6104, 5350, 9806, 317, 517, 4864, 368, 8416, 9571, 1586,
    5632, 3993, 4136, 9466, 5673, 3700, 2988, 6784, 4342, 9898, 29, 8274,
    7147, 5497, 2085, 7811, 1797, 3209, 9478, 7798, 3434, 8350, 2617, 3215,
    4800, 2415, 3968, 600, 9152, 9344, 2983, 2850, 3525, 3850, 6990, 9544,
    6856, 9394, 3999, 6135, 3055, 9994, 9350, 3610, 3626, 2792, 4705, 141,
    2524, 1655, 6956, 1811, 7741, 7902, 6227, 3108, 4460, 5015, 2328, 7164,
    5790, 9090, 3708, 2914, 8841, 1987, 2321, 9197, 8421, 7957, 8290, 2539,
    2942, 1964, 221, 3232, 2139, 9687, 2888, 9868, 4536, 7672, 941, 8945,
    1615, 2731, 997, 3815, 5597, 8249, 7655, 7815, 1779, 4242, 2676, 8306,
    3161, 4575, 7013, 2364, 6202, 6566, 4938, 5652, 8132, 1499, 4421, 9874,
    1314, 7874, 5870, 6462, 4390, 5776, 7209, 5132, 9130, 4692, 6050, 6749,
    9963, 9042, 7686, 5172, 7312, 7854, 6498, 3825, 7476, 7778, 9310, 6281,
    4230, 7975, 4717, 5019, 3307, 1739, 2218, 9699, 8455, 703, 1949, 4524,
    5142, 6325, 9355, 562, 9332, 2403, 9668, 5354, 3907, 8395, 8232, 3288,
    2856, 907, 9681, 3662, 1840, 4598, 7150, 532, 3640, 345, 9342, 9088,
    2912, 8839, 2729, 3638, 8536, 4308, 4218, 9675, 6100, 3104, 4132, 6537,
    41, 7884, 4321, 9414, 4557, 2222, 9080, 5970, 3678, 5990, 8563, 7176,
    4198, 7502, 2688, 3760, 5764, 4670, 8576, 2408, 1035, 6363, 3041, 8719,
    8128, 8482, 2660, 1233, 784, 6584, 1360, 4848, 409, 1868, 469, 333,
    5159, 866, 822, 8885, 1625, 3001, 8538, 2059, 2789, 4533, 2673, 9672,
    5290, 4310, 6942, 5500, 8322, 216, 7645, 9844, 7706, 4114, 8921, 1164,
    5903, 2487, 7416, 6562, 1126, 95, 3510, 5948, 224, 9768, 7182, 6368,
    7499, 2715, 1171, 5293, 9823, 3944, 8910, 5420, 6722, 427, 1928, 9754,
    8091, 238, 6260, 657, 4494, 5754, 8462, 554, 1221, 7026, 8262, 3569,
    9263, 8845, 3441, 8969, 580, 3414, 3235, 4742, 3888, 5054, 8341, 5034,
    5837, 4282, 5311, 5933, 7867, 1651, 1309, 6816, 1879, 9938, 3274, 4735,
    9914, 3581, 2591, 480, 5694, 671, 3342, 9016, 8145, 3952, 7144, 3021,
    6220, 7341, 5375, 2612, 893, 633, 9202, 5462, 1378, 4853, 5713, 3077,
    6123, 8894, 1552, 1246, 6904, 2039, 8411, 7462, 3591, 9654, 700, 3038,
    2214, 4870, 2433, 5090, 2102, 1546, 5257, 706, 1954, 8270, 8040, 9060,
    9979, 1875, 3698, 5495, 2910, 4196, 4112, 6366, 552, 3340, 550, 9458,
    8956, 7982, 8448, 2386, 2088, 4433, 5758, 1387, 159, 7430, 4142, 3124,
    5536, 8664, 3866, 4220, 1144, 374, 4877, 4951, 9677, 9848, 5733, 2996,
    6863, 638, 5429, 3180, 8191, 2509, 6719, 8670, 6576, 6678, 1437, 9829,
    8764, 9713, 7257, 2860, 1002, 8932, 2478, 3250, 4752, 1774, 3225, 8875,
    924, 3385, 2602, 2482, 3088, 9460, 3099, 5168, 7290, 6409, 7472, 8559,
    5331, 1730, 5030, 7718, 726, 7940, 6444, 591, 9447, 8599, 6025, 5678,
    6418, 9283, 265, 4892, 5667, 3768, 9529, 8368, 352, 2721, 1524, 3812,
    8958, 2109, 8528, 5542, 6926, 6255, 9185, 8812, 8314, 281, 1018, 9404,
    7542, 2970, 3374, 1188, 6171, 2445, 459, 2126, 6412, 1991, 5025, 4915,
    7984, 7699, 1693, 1140, 5744, 8077, 6030, 5952, 1294, 9216, 7444, 1856,
    988, 2280, 1996, 6432, 2950, 4683, 3349, 8673, 1332, 296, 808, 9494,
    9547, 1227, 8198, 3240, 1207, 2232, 9161, 2552, 4909, 583, 2099, 8695,
    1821, 1195, 8807, 2351, 6763, 9619, 9959, 1790, 8864, 3721, 649, 9430,
    1805, 5212, 620, 4189, 1090, 2390, 47, 3468, 1887, 4064, 5064, 8978,
    5883, 6596, 6909, 6479, 3285, 5967, 1185, 2549, 1096, 1632, 844, 1048,
    4393, 3118, 5360, 12, 3538, 4884, 5921, 6102, 7809, 3106, 5868, 8837,
    2658, 1952, 6717, 8873, 2719, 1854, 4681, 8805, 4607, 8402, 5104, 6742,
    4397, 5894, 5335, 7318, 1829, 8003, 2503, 1259, 2080, 7485, 8405, 7270,
    4632, 3312, 4290, 7962, 4829, 6623, 7018, 4225, 9751, 7864, 8213, 5176,
    9707, 3044, 2737, 8451, 7356, 7519, 3781, 9500, 971, 6913, 2440, 4567,
    1350, 7548, 6389, 6424, 8632, 612, 4078, 3936, 4609, 7064, 1427, 9779,
    2697, 1099, 8182, 9116, 8722, 455, 7034, 8019, 8280, 913, 7652, 2291,
    7215, 1532, 1374, 6488, 8654, 367, 4135, 5672, 4341, 3998, 3625, 6955,
    2327, 1986, 7814, 5651, 8131, 1313, 5775, 5131, 9041, 7475, 2217, 1948,
    2402, 8394, 4131, 4320, 8562, 7175, 2687, 1359, 1624, 2788, 2672, 6941,
    9767, 2714, 9822, 4493, 8461, 3413, 5932, 1650, 3273, 5374, 2038, 3037,
    9059, 3339, 158, 1143, 2995, 6677, 9712, 3224, 2601, 1729, 6024, 8367,
    1523, 9184, 1139, 6431, 1820, 8863, 2389, 4063, 5063, 6478, 3284, 11,
    3537, 5867, 8836, 2718, 5334, 8404, 7017, 7863, 8450, 7355, 9115, 1373,
    366, 4134, 4340, 2326, 1947, 7174, 3272, 5373, 6430, 2388, 4062, 10,
    7862, 2325, 7173, 6429, 6428, 8333, 5511, 149, 5843, 4713, 2090, 8334,
    4435, 9381, 6193, 5512, 3587, 9212, 7386, 5451, 5795, 5760, 9852, 8216,
    5823, 5106, 2199, 764, 1153, 5473, 5099, 131, 3328, 150, 5844, 6539,
    4714, 3578, 9540, 1279, 9190, 4545, 6982, 8961, 4057, 5253, 7989, 1389,
    2091, 3398, 8335, 3263, 7610, 7559, 4436, 9228, 5859, 7188, 9382, 979,
    7375, 6194, 7759, 5513, 8595, 6696, 4779, 1711, 8083, 161, 3588, 9213,
    7387, 3454, 1646, 8589, 5591, 3114, 7076, 4402, 1216, 6306, 5452, 3496,
    6839, 5796, 6288, 7265, 43, 7367, 5761, 7933, 9853, 7047, 5230, 8217,
    9703, 9521, 711, 1537, 5275, 8786, 5779, 8377, 5824, 3246, 5107, 2646,
    4758, 2598, 3269, 2196, 435, 3417, 2294, 1746, 6640, 1914, 9254, 7109,
    2200, 7068, 9894, 2179, 4008, 6859, 2693, 2183, 6116, 9294, 7432, 6379,
    3792, 273, 4562, 4551, 7127, 6744, 9651, 5209, 1121, 2828, 1640, 1518,
    765, 1154, 6649, 2053, 8648, 5474, 2285, 5100, 132, 6521, 3329, 9725,
    9637, 151, 1449, 2142, 514, 3847, 1356, 7044, 3844, 6970, 7238, 858,
    7886, 9386, 7570, 9690, 4323, 5845, 6540, 952, 5907, 8304, 4573, 6048,
    3823, 4715, 467, 6560, 3579, 3864, 2278, 3282, 8593, 6046, 4774, 4313,
    7554, 2530, 3670, 8174, 5956, 4034, 3976, 6973, 6405, 9416, 9541, 7212,
    4399, 4559, 2275, 1280, 2579, 6658, 2224, 4902, 6338, 9987, 204, 9191,
    3483, 3392, 5135, 2002, 5896, 6448, 7286, 3141, 933, 8207, 7360, 2014,
    2817, 8756, 7426, 6887, 1800, 5111, 4546, 1872, 8870, 3973, 4898, 9143,
    2891, 3560, 9736, 780, 8780, 5983, 4144, 9925, 1784, 4584, 7774, 6020,
    849, 6983, 9643, 9237, 3126, 9082, 8962, 5346, 3051, 1982, 4058, 1598,
    4297, 6901, 5254, 3861, 9733, 3874, 4857, 7990, 7682, 4083, 666, 4152,
    5406, 7966, 1390, 3877, 7999, 9660, 169, 5658, 6269, 4992, 5972, 2092,
    3399, 8336, 9271, 3264, 7629, 1846, 2412, 938, 7254, 6168, 3933, 4776,
    3773, 1635, 7509, 7611, 7560, 4212, 3219, 405, 1283, 8111, 4437, 9229,
    9881, 3024, 8060, 5860, 2706, 2456, 5150, 5647, 6822, 9171, 5337, 444,
    2118, 7189, 2963, 9991, 1612, 293, 9491, 3575, 7235, 9383, 789, 980,
    7320, 2298, 3680, 9397, 5268, 55, 5538, 182, 7376, 4068, 5069, 9108,
    1932, 6511, 8666, 8739, 6312, 285, 8610, 4999, 1681, 6195, 4383, 5193,
    2263, 7760, 5514, 4624, 3868, 8596, 6697, 4780, 5179, 5688, 8161, 1712,
    8084, 8299, 5737, 5234, 5410, 162, 6999, 8624, 3154, 1441, 5749, 3432,
    5788, 9697, 4596, 1124, 2610, 3589, 2212, 5493, 548, 4140, 9214, 3238,
    4907, 1630, 4605, 4491, 9710, 7074, 3790, 8778, 847, 9169, 5747, 4527,
    8952, 1705, 7388, 9363, 7890, 5305, 3939, 6492, 4860, 3455, 1938, 1831,
    8005, 3, 9147, 1045, 1647, 5508, 8590, 2974, 5592, 1369, 9816, 7004,
    5117, 6897, 3651, 7421, 1591, 8240, 8684, 6060, 4315, 6945, 5992, 757,
    3167, 812, 3447, 118, 5145, 3364, 5664, 3115, 4222, 3034, 6637, 511,
    7077, 7138, 7849, 6216, 8565, 9425, 1476, 72, 1660, 4873, 2505, 5850,
    4403, 7178, 1217, 8485, 6917, 5198, 3378, 5576, 739, 5074, 5607, 4529,
    4541, 1569, 3744, 8187, 7123, 1298, 8629, 7135, 5083, 6307, 1146, 5122,
    1261, 7241, 5453, 505, 3497, 6467, 6840, 8517, 5797, 8679, 3300, 3834,
    6289, 4612, 2238, 7053, 1564, 1413, 5217, 376, 9073, 2082, 3212, 1736,
    4149, 3831, 1174, 2428, 122, 9008, 2187, 1177, 642, 4943, 24, 3549,
    3424, 2205, 3929, 6700, 7080, 5876, 9578, 1970, 2452, 5382, 2895, 8942,
    9339, 9455, 2785, 7556, 208, 4091, 854, 9554, 3687, 873, 8728, 7266,
    8903, 4979, 2436, 7929, 5572, 6078, 992, 6299, 5962, 7218, 4923, 6779,
    2979, 1919, 8347, 44, 8080, 1911, 7368, 6328, 4428, 7769, 4726, 4745,
    8046, 195, 6238, 7995, 5322, 9484, 1404, 1510, 6978, 2848, 3706, 2362,
    3636, 6535, 5762, 2431, 9977, 8954, 7316, 4827, 8459, 5865, 7353, 3585,
    8375, 2194, 5191, 6058, 4977, 5433, 4879, 473, 1085, 7934, 5249, 4965,
    9854, 7839, 7487, 4200, 7371, 7048, 3776, 691, 4767, 7504, 3765, 949,
    5231, 1541, 6331, 9949, 8202, 9762, 2582, 836, 2777, 6808, 2877, 5806,
    8225, 6672, 8707, 7735, 8218, 5525, 1482, 8407, 2946, 1707, 5009, 8907,
    9704, 3336, 9984, 9481, 9156, 1060, 397, 9003, 6592, 9522, 3476, 7390,
    8658, 3740, 5093, 712, 6439, 7727, 7749, 2024, 4017, 2532, 7692, 902,
    4002, 6033, 7272, 6266, 1538, 4783, 5276, 2701, 7638, 8787, 7201, 6223,
    5641, 382, 5780, 8855, 9945, 6800, 9358, 8506, 3134, 5942, 3672, 4520,
    5435, 136, 211, 4634, 2341, 9871, 7642, 4881, 5441, 3982, 8832, 3314,
    6187, 200, 4953, 5817, 8378, 8176, 8691, 3464, 6736, 6883, 7301, 5468,
    3921, 5503, 9030, 5998, 5444, 1558, 7954, 7141, 2690, 4962, 8703, 8325,
    5184, 475, 4267, 6014, 3799, 7531, 4292, 7154, 233, 1903, 5825, 605,
    1395, 5202, 5958, 1463, 6350, 3294, 4651, 3247, 8311, 1087, 7061, 8391,
    5108, 3762, 6877, 3601, 1459, 2647, 6732, 526, 7294, 4759, 7621, 6661,
    7936, 1327, 6133, 219, 7852, 7973, 9679, 6582, 5288, 631, 1544, 1435,
    9827, 3810, 647, 1827, 1425, 2599, 5061, 1945, 3270, 9850, 2197, 1151,
    129, 4055, 5251, 1535, 3244, 1638, 5981, 7964, 5735, 116, 509, 7847,
    6297, 2977, 8830, 7059, 5286, 2514, 9365, 436, 3418, 8938, 7795, 2998,
    3410, 2295, 6347, 6579, 6107, 8468, 1747, 6641, 6704, 6691, 2622, 4036,
    9049, 1915, 3736, 9255, 5889, 7892, 3961, 888, 536, 6951, 3978, 5766,
    4967, 3891, 8584, 9259, 3047, 7110, 3177, 1690, 260, 4796, 2201, 60,
    4205, 1303, 413, 19, 5911, 3884, 5307, 7512, 2332, 1158, 4672, 3069,
    7801, 7069, 9895, 9865, 9401, 2180, 6376, 52, 6975, 3333, 6041, 1007,
    7337, 3360, 4831, 792, 5698, 571, 1469, 4184, 7524, 675, 9558, 4048,
    8034, 6865, 7344, 4009, 6249, 2245, 5703, 9743, 7824, 8578, 6860, 7516,
    2694, 2184, 9856, 6792, 4449, 6117, 9295, 6399, 3254, 2683, 7433, 4789,
    1719, 7841, 4169, 7489, 6380, 2070, 2268, 2396, 8899, 3793, 7332, 3941,
    4109, 5772, 6518, 8607, 274, 3437, 2831, 9902, 9969, 1319, 7160, 6494,
    8431, 7614, 4510, 9597, 1603, 3354, 7831, 2566, 1757, 4563, 8851, 565,
    919, 9795, 8120, 4248, 7703, 9911, 1908, 1507, 4517, 4202, 983, 1071,
    2765, 79, 3201, 2652, 4552, 2516, 6151, 4277, 6757, 7128, 2254, 6745,
    4379, 9335, 4862, 3207, 5595, 2406, 1162, 5460, 9652, 6407, 7716, 263,
    986, 1193, 5210, 1183, 1094, 6621, 9820, 9057, 8, 147, 9538, 7608,
    7373, 9519, 6044, 3859, 2410, 4210, 1679, 1122, 6895, 70, 4539, 640,
    5431, 8504, 7299, 5466, 5182, 886, 17, 5770, 2829, 7714, 5722, 861,
    2869, 2423, 9592, 2134, 1131, 6199, 7050, 3131, 6880, 3182, 2663, 5612,
    6668, 6891, 8193, 9220, 1641, 9373, 5724, 3457, 9125, 9859, 4468, 1519,
    6795, 9418, 423, 766, 5628, 1203, 863, 5691, 8929, 6386, 5522, 1155,
    3319, 8164, 6650, 7949, 492, 6245, 8472, 4099, 2937, 1236, 9367, 2160,
    2105, 7250, 8826, 1940, 7675, 6275, 2054, 6604, 5481, 3751, 1833, 6037,
    2955, 2467, 9717, 9602, 1051, 6207, 2871, 4387, 5403, 1473, 4096, 9034,
    8715, 7755, 3490, 7452, 7914, 6472, 7118, 9615, 7579, 6711, 2045, 2836,
    253, 8649, 3786, 8513, 1715, 4303, 2112, 3901, 5475, 438, 4646, 3014,
    85, 8531, 5545, 6456, 6871, 2286, 1290, 3604, 5101, 3778, 1733, 2425,
    5379, 133, 126, 9594, 693, 8550, 6110, 7648, 6093, 6625, 7277, 313,
    9664, 5296, 696, 3644, 6522, 8384, 8638, 3504, 3060, 6358, 2136, 2967,
    7283, 5638, 2511, 3330, 4643, 1287, 4087, 6929, 7437, 6770, 2740, 9316,
    2925, 4030, 8030, 5530, 2304, 9726, 4769, 8087, 1037, 1103, 9638, 341,
    277, 7382, 1503, 9133, 152, 8007, 4286, 7168, 4331, 1450, 9247, 1549,
    7506, 5, 9836, 7020, 2143, 959, 879, 6138, 4178, 6334, 3420, 515, 3848,
    4240, 5157, 6258, 578, 9014, 3950, 8038, 1257, 7032, 8017, 1530, 1357,
    9188, 7045, 9723, 3845, 6971, 9269, 787, 8159, 5081, 7239, 8940, 6777,
    7767, 7993, 6056, 3175, 3882, 2681, 2564, 859, 5520, 4094, 9613, 9012,
    9149, 4227, 349, 3279, 7887, 502, 6294, 8117, 328, 6551, 9387, 7571,
    9691, 5245, 944, 9952, 9584, 2172, 4324, 7563, 1133, 2356, 5555, 7227,
    9533, 173, 2640, 9037, 1725, 5846, 8698, 1577, 7041, 3151, 230, 2959,
    6541, 3985, 8569, 9839, 302, 953, 541, 1010, 2904, 6933, 8974, 5057,
    7114, 732, 4349, 6681, 1815, 1075, 7408, 6850, 2123, 6967, 9922, 1609,
    545, 2845, 5908, 7247, 28, 7797, 4799, 9151, 9543, 4704, 8289, 2887,
    8248, 8305, 4574, 6201, 4420, 7873, 6049, 7685, 3824, 4229, 4716, 8454,
    5141, 561, 5353, 9087, 9413, 8718, 8481, 468, 332, 821, 3000, 9671,
    8321, 9843, 2486, 6561, 7498, 6721, 9753, 656, 5753, 3440, 5033, 7866,
    1308, 6815, 4734, 3580, 670, 6219, 7340, 1377, 5712, 7461, 4869, 6365,
    4432, 3123, 5535, 3865, 9847, 1001, 3384, 5167, 6443, 590, 3767, 351,
    3373, 2279, 8672, 1331, 8197, 9160, 5882, 5966, 1047, 5359, 7808, 1853,
    4396, 5893, 4631, 3043, 1349, 8721, 7214, 3624, 8130, 2401, 4130, 3412,
    1649, 157, 2994, 3283, 8835, 9114, 1372, 5510, 6192, 8215, 5098, 3397,
    9227, 5858, 7187, 7758, 8594, 4401, 1215, 6838, 9293, 4561, 5208, 1120,
    2052, 2284, 9636, 1355, 3843, 857, 951, 8303, 6047, 3822, 2277, 3281,
    8592, 5955, 2578, 6657, 4901, 2001, 3140, 7359, 2013, 4897, 6019, 5345,
    9732, 3873, 4082, 4775, 1282, 8110, 5149, 2117, 2962, 2297, 5178, 5233,
    6998, 1440, 9709, 7073, 9168, 7889, 6491, 9146, 9815, 5116, 4314, 756,
    3363, 3033, 8484, 5197, 1297, 504, 7052, 1412, 2204, 9577, 2894, 7555,
    4090, 853, 8727, 6077, 4427, 6237, 3705, 7352, 5190, 2581, 8224, 8706,
    5008, 6591, 8657, 7726, 2531, 7691, 7637, 7200, 3133, 3671, 5816, 8175,
    6882, 3920, 4961, 4266, 3798, 7530, 5957, 1462, 6349, 7620, 6660, 7972,
    6581, 1543, 1434, 9826, 1826, 1944, 115, 7846, 6296, 2976, 7058, 6346,
    6578, 6106, 4035, 3977, 3046, 1689, 6974, 1468, 7523, 2395, 4108, 7159,
    8430, 1602, 7830, 8119, 4276, 2253, 5594, 6406, 9819, 885, 16, 7713,
    5721, 2662, 9219, 9417, 422, 3318, 7948, 8471, 1235, 3750, 9601, 7913,
    2835, 3785, 5378, 125, 6109, 7276, 3059, 4086, 2739, 9315, 4330, 9246,
    878, 6333, 5156, 577, 1256, 786, 5080, 6055, 3174, 501, 6293, 9951,
    9583, 2171, 8568, 6932, 6680, 1074, 9542, 2886, 8247, 8453, 5352, 331,
    669, 5711, 4431, 3122, 5534, 3383, 5166, 9159, 1852, 7213, 4129, 2993,
    1371, 4400, 4560, 2051, 8302, 2276, 2000, 7358, 2012, 4896, 1281, 1439,
    9814, 1411, 4426, 2580, 7529, 6659, 1825, 1688, 1467, 7522, 2394, 9818,
    7712, 3317, 8470, 2834, 3784, 3058, 9245, 5155, 576, 8246, 330, 3121,
    3382, 1851, 4425, 1824, 1687, 7521, 7711, 3783, 8245, 7710, 8244, 7709,
    2225, 7537, 6127, 1242, 2155, 1749, 2226, 1976, 7538, 5740, 4903, 3543,
    8440, 6339, 7006, 6128, 6586, 6321, 9121, 8818, 9502, 1211, 1243, 3346,
    8204, 9988, 5119, 205, 9799, 3184, 9192, 3484, 5126, 2156, 1750, 1767,
    6727, 6556, 7745, 2227, 8985, 1977, 4822, 8328, 5426, 7539, 5741, 6938,
    9378, 761, 7364, 4904, 6789, 9410, 6553, 2710, 9325, 3544, 7583, 9831,
    3393, 5136, 1362, 2003, 5897, 4365, 2665, 2317, 8441, 6449, 7287, 9764,
    6285, 3947, 5187, 973, 2584, 1420, 3142, 3192, 9808, 9389, 7440, 934,
    5580, 4697, 5237, 6340, 8476, 319, 8208, 2635, 5854, 7007, 188, 6643,
    6706, 4120, 9997, 6129, 6587, 5048, 6322, 4850, 8074, 3493, 6436, 9122,
    2934, 3030, 8815, 7361, 8071, 2460, 2015, 5623, 7573, 5315, 5413, 2463,
    7466, 519, 2818, 9693, 7721, 5586, 8800, 7602, 8819, 9503, 8353, 8757,
    3258, 1667, 1496, 6097, 7427, 976, 6693, 7679, 6888, 325, 1212, 4273,
    1198, 6086, 1801, 5112, 2768, 838, 5614, 8766, 5086, 4547, 3991, 2615,
    7310, 9353, 2854, 39, 1244, 1873, 8810, 3347, 8871, 2501, 2078, 9210,
    3974, 8205, 7772, 6899, 5645, 9989, 6915, 7133, 5120, 2236, 206, 5247,
    6670, 6264, 5439, 8389, 1457, 2620, 411, 6893, 9715, 5553, 7039, 8195,
    5357, 4899, 9144, 2892, 5006, 7970, 4695, 7600, 9800, 7259, 3185, 8152,
    4416, 3561, 9737, 9470, 6180, 1063, 9193, 9275, 965, 63, 4161, 717,
    9625, 9222, 3485, 2779, 5127, 3913, 781, 2442, 946, 2157, 82, 4866,
    2152, 6544, 4025, 6354, 7455, 9954, 1342, 2841, 1751, 9025, 4155, 9802,
    9563, 1768, 4569, 6804, 743, 1025, 9586, 8781, 4505, 370, 7261, 5685,
    3807, 1352, 3910, 3187, 7305, 5984, 3322, 3515, 109, 8603, 4145, 6547,
    2862, 8167, 6007, 9926, 5833, 9444, 1643, 5227, 270, 6634, 8688, 1004,
    7349, 2624, 1785, 8012, 4585, 2345, 6810, 8124, 7731, 4729, 2879, 9629,
    7231, 6147, 7092, 6728, 4957, 5808, 4038, 1382, 2627, 8154, 3613, 6833,
    6072, 5489, 8934, 1573, 7775, 9329, 8445, 6021, 6557, 7746, 3204, 850,
    9375, 7917, 6984, 2228, 7550, 8986, 9644, 8227, 4973, 2471, 1978, 4658,
    7398, 9238, 9051, 2018, 4823, 3127, 9083, 8963, 5347, 3052, 8418, 1983,
    6674, 4059, 4376, 1599, 7447, 2557, 8329, 488, 2167, 5391, 4298, 1014,
    7011, 5901, 8967, 6902, 5255, 5427, 2480, 5329, 6416, 7540, 5742, 9428,
    6939, 2036, 9379, 762, 7365, 3862, 1870, 9734, 3875, 7627, 5266, 6310,
    4905, 4858, 8238, 737, 4921, 1917, 471, 3474, 3738, 2339, 629, 9257,
    6790, 3252, 9793, 5626, 1201, 7753, 7450, 1528, 7991, 2354, 4347, 9920,
    4418, 7683, 9411, 5891, 4084, 6053, 667, 6554, 186, 5551, 8150, 2150,
    4153, 5726, 8913, 400, 9573, 3653, 3563, 3459, 6503, 750, 4754, 9739,
    6753, 6773, 9472, 5728, 1776, 9127, 2475, 2711, 5407, 5569, 899, 7576,
    1493, 7967, 9326, 1391, 335, 2545, 9861, 5161, 8915, 4470, 4748, 3878,
    1697, 2174, 3545, 7584, 8000, 402, 3066, 4786, 9661, 170, 3988, 5659,
    8709, 9099, 1923, 6571, 6270, 8948, 1400, 4326, 4834, 868, 4812, 7661,
    4483, 4807, 1618, 9832, 308, 4215, 1588, 7926, 1479, 3394, 5423, 3804,
    4993, 795, 5137, 5973, 8137, 5260, 7565, 4676, 2093, 3227, 3400, 1363,
    8337, 3632, 2560, 2004, 9933, 3897, 6182, 1065, 8553, 2655, 7423, 4996,
    2607, 587, 8107, 9272, 5898, 8543, 4366, 8877, 7084, 1135, 7737, 5634,
    9289, 1593, 2666, 3265, 6653, 8103, 824, 7894, 8887, 803, 5562, 362,
    2358, 7630, 6393, 8398, 8546, 6766, 2313, 682, 391, 2318, 3995, 930,
    8442, 896, 8065, 6450, 1847, 8572, 2413, 9195, 939, 6279, 4555, 8883,
    478, 8143, 8892, 2908, 7255, 7288, 6169, 1788, 3719, 6740, 3934, 9765,
    1521, 5821, 4777, 6286, 709, 3390, 4990, 6820, 4138, 9006, 2450, 9552,
    3774, 1149, 1636, 7510, 5701, 2068, 7330, 7612, 2650, 4208, 4028, 7380,
    3948, 8015, 7561, 819, 9575, 5188, 5814, 574, 8242, 6725, 3190, 974,
    9468, 5225, 8686, 2337, 4213, 6391, 3713, 2585, 828, 8049, 6062, 3220,
    1421, 926, 3715, 406, 7023, 4317, 1446, 6532, 6797, 1284, 4022, 8112,
    6232, 2587, 4438, 9230, 9882, 3025, 3143, 8061, 3009, 7920, 3193, 9809,
    4369, 9905, 6686, 5861, 8220, 5279, 8055, 7323, 2743, 386, 9020, 9277,
    9390, 3963, 7441, 663, 935, 5527, 2707, 2457, 3629, 8880, 967, 65,
    8746, 9972, 7898, 1899, 3655, 5557, 9887, 6158, 4163, 7592, 5581, 9420,
    4698, 7412, 1275, 5151, 5238, 4617, 6947, 6089, 6987, 9776, 5648, 1627,
    4117, 8068, 6177, 8235, 3003, 3530, 6341, 719, 430, 8435, 1484, 7100,
    6823, 8477, 8494, 320, 9567, 3565, 7784, 9172, 8209, 4074, 2636, 5397,
    5338, 798, 445, 890, 5675, 3461, 4793, 538, 5855, 3702, 7008, 8540,
    3387, 7087, 8489, 5994, 2119, 1110, 189, 2061, 3095, 1251, 6317, 4442,
    4499, 34, 6644, 8735, 6707, 4121, 7190, 3691, 9998, 6961, 9608, 1115,
    6130, 8501, 2964, 8027, 3819, 6588, 6144, 2033, 2604, 5318, 5366, 5049,
    5602, 1265, 9992, 1613, 6323, 3660, 530, 425, 4851, 636, 8075, 294,
    9492, 6953, 6426, 3576, 3494, 7236, 9384, 2704, 4489, 3649, 3298, 9760,
    6437, 3980, 198, 7952, 790, 981, 9055, 5768, 9123, 2935, 5401, 6869,
    8115, 1723, 6965, 7245, 2484, 6190, 3031, 6235, 8816, 759, 7362, 8072,
    2461, 6084, 7308, 9208, 5004, 9627, 7229, 7090, 2016, 5264, 5624, 7574,
    1491, 9097, 306, 7321, 5316, 1957, 2299, 3681, 5414, 1032, 6113, 6508,
    8344, 9398, 2464, 729, 3917, 2990, 1417, 8498, 7858, 5269, 4969, 495,
    4252, 7467, 830, 520, 4456, 9319, 2819, 3090, 4838, 4479, 56, 8634,
    7096, 5539, 9694, 9535, 9622, 183, 747, 7377, 3006, 5363, 9205, 9462,
    3369, 7722, 5927, 3730, 5587, 165, 3169, 4589, 8801, 3893, 5037, 2928,
    2381, 75, 1583, 2734, 5370, 9224, 1022, 679, 8743, 1859, 4069, 5070,
    4044, 686, 9109, 3533, 1933, 1663, 7603, 1959, 6512, 8820, 5221, 1862,
    2028, 7669, 3101, 8667, 8586, 8645, 1042, 1324, 3487, 250, 6453, 2301,
    6505, 2378, 8740, 8051, 6313, 286, 175, 6064, 8791, 9504, 2795, 6164,
    752, 8772, 3595, 8611, 8619, 614, 2781, 3683, 768, 2802, 8354, 2519,
    814, 8758, 3449, 7943, 3259, 2008, 5000, 1682, 1668, 6612, 2642, 2813,
    4948, 6475, 3291, 6196, 4384, 5194, 6786, 4344, 2264, 7761, 5515, 4625,
    3869, 5630, 1497, 5170, 6098, 9261, 3075, 8409, 7428, 8597, 6924, 2948,
    1205, 5129, 9039, 3222, 1727, 977, 6694, 1709, 4756, 3049, 7680, 291,
    7002, 5848, 7121, 120, 6698, 1968, 395, 4781, 7292, 1423, 258, 9741,
    9900, 1755, 6755, 6619, 5180, 6889, 5689, 8162, 1713, 8085, 6775, 326,
    7112, 8319, 1347, 1213, 4080, 5147, 4274, 2251, 8300, 5738, 5235, 5411,
    4656, 1199, 360, 928, 3717, 4988, 1897, 6087, 8492, 4072, 1489, 3915,
    163, 1322, 7000, 8317, 1802, 8700, 5342, 9165, 5113, 6155, 31, 3366,
    8616, 8276, 2769, 5416, 8625, 6241, 4452, 1579, 4708, 4662, 3155, 9474,
    4103, 839, 5615, 7105, 8767, 9435, 5011, 1442, 3018, 5087, 5730, 5840,
    4548, 8753, 3198, 3501, 5750, 3992, 8273, 3433, 2616, 9349, 2791, 7163,
    5789, 3231, 4535, 1778, 2675, 9129, 4691, 9962, 7311, 6497, 9698, 9354,
    2855, 906, 4597, 7149, 2728, 4307, 9674, 40, 9079, 3677, 1034, 1232,
    783, 408, 1867, 865, 5499, 1125, 94, 5292, 8909, 7025, 4281, 2590,
    5693, 2611, 9201, 1245, 3590, 2213, 1874, 5494, 549, 2087, 4141, 4876,
    3179, 2508, 2859, 8931, 2477, 5666, 6254, 8811, 2444, 6411, 1692, 6029,
    9215, 1995, 3348, 1226, 3239, 2231, 4908, 9618, 1804, 6595, 1631, 3117,
    6716, 8872, 4606, 2502, 2079, 7484, 4224, 9750, 9706, 9499, 970, 6388,
    8181, 454, 8279, 912, 7651, 7813, 7474, 4319, 8561, 2713, 4492, 3036,
    3338, 9711, 1138, 5333, 7861, 7172, 8332, 9211, 5794, 3113, 7075, 6639,
    9253, 7067, 6115, 3791, 1448, 513, 7043, 7569, 4312, 7553, 8173, 4033,
    3975, 9986, 203, 6447, 8206, 1799, 8779, 7773, 848, 9236, 1597, 6900,
    7998, 9659, 3218, 9880, 5646, 9170, 443, 9990, 3574, 1931, 6510, 5409,
    3153, 5748, 5787, 8777, 9362, 3938, 6944, 3446, 5663, 6215, 6916, 5073,
    4528, 7134, 5121, 6466, 8678, 4611, 2237, 3211, 7079, 9454, 207, 5571,
    991, 8346, 5321, 9483, 6534, 8953, 2193, 5248, 948, 6671, 5524, 1706,
    9480, 1059, 7389, 4016, 901, 6265, 6799, 7641, 5440, 6186, 6735, 5502,
    9029, 5997, 7140, 8702, 8324, 232, 1394, 8390, 6876, 1458, 525, 218,
    7851, 9364, 2621, 3735, 7891, 3960, 1302, 412, 5306, 1157, 7800, 9400,
    51, 3359, 9557, 4047, 8033, 6248, 4448, 4168, 2267, 3940, 3436, 6493,
    3353, 1070, 4861, 262, 146, 6894, 69, 5465, 1130, 3456, 4467, 491,
    2159, 1939, 6603, 1832, 2466, 9716, 1472, 6471, 7578, 3013, 84, 1732,
    7647, 6357, 1286, 340, 8006, 4285, 4, 4239, 2680, 9011, 9148, 5244,
    5554, 7040, 229, 731, 2122, 4798, 5140, 9842, 5032, 6218, 4868, 9846,
    1000, 8196, 1046, 5358, 7807, 4630, 1648, 5509, 5097, 2283, 3842, 8591,
    4900, 5344, 2961, 6997, 9167, 9145, 5115, 2203, 2893, 5007, 7199, 3919,
    3797, 7971, 7845, 2975, 4107, 8429, 5593, 884, 15, 3749, 2170, 8567,
    5710, 5533, 9158, 2992, 1370, 1999, 1410, 9817, 1850, 7708, 1241, 2154,
    7005, 5118, 5125, 4821, 7582, 1419, 4696, 8475, 5853, 6435, 2933, 5314,
    7720, 5585, 7601, 8352, 1495, 6096, 6898, 8388, 1456, 2619, 7038, 7969,
    1062, 62, 716, 6543, 4024, 1341, 9801, 9562, 7260, 3186, 3321, 6546,
    8166, 6006, 7348, 4956, 1381, 8153, 5488, 9328, 2556, 9427, 736, 9919,
    4417, 8149, 399, 3652, 3562, 6502, 9738, 6752, 9471, 5568, 3987, 4806,
    1478, 3803, 9932, 6181, 1064, 7422, 9288, 1592, 6652, 802, 8571, 9194,
    5820, 9005, 4207, 4027, 8241, 8685, 2336, 6061, 4316, 8054, 9276, 966,
    64, 6157, 4162, 1274, 4616, 6946, 4116, 3529, 718, 8434, 5993, 6316,
    4441, 33, 3690, 1114, 8500, 3818, 6143, 5601, 1264, 3659, 529, 7951,
    8114, 7244, 6234, 758, 9626, 728, 494, 3368, 3729, 3168, 4588, 74,
    5369, 9223, 1662, 3486, 8790, 8618, 2780, 813, 3448, 7942, 5128, 119,
    5146, 2250, 4655, 3914, 6154, 3365, 4102, 8752, 4690, 2727, 782, 4280,
    2589, 4875, 2507, 5665, 2443, 1225, 6594, 3116, 6715, 4223, 8278, 911,
    3035, 7860, 6638, 7066, 512, 6446, 3217, 9879, 5662, 8677, 7078, 2192,
    947, 4015, 7139, 1393, 7850, 6247, 1069, 68, 2158, 83, 6356, 9841,
    6217, 4867, 8566, 5709, 2153, 8474, 5852, 6545, 9426, 1477, 4026, 4440,
    1113, 5600, 3728, 73, 1661, 4101, 4874, 2506, 6714, 910, 9878, 6355,
    5851, 9065, 9066, 8523, 9524, 4404, 3478, 9232, 9509, 7456, 2809, 4802,
    9067, 4173, 8252, 8524, 9955, 8363, 9515, 1343, 7392, 597, 2939, 7179,
    1218, 8712, 2842, 1305, 5620, 106, 9884, 8486, 1752, 9026, 1238, 4156,
    2373, 593, 6918, 773, 9525, 2417, 6628, 4405, 8923, 9449, 9102, 8660,
    3479, 8425, 2771, 1793, 9803, 9684, 8867, 192, 7836, 5199, 2048, 3379,
    5577, 3027, 740, 9564, 9757, 9233, 4335, 1429, 1769, 5075, 3145, 9369,
    3724, 9510, 415, 5608, 7457, 5325, 5987, 4530, 4542, 4570, 21, 6805,
    7280, 4270, 1570, 744, 2810, 5784, 4803, 8749, 1166, 9781, 4929, 5271,
    9068, 4174, 3745, 1026, 5386, 5913, 6844, 4410, 8253, 3665, 2162, 9587,
    8782, 4506, 2146, 371, 8188, 8525, 9956, 8364, 7262, 2595, 7124, 8043,
    9516, 1344, 1299, 337, 7393, 598, 2940, 7180, 5418, 1219, 3086, 2107,
    2547, 8630, 1622, 1516, 5905, 7252, 5686, 2210, 1, 7136, 3742, 8627,
    2699, 380, 3808, 8828, 9863, 4515, 7297, 6243, 8713, 1101, 7030, 2843,
    559, 1306, 1353, 1942, 6344, 876, 499, 9243, 5621, 7677, 5084, 4159,
    3911, 107, 8601, 4971, 7396, 6308, 8063, 6277, 1147, 2066, 5812, 3188,
    9885, 8487, 6082, 7306, 304, 1030, 4454, 1581, 4042, 289, 1753, 358,
    4986, 6027, 9027, 144, 3011, 5095, 1239, 5123, 1454, 714, 1262, 7242,
    4013, 9063, 104, 5985, 497, 4157, 2064, 102, 5680, 5454, 955, 2374,
    2489, 2749, 3406, 7922, 2056, 6120, 6760, 4710, 3970, 602, 506, 7764,
    5163, 962, 3195, 3498, 6468, 594, 6841, 3323, 3516, 8518, 6420, 9487,
    1081, 6919, 774, 1883, 3082, 5798, 6606, 4664, 110, 8991, 4254, 3157,
    9139, 8680, 8094, 3757, 7469, 9526, 2348, 609, 2527, 7821, 8604, 9811,
    7658, 7818, 3301, 2418, 244, 8917, 832, 7588, 2368, 5976, 6629, 4371,
    89, 9907, 4406, 2493, 8924, 3835, 9450, 2753, 6483, 178, 9103, 8661,
    3480, 4146, 6548, 6290, 4362, 5682, 522, 8426, 9285, 4613, 2863, 2239,
    6002, 2899, 8168, 2772, 4472, 6008, 9927, 722, 1674, 6067, 5483, 4934,
    4844, 5802, 7054, 3521, 1565, 5456, 3620, 1794, 5834, 1843, 7418, 8184,
    6688, 267, 1414, 5218, 377, 4983, 9074, 9804, 2083, 9476, 3213, 4458,
    2537, 9685, 6564, 1737, 1169, 1926, 3886, 5052, 5309, 6574, 9445, 4187,
    4338, 3326, 1644, 5228, 4006, 271, 465, 8868, 1782, 4150, 6635, 3832,
    1175, 4724, 193, 2429, 5863, 7837, 9982, 8689, 5200, 1005, 7514, 2132,
    957, 5518, 6848, 543, 6441, 6017, 7350, 8222, 123, 2049, 4894, 3380,
    3541, 5578, 2633, 3028, 7131, 2839, 741, 7729, 2625, 486, 1012, 7751,
    2311, 2906, 1786, 8013, 9565, 9758, 2026, 2376, 256, 9251, 9234, 1057,
    1128, 9009, 4628, 3840, 4105, 882, 2334, 4586, 2807, 8989, 2346, 4336,
    8794, 9784, 449, 1430, 9321, 2188, 1178, 643, 6811, 4944, 2821, 5281,
    8125, 8259, 841, 5669, 2825, 3770, 8057, 7732, 25, 6935, 9790, 3092,
    5617, 3550, 3425, 2759, 3854, 3553, 1770, 4730, 2206, 2880, 9647, 5076,
    97, 3146, 9440, 9630, 7790, 3304, 3930, 7232, 6701, 9966, 6148, 9118,
    3512, 5830, 7081, 8140, 4019, 7093, 2534, 5877, 8796, 8098, 8266, 3753,
    2307, 7325, 4887, 6372, 9305, 6828, 4601, 9579, 9786, 5936, 7694, 6729,
    9370, 9729, 4958, 1407, 3725, 5809, 4039, 1971, 1383, 9511, 4840, 2628,
    416, 8155, 3614, 6834, 6073, 4762, 4260, 5918, 2453, 5490, 8935, 5609,
    8381, 1574, 7870, 7458, 8724, 7617, 5326, 9094, 5924, 451, 6994, 5383,
    2896, 1835, 9045, 3608, 8943, 7776, 9330, 1838, 9340, 5988, 4531, 3696,
    9456, 7980, 8446, 4750, 457, 5950, 8976, 8652, 2786, 6022, 4543, 7557,
    7107, 4571, 6558, 5605, 22, 689, 6806, 9154, 7747, 5940, 209, 5059,
    8466, 8582, 6039, 3205, 1160, 2421, 2953, 7116, 7281, 3880, 4092, 9531,
    2957, 5880, 9634, 855, 851, 7689, 1432, 7527, 8983, 9376, 9323, 4271,
    7598, 1571, 2469, 3472, 4481, 4674, 7918, 6985, 745, 2800, 2811, 904,
    2229, 8179, 7551, 5785, 9555, 7036, 734, 4804, 3688, 8750, 4688, 2190,
    9507, 874, 2747, 2491, 2751, 1167, 4004, 8987, 9782, 9645, 2798, 4686,
    2745, 4351, 7402, 8729, 8021, 1699, 8998, 8228, 3071, 4930, 7267, 8860,
    5272, 2176, 4974, 8904, 2922, 2075, 7624, 2472, 388, 6683, 9346, 226,
    1513, 4980, 8370, 9069, 4125, 3956, 7879, 5041, 8295, 9719, 354, 8282,
    1337, 1268, 2437, 7930, 1979, 5573, 4175, 7910, 9022, 5394, 8642, 8769,
    4659, 3746, 6079, 1027, 7399, 6664, 652, 5387, 9604, 993, 7803, 9770,
    4353, 915, 1053, 6209, 6300, 7404, 8731, 8359, 9239, 5914, 9181, 1817,
    2866, 4640, 5963, 7184, 9052, 247, 241, 6845, 9437, 4411, 8254, 1077,
    7219, 8023, 7204, 2019, 3666, 2163, 9298, 4924, 6780, 9279, 2259, 1701,
    9588, 2723, 2985, 2980, 8783, 6402, 3926, 4824, 9000, 4507, 4245, 1180,
    3128, 8286, 9084, 8964, 2147, 1920, 9302, 5348, 8348, 6854, 9392, 3053,
    5013, 8419, 8230, 372, 8189, 8557, 8526, 9957, 45, 1984, 6675, 8365,
    4060, 8081, 7263, 2596, 1912, 7125, 4772, 3430, 1658, 3547, 8044, 9975,
    7369, 6329, 2875, 645, 3408, 58, 2763, 4377, 9517, 6273, 6035, 4301,
    8636, 7225, 6813, 9112, 2576, 7071, 1600, 5719, 7274, 4429, 1974, 2852,
    7770, 4414, 4727, 7448, 1526, 4746, 7924, 2558, 8047, 1444, 7410, 7098,
    6959, 196, 6610, 4946, 3073, 1345, 6239, 9433, 9077, 7482, 8330, 7996,
    1300, 4446, 489, 338, 2168, 4819, 1272, 3527, 6141, 5323, 4513, 7394,
    9485, 7586, 4932, 8257, 2823, 3852, 6370, 1405, 6992, 1511, 5392, 2257,
    6852, 3428, 2873, 4299, 3965, 1808, 1015, 6979, 5283, 4181, 2242, 316,
    9465, 9897, 3967, 599, 2849, 1654, 1810, 7740, 7901, 6226, 3707, 2941,
    9867, 3814, 7654, 7012, 2363, 4389, 6748, 9667, 3637, 6536, 7501, 5763,
    8575, 8127, 2058, 8920, 5902, 7181, 5419, 8090, 1220, 8261, 8968, 1878,
    3020, 6122, 6903, 2432, 5089, 5256, 9978, 4195, 8955, 1386, 5732, 5428,
    2481, 3087, 3098, 5330, 5029, 7939, 6417, 2108, 5541, 1017, 9403, 7541,
    2125, 7698, 5743, 7443, 9546, 2098, 8694, 6762, 9429, 619, 3467, 2548,
    843, 7317, 8002, 7269, 7961, 4828, 5175, 8631, 5671, 1312, 1623, 6940,
    8460, 2037, 8862, 3536, 5866, 2717, 7354, 5842, 4712, 9380, 3586, 5450,
    763, 5472, 6981, 8960, 7366, 5274, 8376, 2195, 2293, 1745, 9893, 2178,
    6858, 2182, 6378, 4550, 2827, 1517, 6969, 5906, 3863, 6337, 8755, 1871,
    3972, 9735, 779, 9924, 4856, 665, 5405, 3876, 6268, 7628, 937, 7253,
    6167, 3772, 404, 8059, 1611, 9396, 5267, 54, 9107, 6311, 284, 4382,
    5192, 4623, 5687, 9696, 4595, 2211, 547, 4906, 3789, 4859, 1937, 2,
    1368, 8239, 6059, 3166, 7137, 1475, 738, 3743, 8628, 8516, 5216, 1173,
    3423, 5875, 9338, 4978, 7217, 4922, 1918, 6977, 2847, 7315, 8374, 4976,
    472, 1540, 835, 7734, 8906, 3335, 3475, 3739, 4001, 2700, 381, 5434,
    2340, 4880, 1557, 474, 7153, 1902, 604, 8310, 1086, 7935, 630, 3809,
    5250, 1534, 508, 8829, 5285, 7794, 9048, 5888, 535, 4966, 9258, 5910,
    3068, 9864, 7336, 4183, 2244, 9855, 6791, 3253, 4788, 1718, 7840, 7488,
    8898, 9794, 4516, 4201, 3200, 1192, 9537, 7607, 7372, 6043, 3858, 7298,
    7049, 6667, 5627, 1202, 6244, 4098, 7249, 5480, 6206, 8714, 7754, 7451,
    2111, 8530, 5544, 3777, 692, 312, 9663, 5295, 3643, 3503, 5637, 6928,
    2924, 5529, 4768, 1102, 7505, 6137, 6257, 7031, 1529, 9187, 7766, 7992,
    9612, 348, 3278, 2355, 172, 9036, 301, 1009, 2903, 8973, 4348, 1814,
    9921, 1608, 2844, 27, 4703, 4419, 7684, 560, 9412, 8717, 7497, 655,
    5752, 1307, 7339, 1376, 3766, 3372, 1330, 5892, 7757, 9292, 1119, 1354,
    950, 3139, 3872, 2116, 5232, 6490, 755, 3362, 8656, 7725, 1542, 9825,
    1943, 6345, 6105, 7829, 9600, 4085, 877, 6332, 6054, 3173, 500, 9950,
    5351, 668, 5165, 3057, 9244, 8203, 6555, 7744, 6937, 2709, 9763, 3946,
    2583, 9807, 318, 187, 9996, 3492, 8814, 2459, 5622, 518, 8799, 3257,
    7678, 837, 5085, 3990, 9352, 2077, 5644, 6263, 5552, 8151, 964, 4160,
    9624, 2778, 3912, 4865, 2151, 7454, 4154, 369, 108, 8602, 6809, 2878,
    5807, 3612, 7916, 8226, 4972, 7397, 8417, 6673, 5390, 7626, 6309, 9792,
    185, 5550, 8912, 9572, 749, 5727, 2474, 8914, 401, 4785, 8708, 1399,
    4833, 1587, 5422, 794, 3631, 7736, 5633, 5561, 390, 3994, 8064, 6278,
    8882, 8891, 6739, 6819, 4137, 1148, 5700, 2067, 7379, 9574, 5813, 573,
    6724, 3189, 9467, 3712, 3008, 6685, 8219, 5278, 385, 662, 5526, 3628,
    3654, 9886, 7591, 6176, 429, 1483, 3564, 7783, 5674, 3460, 3701, 8488,
    3094, 9607, 5365, 2703, 3648, 6083, 7307, 9207, 5263, 305, 1031, 2989,
    7857, 4455, 4478, 1582, 1021, 678, 4043, 6504, 2794, 751, 6474, 6785,
    4343, 8408, 6923, 2947, 1708, 4755, 290, 7120, 9740, 9899, 1754, 6754,
    6774, 359, 4987, 8316, 30, 8275, 4707, 9473, 5010, 5729, 3197, 3500,
    9348, 1777, 9128, 7148, 4306, 1866, 5498, 8908, 2086, 2476, 6253, 6028,
    9617, 9705, 9498, 969, 7812, 2712, 3337, 7568, 8172, 9985, 1798, 1596,
    3573, 1930, 5408, 8776, 6214, 3210, 5570, 9482, 9479, 900, 7640, 9028,
    7799, 3435, 3352, 145, 6602, 1471, 6470, 7577, 3012, 2679, 5243, 228,
    5096, 7198, 9157, 1240, 5124, 7581, 8351, 1494, 1455, 2618, 7968, 1061,
    715, 9561, 6005, 9327, 398, 6501, 3802, 9004, 1263, 3658, 7243, 8789,
    6593, 3216, 5661, 8676, 4014, 1392, 67, 5708, 6713, 9064, 9523, 3477,
    4801, 9514, 7391, 596, 8711, 5619, 105, 772, 2416, 9101, 8659, 7835,
    2047, 9756, 5986, 5783, 8748, 6843, 336, 2546, 1515, 3741, 9862, 498,
    4158, 2065, 143, 5094, 713, 103, 3405, 3969, 601, 5162, 1080, 1882,
    8093, 2526, 8916, 5681, 4471, 1673, 4843, 5455, 4982, 1925, 6573, 4186,
    3325, 956, 6440, 2632, 2838, 7728, 7750, 2025, 2375, 255, 3552, 4018,
    2533, 7693, 4749, 8651, 9153, 3879, 7526, 7597, 903, 2490, 2750, 4003,
    1698, 2175, 2074, 9345, 1336, 240, 7203, 2984, 3925, 1657, 3546, 9974,
    3407, 2762, 6272, 6034, 5718, 7273, 2851, 7923, 6958, 4818, 3526, 7585,
    3851, 6991, 3427, 9464, 7900, 6225, 2057, 6121, 9545, 6761, 8001, 4711,
    6857, 3971, 6267, 403, 9395, 283, 3788, 1936, 8515, 5215, 8373, 1539,
    4000, 1556, 1901, 603, 8309, 507, 3067, 4787, 1717, 3857, 9662, 6136,
    7765, 171, 2902, 1813, 3371, 2115, 7724, 5164, 3056, 7743, 9995, 3989,
    9351, 5643, 6262, 963, 3611, 4784, 5560, 5277, 384, 661, 3627, 6175,
    2702, 1020, 677, 2793, 4706, 3196, 3499, 4305, 8171, 8775, 7639, 6469,
    5242, 9560, 3657, 8788, 5660, 595, 8710, 9100, 5782, 6842, 142, 2525,
    1924, 6572, 3324, 2631, 1335, 7202, 1656, 2761, 6271, 6957, 6224, 8372,
    3856, 1812, 2114, 7742, 5642, 5559, 383, 660, 5781, 659, 3517, 8856,
    7903, 1763, 6228, 3109, 8519, 4461, 9889, 5016, 6421, 9488, 8949, 1401,
    1082, 9946, 3598, 2329, 7165, 4327, 7534, 6801, 4835, 5791, 9359, 3518,
    9091, 8857, 3709, 6920, 8507, 869, 775, 3135, 2915, 623, 4813, 7662,
    5943, 6527, 3673, 7493, 7904, 4484, 9406, 2497, 4808, 6160, 4521, 8842,
    1988, 1884, 5929, 2322, 1764, 5436, 6229, 9198, 3110, 3732, 4165, 8520,
    8422, 3083, 1619, 5799, 9178, 6607, 7958, 7594, 4665, 4050, 4462, 137,
    7544, 8291, 1891, 2540, 212, 111, 3555, 4635, 8992, 4255, 3158, 2669,
    9140, 8681, 2943, 9833, 2342, 4496, 1965, 8095, 2919, 9890, 309, 9872,
    5017, 4216, 3758, 4668, 7643, 222, 3233, 5756, 1772, 7470, 9527, 4913,
    2349, 4882, 4679, 6422, 610, 5589, 433, 2140, 9688, 2528, 2889, 4295,
    167, 9489, 8950, 2972, 1589, 3376, 9071, 3829, 7927, 1402, 1083, 9947,
    2775, 1480, 9869, 5442, 3599, 4053, 2330, 569, 7822, 8605, 4537, 7673,
    7166, 8036, 942, 3983, 4732, 8833, 3395, 4328, 4127, 9812, 3315, 7535,
    5424, 6802, 3805, 8946, 7659, 1616, 4994, 8101, 9774, 796, 6867, 6188,
    4836, 2732, 1040, 6617, 8614, 5792, 201, 9360, 3958, 5138, 998, 5583,
    7346, 4954, 5818, 3816, 5598, 8250, 2371, 2208, 4011, 7819, 7656, 7816,
    5974, 3519, 1780, 4722, 3302, 8138, 8379, 9092, 8464, 2419, 8177, 8858,
    245, 4243, 8918, 2096, 8692, 3465, 4621, 3164, 833, 1190, 299, 3171,
    6737, 3710, 7589, 5261, 6921, 6251, 7566, 2677, 8307, 6173, 4677, 2369,
    2094, 3162, 8508, 3903, 9941, 4358, 4591, 4576, 5977, 6630, 4372, 90,
    7014, 6884, 9422, 870, 9908, 7302, 2447, 3228, 2247, 4407, 2365, 5469,
    776, 5477, 6203, 3136, 4475, 5705, 3922, 2494, 2916, 8925, 6567, 1106,
    461, 2128, 3836, 3401, 624, 5504, 4939, 4814, 7663, 5653, 4234, 5944,
    8133, 6528, 1364, 7881, 8338, 3633, 9031, 8510, 1500, 2561, 419, 4422,
    2005, 3674, 440, 9451, 9875, 5999, 4192, 4700, 7494, 5043, 5445, 2754,
    5299, 9934, 6484, 1315, 7875, 9745, 1559, 7905, 2882, 4485, 5871, 179,
    4648, 3898, 9407, 2498, 4809, 6161, 6463, 6183, 1066, 556, 8554, 9104,
    7955, 4522, 3905, 7414, 8843, 7142, 8662, 1989, 1885, 4391, 2656, 8803,
    5930, 2323, 1277, 5777, 2691, 9649, 7210, 3481, 5133, 7424, 9641, 4997,
    8297, 8622, 2608, 4147, 4963, 9943, 9131, 9721, 6549, 588, 8108, 8704,
    7635, 7157, 1254, 5078, 6291, 5153, 8438, 1765, 8326, 4363, 5185, 5046,
    5437, 4693, 9273, 5683, 5899, 6414, 6051, 3895, 8544, 476, 6230, 4367,
    8878, 7085, 1487, 3016, 9199, 1993, 1136, 3111, 523, 3733, 4166, 4465,
    8427, 6750, 9286, 4614, 1223, 8521, 8423, 4268, 3084, 1620, 7028, 356,
    87, 4360, 5800, 6015, 484, 9964, 8264, 9043, 7687, 5039, 9179, 2864,
    8284, 6608, 7480, 2240, 7738, 5027, 7959, 5173, 5448, 4593, 7313, 5635,
    1606, 9290, 7855, 1594, 3571, 6003, 6499, 3800, 7595, 5716, 2900, 8169,
    5240, 7532, 2667, 4666, 4293, 2773, 4051, 4619, 4473, 7155, 4463, 6009,
    234, 9928, 138, 8533, 723, 7545, 3266, 6654, 8104, 825, 7895, 2930, 99,
    8292, 7826, 5547, 8888, 3826, 7477, 804, 7779, 5563, 4578, 1892, 1675,
    9265, 6068, 2541, 2570, 6458, 1904, 9311, 4917, 5484, 5826, 4935, 4845,
    213, 4739, 363, 7986, 2359, 5803, 3148, 112, 7055, 6282, 6873, 606,
    1396, 4231, 3556, 8847, 7976, 4636, 7194, 4718, 7631, 6394, 5020, 8993,
    3308, 1740, 5203, 4256, 3522, 2219, 2383, 8399, 2288, 9700, 1566, 5959,
    8456, 6011, 5457, 8547, 6767, 3621, 1464, 2314, 6351, 3295, 683, 392,
    3443, 4652, 7222, 1795, 2319, 3159, 3508, 236, 5835, 704, 3248, 8312,
    1292, 1088, 1950, 7062, 3996, 8392, 2670, 931, 5109, 9141, 1844, 4525,
    7419, 8682, 5143, 8185, 6326, 3763, 2944, 9356, 5979, 6689, 6949, 563,
    7701, 77, 9333, 2404, 6878, 3602, 6091, 9834, 9669, 1460, 5355, 4503,
    3908, 9442, 268, 6632, 2343, 8443, 4374, 897, 1695, 3064, 8396, 2648,
    8066, 8233, 4497, 1415, 5219, 6451, 3289, 1966, 7103, 92, 2857, 6733,
    3357, 1848, 1339, 9930, 527, 908, 9682, 3663, 378, 7295, 4984, 3080,
    1841, 8096, 6826, 4599, 4760, 3606, 8580, 9632, 2920, 7622, 6662, 9075,
    1270, 8573, 7937, 617, 9891, 7151, 7792, 533, 310, 3641, 346, 8971,
    1328, 9805, 1585, 2084, 9477, 3214, 2414, 9343, 6989, 6134, 140, 4459,
    9089, 2913, 8840, 9196, 2538, 220, 9686, 940, 2730, 996, 6565, 9873,
    7208, 7853, 9309, 6280, 7974, 5018, 3306, 1738, 9680, 3639, 344, 8535,
    4217, 4556, 3759, 4669, 6583, 8884, 8537, 5289, 4309, 7644, 223, 1170,
    1927, 3234, 3887, 5053, 5310, 479, 8144, 632, 8893, 699, 1545, 8269,
    2909, 5757, 4219, 9676, 6862, 6575, 1436, 9828, 7256, 1773, 7289, 7471,
    725, 9446, 9528, 3811, 280, 6170, 4914, 807, 582, 2350, 1789, 3720,
    648, 4188, 4883, 6101, 3105, 4680, 5103, 6741, 1828, 2736, 7518, 3780,
    2439, 7547, 6423, 611, 3935, 1426, 9778, 2696, 5650, 2686, 9766, 1142,
    2600, 1522, 5062, 7016, 4133, 4339, 1946, 3271, 5372, 7385, 9851, 5822,
    2198, 1152, 130, 3327, 6538, 4056, 5252, 4778, 1645, 5590, 6287, 42,
    7932, 5229, 710, 1536, 3245, 3268, 434, 3416, 4007, 272, 1639, 2141,
    7885, 9689, 4322, 466, 2529, 9415, 4558, 2223, 3391, 6886, 8869, 2890,
    3559, 5982, 1783, 9081, 1981, 4296, 4151, 7965, 168, 4991, 5971, 3932,
    6821, 9490, 7234, 3679, 5068, 5736, 4139, 3237, 1629, 5746, 8951, 2973,
    1590, 5991, 811, 117, 6636, 510, 7848, 8564, 9424, 7177, 3377, 5575,
    3833, 9072, 1735, 3830, 2427, 9007, 2186, 1176, 2451, 5381, 2784, 9553,
    3686, 872, 7928, 6298, 2978, 8079, 4725, 4744, 194, 1403, 2430, 5864,
    1084, 7838, 4199, 3775, 7503, 9948, 2776, 1481, 9983, 6032, 135, 9870,
    8831, 8690, 5443, 2689, 5201, 7060, 3761, 3600, 5287, 1150, 128, 4054,
    1637, 6703, 5765, 3890, 7511, 2331, 4671, 1006, 570, 5702, 7823, 8577,
    7515, 6398, 2069, 7331, 6517, 8606, 9968, 7613, 9596, 8850, 9910, 1506,
    2651, 2515, 6150, 1093, 2409, 4209, 4538, 2133, 9858, 6794, 8928, 6385,
    9366, 7674, 437, 695, 7436, 4029, 1036, 7381, 7167, 958, 4177, 3419,
    3949, 8037, 8016, 8158, 8939, 5519, 943, 7562, 3984, 5056, 6849, 544,
    7796, 8480, 820, 2999, 4733, 6364, 6442, 3042, 8720, 8129, 3411, 156,
    8834, 3396, 9226, 5954, 6656, 6018, 2296, 8483, 1296, 9576, 7351, 5189,
    8223, 5815, 6348, 6580, 2661, 9218, 1234, 7912, 124, 6108, 4329, 785,
    4128, 2050, 4895, 9813, 2393, 3316, 8469, 575, 3381, 1686, 8243, 7536,
    6126, 1748, 3542, 6585, 6320, 9120, 6726, 5425, 1361, 3191, 5579, 2634,
    6642, 6705, 4119, 4849, 3029, 8070, 1666, 975, 6692, 324, 38, 7132,
    410, 9469, 6179, 2840, 9024, 6803, 742, 1024, 3806, 7304, 3514, 5832,
    5226, 8687, 2623, 7730, 4037, 2626, 6832, 9050, 7446, 487, 1013, 1869,
    8237, 1916, 470, 3737, 2338, 628, 9256, 7752, 5890, 334, 5160, 6570,
    8947, 867, 7660, 1617, 4214, 8552, 4995, 8106, 7083, 8102, 823, 7893,
    8886, 6392, 2312, 681, 8142, 2907, 1787, 2449, 9551, 8014, 827, 3714,
    4021, 2586, 3962, 8745, 7897, 9775, 1626, 3002, 9566, 5396, 797, 889,
    4792, 537, 8539, 1109, 2060, 1250, 8026, 6952, 9759, 3979, 5767, 6868,
    1722, 6189, 6112, 8343, 8497, 4968, 829, 4837, 7095, 3005, 3892, 5036,
    2733, 1858, 3532, 1861, 2027, 8585, 8644, 1041, 2377, 8050, 6063, 8771,
    9260, 3221, 3048, 1422, 257, 6618, 7111, 927, 3716, 4071, 8615, 4451,
    4661, 5839, 2790, 3230, 4534, 2674, 9673, 407, 5291, 7024, 3178, 1691,
    7650, 4318, 5793, 9252, 1447, 4311, 202, 9235, 9658, 9361, 6943, 5072,
    990, 6533, 1058, 6798, 5501, 8323, 217, 3959, 50, 4046, 261, 1129,
    7646, 1285, 4284, 9010, 4797, 5139, 9845, 999, 7806, 4629, 2282, 3841,
    2202, 7844, 4106, 883, 3748, 1998, 7707, 6434, 2932, 5313, 5584, 6095,
    61, 4023, 7347, 4955, 5567, 5819, 4206, 2335, 4115, 3817, 8113, 6233,
    4587, 2249, 2588, 4439, 5599, 9231, 2808, 4172, 8251, 1304, 9883, 2372,
    6627, 8922, 3026, 3144, 414, 20, 7279, 1165, 5912, 4409, 5904, 2209,
    6343, 8062, 6081, 1029, 3010, 4012, 101, 2488, 7921, 6119, 3194, 8990,
    3756, 2347, 7820, 9810, 7657, 7817, 2367, 5975, 4370, 9906, 721, 3520,
    3619, 7417, 6687, 2536, 6563, 3885, 5308, 4337, 464, 1781, 4723, 5862,
    7513, 2131, 8221, 2310, 1127, 3839, 2333, 2806, 5280, 8056, 96, 3303,
    3511, 8139, 8795, 7324, 9785, 5935, 8380, 7869, 9093, 450, 3695, 7979,
    5949, 688, 8465, 1159, 2420, 2952, 5879, 1431, 8982, 9322, 3471, 4673,
    8178, 2189, 4685, 2744, 7401, 3070, 8859, 387, 225, 8294, 9021, 7802,
    9769, 8358, 4639, 7183, 246, 9297, 9278, 6401, 4244, 1179, 9391, 644,
    6812, 9111, 2575, 7070, 4945, 4445, 2822, 6369, 3964, 5282, 315, 9896,
    1653, 9866, 9666, 7500, 8126, 8919, 8260, 9402, 7442, 2097, 8693, 3466,
    842, 5670, 1311, 3535, 2716, 5471, 2181, 6377, 2826, 778, 664, 936,
    3771, 8058, 53, 4622, 3165, 1172, 6976, 834, 7733, 3334, 8897, 1191,
    7606, 6042, 6666, 5479, 6205, 5294, 5528, 3277, 300, 1008, 26, 654,
    7338, 3138, 3361, 9824, 7828, 3172, 6936, 2708, 3945, 2458, 8798, 3256,
    5389, 9791, 5549, 8911, 4832, 5421, 793, 3630, 8881, 8890, 6738, 6818,
    5699, 572, 6723, 3711, 7590, 428, 7782, 3093, 9606, 3647, 5262, 4477,
    6922, 6252, 9497, 968, 7567, 1929, 6213, 3351, 1470, 2678, 7197, 8675,
    66, 5707, 5618, 771, 7834, 9755, 8747, 3404, 1881, 8092, 1672, 4185,
    3551, 7525, 239, 3924, 9973, 3426, 7899, 1935, 1555, 1900, 8308, 6261,
    6174, 676, 9559, 3656, 1334, 2760, 3855, 5558, 658, 1762, 9888, 6526,
    7492, 2496, 6159, 4164, 7593, 4049, 1890, 3554, 4495, 2918, 5755, 1771,
    4678, 432, 3828, 8035, 4731, 8100, 9773, 6866, 5582, 7345, 2370, 2207,
    4010, 4721, 8463, 2095, 3163, 298, 6250, 9940, 4357, 9421, 2246, 5704,
    8509, 4699, 5298, 9744, 2881, 555, 3904, 7413, 1276, 9648, 9942, 7634,
    5077, 5152, 8437, 1486, 1222, 7027, 4359, 8263, 7479, 4592, 3570, 5239,
    4618, 98, 7825, 4577, 9264, 2569, 4738, 3147, 8846, 3442, 5978, 6948,
    6090, 4502, 9441, 6631, 4373, 7102, 91, 6825, 8579, 9631, 7791, 8970,
    6988, 995, 7207, 3305, 698, 8268, 6861, 806, 581, 7517, 9777, 2695,
    5649, 2685, 7015, 3415, 6885, 3931, 7233, 3236, 1628, 810, 9423, 2185,
    871, 4743, 6702, 3889, 6397, 9967, 9909, 6149, 9857, 6793, 8927, 6384,
    7435, 5055, 8479, 9119, 4118, 8069, 1665, 323, 37, 6178, 7303, 3513,
    5831, 8236, 627, 6569, 7082, 8141, 2448, 9550, 4020, 4791, 1108, 1249,
    1721, 8342, 8496, 7094, 3004, 5035, 3531, 4450, 5838, 3229, 4283, 7805,
    7843, 5312, 5566, 2248, 4171, 4408, 6342, 6118, 3755, 2366, 720, 2535,
    463, 2130, 2309, 3838, 2805, 5934, 7868, 5878, 8357, 9296, 6400, 1652,
    1310, 5470, 777, 7605, 5478, 6204, 3276, 3137, 8797, 3255, 6817, 7781,
    3646, 4476, 9496, 5706, 3403, 1880, 3923, 1761, 6525, 7491, 2495, 2917,
    431, 8099, 9772, 9939, 4356, 8436, 1485, 4737, 7101, 6824, 8267, 2684,
    8926, 6383, 7434, 8478, 322, 626, 6568, 9549, 4790, 1107, 1720, 8495,
    7842, 5565, 4170, 3754, 462, 2129, 2308, 3837, 3275, 3402, 1760, 6524,
    7490, 4355, 4736, 6382, 321, 625, 6381, 4580, 7326, 9915, 4888, 9568,
    1961, 3566, 4581, 5505, 4940, 3582, 6373, 7327, 1894, 1229, 9916, 2592,
    2071, 4815, 481, 9306, 6514, 6829, 7785, 8822, 2269, 7664, 9173, 5654,
    2397, 4235, 5945, 4889, 8210, 4075, 4602, 8900, 5695, 672, 2637, 9580,
    3343, 8134, 6529, 9017, 5398, 5339, 3794, 8146, 799, 9136, 446, 9787,
    5937, 3953, 7695, 1365, 7333, 9569, 7145, 2522, 1962, 7882, 3942, 3567,
    8339, 891, 4110, 5676, 5023, 6907, 5773, 6304, 6647, 6519, 4582, 3022,
    8608, 5506, 4941, 3634, 3583, 8200, 6221, 3462, 6730, 3242, 4794, 6374,
    7342, 917, 1677, 9371, 9032, 8511, 275, 1501, 9267, 2562, 539, 3438,
    5856, 9730, 3703, 4959, 420, 5376, 2832, 4423, 1209, 2613, 2234, 6070,
    7009, 2543, 8541, 894, 3388, 7328, 817, 5223, 9903, 9970, 634, 7088,
    9203, 2006, 1895, 8490, 1320, 9163, 7161, 6495, 3675, 1230, 441, 9452,
    5995, 5463, 2120, 1408, 8386, 1379, 2554, 9917, 8432, 1111, 3726, 9876,
    190, 2593, 5810, 4040, 2062, 6000, 1055, 2757, 7788, 7615, 8640, 1972,
    4511, 4193, 1384, 3096, 4854, 4701, 7495, 9598, 1864, 9512, 4841, 2072,
    4816, 2629, 4911, 417, 1252, 5044, 482, 5446, 1604, 5714, 3506, 3062,
    3355, 3078, 9307, 6515, 8156, 6124, 6318, 6830, 3617, 4443, 8895, 6211,
    7832, 1553, 2567, 4500, 35, 1247, 1758, 6905, 6302, 6645, 2755, 7786,
    3615, 5300, 2040, 8412, 9935, 4564, 6485, 8736, 5302, 8852, 1316, 8823,
    6708, 6835, 6074, 7463, 2030, 3592, 4122, 7876, 566, 7191, 9655, 3692,
    2572, 2270, 920, 4763, 7665, 9746, 9174, 4261, 9999, 1560, 7906, 6360,
    8761, 5655, 2042, 2398, 2883, 9796, 8121, 4486, 6962, 4249, 4236, 5872,
    9609, 1116, 8414, 6460, 701, 3039, 7704, 5946, 9912, 4890, 5919, 8211,
    4076, 2215, 3452, 2273, 2454, 180, 5491, 4603, 4871, 8901, 2434, 1909,
    1508, 5091, 2022, 4518, 4649, 6131, 8936, 4203, 5696, 673, 1906, 984,
    8502, 5610, 2103, 3899, 8382, 2965, 8028, 1547, 2638, 1575, 7406, 7871,
    7459, 3820, 8725, 6589, 7618, 7946, 9313, 9581, 1072, 3344, 9408, 2766,
    2499, 80, 6145, 3202, 5327, 2034, 4919, 4810, 8135, 5258, 2653, 2605,
    585, 4553, 707, 6530, 9018, 8733, 5399, 9095, 1955, 5925, 6162, 2517,
    5340, 8271, 452, 6464, 5319, 6184, 6995, 3795, 5486, 8147, 800, 5367,
    6152, 4278, 1067, 8361, 5384, 8041, 557, 9241, 9061, 6758, 9137, 2897,
    5050, 9980, 7129, 447, 9788, 5828, 5916, 1836, 5603, 5938, 8996, 3954,
    1266, 8555, 2255, 6746, 1876, 7696, 9105, 4380, 1366, 9336, 9046, 7334,
    4863, 9570, 3699, 7146, 5496, 3208, 9993, 3609, 2523, 7956, 1963, 2138,
    7671, 8944, 1614, 5596, 4937, 7777, 4523, 6324, 9331, 3906, 3661, 1839,
    531, 9341, 2911, 3103, 7883, 5989, 4197, 2407, 4847, 4532, 215, 4113,
    1163, 7415, 6367, 3943, 426, 553, 3568, 8844, 4741, 8340, 9937, 3341,
    7143, 892, 5461, 4852, 9653, 2101, 3697, 4111, 551, 9457, 7981, 8447,
    8663, 637, 8669, 4751, 923, 9459, 6408, 7717, 5677, 264, 8957, 2969,
    458, 1990, 5024, 7983, 8076, 5951, 987, 295, 9493, 1194, 5211, 1886,
    8977, 6908, 1184, 1095, 4392, 2657, 8804, 3311, 6622, 4566, 6487, 8653,
    6954, 5774, 2787, 9821, 5931, 9058, 6023, 9183, 1819, 8449, 365, 2387,
    9, 2324, 6427, 148, 2089, 4434, 5759, 3577, 9539, 1278, 4544, 7988,
    1388, 3262, 7609, 7558, 7374, 160, 8588, 6305, 3495, 9520, 5778, 7108,
    2692, 7431, 9650, 6648, 8647, 6520, 7237, 9385, 4572, 6559, 6045, 3669,
    7211, 3482, 5134, 7285, 7425, 4143, 4583, 9642, 3125, 3860, 2411, 4211,
    3023, 2705, 5537, 4067, 8665, 8738, 8609, 4998, 1680, 3867, 8298, 8623,
    1123, 2609, 4490, 5304, 1044, 5507, 6896, 3650, 4221, 71, 5606, 4540,
    1145, 3299, 375, 4148, 641, 4942, 23, 2361, 3635, 3584, 5432, 4878,
    4964, 690, 4766, 8201, 9761, 6807, 5805, 9155, 6438, 7748, 6222, 5640,
    8854, 9944, 8505, 5941, 210, 3981, 199, 4952, 3463, 7300, 5467, 7953,
    5183, 6731, 1326, 9678, 5060, 9849, 3243, 5734, 2513, 2997, 8467, 887,
    8583, 4795, 18, 6375, 3332, 6040, 791, 6864, 7343, 5771, 2830, 1318,
    918, 982, 3206, 1161, 7715, 9056, 1678, 639, 5430, 5769, 2868, 2422,
    3181, 8192, 9372, 5723, 9124, 862, 2936, 8825, 2954, 2870, 5402, 9033,
    3489, 7117, 6710, 252, 8512, 4645, 6455, 6870, 1289, 2424, 9593, 2135,
    7282, 2510, 4642, 2303, 276, 1502, 9132, 9722, 9268, 3881, 2563, 4093,
    8116, 6550, 1132, 9532, 1724, 8697, 3150, 2958, 540, 6966, 7246, 6200,
    2485, 6720, 3439, 589, 8671, 5881, 5965, 6191, 5857, 7186, 6837, 9635,
    856, 9731, 8109, 3032, 7051, 4089, 852, 6076, 6236, 3704, 8705, 7690,
    7636, 3132, 6881, 4960, 4265, 1433, 114, 7057, 6577, 7158, 421, 5377,
    1255, 5079, 6292, 6931, 6679, 2011, 1438, 7528, 2833, 5154, 4424, 1823,
    8439, 8817, 1210, 3183, 1766, 8984, 8327, 9377, 760, 7363, 9324, 9830,
    4364, 2664, 6284, 5186, 7439, 5047, 8073, 2462, 7465, 4272, 1197, 6085,
    5613, 8765, 2614, 7309, 8809, 9209, 2235, 6669, 5438, 6892, 9714, 8194,
    5005, 4694, 7599, 7258, 9274, 9221, 5684, 2861, 1642, 1003, 8011, 9628,
    7230, 7091, 6071, 8933, 1572, 9374, 2470, 2017, 2166, 7010, 5900, 2479,
    6415, 5265, 3473, 3251, 5625, 2353, 6052, 5725, 3458, 4753, 6772, 1775,
    9126, 7575, 1492, 2544, 9860, 4469, 9098, 4482, 307, 4675, 3226, 3896,
    8542, 8876, 8545, 6765, 895, 477, 1520, 3389, 7329, 818, 5224, 925,
    6796, 6231, 7919, 4368, 9904, 7322, 2742, 8879, 9971, 9419, 6986, 3386,
    7086, 2032, 2603, 5317, 424, 635, 9054, 2483, 5003, 7089, 6507, 9318,
    3089, 9621, 746, 9204, 9461, 2927, 2380, 8742, 1958, 7668, 3100, 249,
    2300, 3594, 3682, 767, 2801, 2007, 2812, 5629, 5169, 1204, 7291, 1896,
    8491, 1488, 1321, 9164, 5415, 3017, 7162, 9961, 6496, 905, 3676, 1033,
    1231, 864, 5692, 9200, 8930, 6410, 1994, 2230, 9749, 6387, 8180, 7473,
    8560, 1137, 5332, 3112, 6114, 7552, 4032, 442, 6509, 5786, 9453, 8345,
    5523, 5996, 6875, 524, 3734, 1156, 9399, 9556, 8032, 4167, 5464, 4466,
    2465, 1731, 730, 2121, 5031, 3918, 8428, 5532, 2991, 1409, 1418, 7719,
    8387, 7037, 3320, 8165, 1380, 2555, 735, 9918, 6751, 4805, 9287, 6651,
    8053, 4615, 8433, 6315, 3689, 8499, 7950, 727, 493, 7941, 8751, 4689,
    1224, 7859, 6445, 2191, 6246, 8473, 1112, 3727, 4100, 9877, 8522, 9508,
    2938, 1237, 592, 9448, 8424, 1792, 8866, 191, 9368, 3723, 4269, 5270,
    2161, 2594, 3085, 2106, 1621, 7251, 8827, 7029, 1941, 875, 7676, 8600,
    4970, 6276, 5811, 4041, 288, 357, 6026, 496, 2063, 5679, 2748, 2055,
    6419, 6605, 4253, 7468, 608, 243, 831, 88, 2492, 2752, 177, 4361, 521,
    9284, 6001, 6066, 5482, 5801, 266, 4457, 1168, 4005, 6847, 6016, 4893,
    485, 1056, 8988, 8793, 9783, 9320, 2820, 5668, 3769, 3091, 2758, 9646,
    9439, 7789, 9965, 8265, 3752, 2306, 9728, 4839, 7616, 1834, 9044, 6038,
    9530, 2956, 7688, 2468, 4480, 2799, 4687, 9506, 2746, 2797, 8369, 4124,
    7878, 5040, 9718, 353, 8641, 651, 9603, 4352, 1052, 6208, 7403, 8730,
    9180, 2865, 8022, 1700, 2722, 8999, 8285, 9301, 8229, 4771, 57, 8635,
    1973, 4413, 1525, 7097, 6609, 3072, 9432, 7481, 4512, 4931, 8256, 2872,
    1807, 2241, 7739, 3813, 4388, 8089, 4194, 1385, 3097, 5028, 5540, 7268,
    7960, 5174, 8861, 5449, 8959, 5273, 1744, 2177, 4855, 5404, 6166, 9695,
    4594, 1474, 7314, 4975, 8905, 5887, 9536, 4097, 2110, 8529, 5543, 5636,
    6927, 2923, 6256, 9186, 9035, 1607, 4702, 8716, 7496, 7756, 9291, 754,
    9599, 3491, 8813, 2076, 9623, 7453, 7915, 7625, 184, 748, 2473, 1398,
    389, 7378, 3007, 6684, 5364, 9206, 7856, 6473, 7119, 8315, 9347, 1865,
    9616, 1595, 3572, 6601, 227, 7580, 6004, 6500, 3801, 6712, 9513, 2046,
    1514, 1079, 4842, 4981, 2837, 254, 8650, 7596, 2073, 5717, 4817, 9463,
    282, 3787, 8514, 5214, 1716, 2901, 3370, 7723, 1019, 4304, 8170, 8774,
    5241, 2630, 8371, 2113, 3597, 7533, 622, 9405, 5928, 3731, 9177, 7543,
    2668, 4667, 4912, 5588, 4294, 166, 2971, 3375, 9070, 2774, 4052, 568,
    4126, 1039, 6616, 8613, 3957, 4620, 1189, 3170, 6172, 3902, 4590, 2446,
    5476, 4474, 1105, 460, 2127, 4233, 7880, 418, 439, 4191, 5042, 4647,
    8802, 9640, 8296, 8621, 9720, 7156, 1253, 5045, 6413, 3894, 3015, 1992,
    4464, 355, 86, 483, 5038, 8283, 5026, 5447, 1605, 5715, 8532, 2929,
    5546, 6457, 4916, 7985, 6872, 7193, 2382, 2287, 6010, 7221, 3507, 235,
    1291, 7700, 76, 1694, 3063, 3356, 1338, 9929, 3079, 3605, 1269, 616,
    1584, 139, 9308, 343, 8534, 724, 279, 5102, 2735, 3779, 2438, 7546,
    1141, 5371, 7384, 7931, 3267, 3558, 1980, 5067, 5745, 5574, 1734, 2426,
    5380, 2783, 3685, 8078, 6031, 134, 127, 6516, 9595, 8849, 1505, 1092,
    694, 4176, 8157, 155, 9225, 5953, 6655, 1295, 9217, 7911, 2392, 1685,
    6125, 6319, 9023, 1023, 6831, 7445, 8551, 8105, 680, 826, 8744, 7896,
    5395, 8025, 6111, 1857, 1860, 8643, 8770, 4070, 4660, 7649, 9657, 5071,
    989, 49, 4045, 2281, 3747, 1997, 6433, 2931, 6094, 6626, 7278, 6080,
    1028, 100, 3618, 3694, 7978, 687, 2951, 8981, 3470, 4684, 7400, 8293,
    4638, 9110, 2574, 4444, 314, 9665, 3534, 8896, 6665, 653, 7827, 5388,
    5548, 8889, 9605, 6212, 3350, 7196, 8674, 770, 7833, 1671, 1934, 1554,
    1333, 1889, 3827, 4720, 297, 5297, 7633, 7478, 2568, 4501, 994, 7206,
    697, 805, 809, 6396, 1664, 36, 1248, 7804, 2804, 8356, 7604, 7780,
    3645, 9495, 9771, 9548, 5564, 1759, 6523, 4354, 4579, 1960, 1893, 1228,
    6513, 8821, 9135, 2521, 5022, 6906, 6303, 6646, 8199, 3241, 916, 1676,
    9266, 1208, 2233, 6069, 2542, 816, 5222, 9162, 8385, 2553, 1054, 2756,
    7787, 8639, 1863, 4910, 3505, 3061, 3616, 6210, 6301, 5301, 2029, 2571,
    6359, 8760, 2041, 8413, 6459, 3451, 2272, 2021, 1905, 7405, 7945, 9312,
    4918, 584, 8732, 5485, 8360, 9240, 5827, 5915, 8995, 2137, 7670, 4936,
    3102, 4846, 214, 4740, 9936, 2100, 8668, 922, 2968, 3310, 4565, 6486,
    9182, 1818, 364, 7987, 3261, 8587, 8646, 3668, 7284, 4066, 8737, 5303,
    1043, 2360, 4765, 5804, 5639, 8853, 1325, 2512, 3331, 1317, 2867, 8824,
    3488, 6709, 251, 4644, 6454, 1288, 4641, 2302, 8696, 3149, 5964, 7185,
    6836, 4088, 6075, 4264, 113, 7056, 6930, 2010, 1822, 6283, 7438, 7464,
    1196, 8808, 8010, 2165, 2352, 6771, 6764, 2741, 2031, 9053, 5002, 6506,
    9317, 9620, 2926, 2379, 8741, 7667, 248, 3593, 9960, 9748, 4031, 6874,
    8031, 5531, 8052, 6314, 1791, 8865, 3722, 287, 607, 242, 176, 6065,
    6846, 8792, 9438, 2305, 9727, 9505, 2796, 4123, 7877, 650, 9300, 4770,
    4412, 9431, 8255, 1806, 8088, 1743, 6165, 5886, 753, 1397, 6600, 1078,
    5213, 8773, 3596, 621, 9176, 567, 1038, 6615, 8612, 1104, 4232, 4190,
    9639, 8620, 7192, 7220, 615, 342, 278, 7383, 3557, 5066, 2782, 3684,
    8848, 1504, 1091, 154, 2391, 1684, 8024, 9656, 48, 3693, 7977, 8980,
    3469, 4637, 2573, 7195, 769, 1670, 1888, 4719, 7632, 7205, 6395, 2803,
    8355, 9134, 2520, 5021, 815, 8759, 3450, 2271, 2020, 7944, 8994, 921,
    3309, 3260, 3667, 4065, 4764, 4263, 2009, 8009, 2164, 5001, 7666, 9747,
    9299, 1742, 5885, 6599, 9175, 6614, 5065, 153, 1683, 8979, 1669, 4262,
    8008, 1741, 5884, 6598, 6613, 6597};

    long* sa = suffix_array(t);
    
    for (i = 0; i < 10001; i++) {
        if(sa[i] != sa_valid[i]) {
	    printf("expected %ld, got %ld\n", sa_valid[i], sa[i]);
            test_pass = 0;
        }
    }

    printf("sa_test.c :: suffix_array_from_file_test(): %d\n", test_pass);

    free(sa);
}

void suffix_array_from_file_test2() {
    int test_pass = 1;
    
    int i;

    // TODO try with really large files.
    char* in_path = "data/input_test_file3.txt";
    long* t = read_file(in_path);

    long sa_valid[] = { 11, 7, 3, 0, 9, 5, 10, 6, 8, 4, 2, 1 };

    long* sa = suffix_array(t);
    
    for (i = 0; i < 12; i++) {
	if (sa[i] != sa_valid[i]) {
	    test_pass = 0;
	    printf("got %ld, expected %ld\n", sa[i], sa_valid[i]);
	}
    }

    printf("sa_test.c :: suffix_array_from_file_test2(): %d\n", test_pass);

    free(sa);
}
