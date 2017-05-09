#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include <string>

#include <fcntl.h>
#include <unistd.h>

#include <fec/io.h>

#include "dump.h"

struct verity_table_params {
    char *table;
    int mode;
    struct fec_ecc_metadata ecc;
    const char *ecc_dev;
};

static void usage(void)
{
    printf("usage: dv [ <option> ] <block device>\n"
           "\n"
           "options:\n"
           "  -h, --help        show this message.\n"
           );
}

int main(int argc, char *argv[])
{
    int longindex;
    const char *block_device = NULL;
    struct fec_handle *f = NULL;
    struct fec_verity_metadata verity;
    struct verity_table_params params = { .table = NULL };

    const struct option longopts[] = {
        {"all", no_argument, 0, 'a'},
        {"fec_ecc", no_argument, 0, 'e'},
        {"fec_header", no_argument, 0, 'f'},
        {"help", no_argument, 0, 'h'},
        {"metadata_header", no_argument, 0, 'm'},
        {"fec_verity", no_argument, 0, 'v'},
        {0, 0, 0, 0},
    };

    while (1) {
        int c = getopt_long(argc, argv, "aefhmv", longopts, &longindex);
        if (c < 0) {
            break;
        }

        switch (c) {
            case 'a':
                g_dump_all = true;
                break;
            case 'e':
                g_dump_fec_ecc = true;
                break;
            case 'f':
                g_dump_fec_header = true;
                break;
            case 'h':
                usage();
                return 0;
            case 'm':
                g_dump_metadata_header = true;
                break;
            case 'v':
                g_dump_fec_verity = true;
                break;
            default:
                abort();
        }
    }

    argc -= optind;
    argv += optind;

    if (argc == 0) {
        usage();
        return 1;
    }

    block_device = *argv;

    if (fec_open(&f, block_device, O_RDONLY, FEC_VERITY_DISABLE,
                 FEC_DEFAULT_ROOTS) < 0) {
        fprintf(stderr, "Failed to open '%s': %s\n",
                block_device, strerror(errno));
        return 1;
    }

    if (fec_verity_get_metadata(f, &verity) < 0) {
        fprintf(stderr, "Failed to get verity metadata '%s': %s\n",
                block_device, strerror(errno));
        return 1;
    }

    if (g_dump_fec_verity || g_dump_all) {
        dump_fec_verity_metadata(&verity);
    }

    if (fec_ecc_get_metadata(f, &params.ecc) < 0) {
        params.ecc.valid = false;
    }

    if (g_dump_fec_ecc || g_dump_all) {
        dump_fec_ecc_metadata(&params.ecc);
    }

    params.ecc_dev = block_device;

    return 0;
}
