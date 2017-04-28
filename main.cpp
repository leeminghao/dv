#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include <string>

#include <fcntl.h>
#include <unistd.h>

#include <fec/io.h>

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

static std::string str2hex(const uint8_t* str, size_t len)
{
    const char* hex = "0123456789abcdef";
    std::string result = "";
    for (size_t i = 0; i < len; ++i) {
        result.push_back(hex[(str[i]>>4) & 0xf]);
        result.push_back(hex[str[i] & 0xf]);
    }
    return result;
}

static void dump_fec_verity_metadata(struct fec_verity_metadata *verity)
{
    printf("\nfec verity metadata:\n");
    printf("  diabled: %s\n", verity->disabled ? "true" : "false");
    printf("  data size: %lu\n", verity->data_size);
    printf("  signature: 0x%s\n",
           str2hex(verity->signature,
                   strlen((const char*)verity->signature)).c_str());
    printf("  ecc signature: 0x%s\n",
           str2hex(verity->ecc_signature,
                   strlen((const char*)verity->ecc_signature)).c_str());
    printf("  table: %s\n", verity->table);
    printf("  table length: %u\n\n", verity->table_length);
}

static void dump_fec_ecc_metadata(struct fec_ecc_metadata *ecc)
{
    printf("\nfec ecc metadata:\n");
    printf("  valid: %s\n", ecc->valid ? "true" : "false");
    printf("  roots: %u\n", ecc->roots);
    printf("  blocks: %lu\n", ecc->blocks);
    printf("  rounds: %lu\n", ecc->rounds);
    printf("  start: %lu\n\n", ecc->start);
}

int main(int argc, char *argv[])
{
    int longindex;
    bool dump_all = false;
    bool dump_fec_verity = false;
    bool dump_fec_ecc = false;
    const char *block_device = NULL;
    struct fec_handle *f = NULL;
    struct fec_verity_metadata verity;
    struct verity_table_params params = { .table = NULL };

    const struct option longopts[] = {
        {"all", no_argument, 0, 'a'},
        {"fec_ecc", no_argument, 0, 'e'},
        {"fec_verity", no_argument, 0, 'v'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0},
    };

    while (1) {
        int c = getopt_long(argc, argv, "aehv", longopts, &longindex);
        if (c < 0) {
            break;
        }

        switch (c) {
            case 'a':
                dump_all = true;
                break;
            case 'e':
                dump_fec_ecc = true;
                break;
            case 'h':
                usage();
                return 0;
            case 'v':
                dump_fec_verity = true;
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

    if (dump_fec_verity || dump_all) {
        dump_fec_verity_metadata(&verity);
    }

    if (fec_ecc_get_metadata(f, &params.ecc) < 0) {
        params.ecc.valid = false;
    }

    if (dump_fec_ecc || dump_all) {
        dump_fec_ecc_metadata(&params.ecc);
    }

    params.ecc_dev = block_device;

    return 0;
}
