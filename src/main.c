#include <stdio.h>

#include "src.h"
#include "util.h"

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Invalid number of input parameters. Expecting path to input file and path to output file.\n");
    }

    char* in_path = argv[1];
    char* out_path = argv[2];
    struct long_suite* ls = read_file_suite(in_path);
    long* sa = suffix_array(ls->t);
    long* bwt = burrows_wheeler_transform(ls->t, sa, ls->length);

    write_file(bwt, ls->length, out_path);

    return 0;
}
