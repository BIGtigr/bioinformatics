#include<stdio.h>

main()
{
    char T[] = {'B', 'I', 'O', 'I', 'N', 'F', 'O', 'R', 'M', 'A', 'T', 'I', 'K', 'A'};

    // Call function which creates suffix-array from T[]
    // sa = ...
    int sa[] = {14, 13, 9, 0, 5, 11, 3, 1, 12, 8, 4, 2, 6, 7, 10};

    // expected result
    int res[] = {14, 13, 9, 0, 5, 11, 3, 1, 12, 8, 4, 2, 6, 7, 10};

    int i;
    int n = sizeof(res) / sizeof(int);

    int test_pass = 1;
    for (i = 0; i < n; ++i) {
        if (sa[i] != res[i]) {
            test_pass = 0;
        }
    }

    printf("Suffix_array_test: %d\n", test_pass);
}