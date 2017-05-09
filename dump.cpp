/*
 * Copyright (C) 2017 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <stdlib.h>

#include <string>

#include "dump.h"

bool g_dump_all = false;
bool g_dump_fec_header = false;
bool g_dump_fec_verity = false;
bool g_dump_fec_ecc = false;
bool g_dump_metadata_header = false;

std::string str2hex(const uint8_t *str, size_t len)
{
    const char* hex = "0123456789abcdef";
    std::string result = "";
    for (size_t i = 0; i < len; ++i) {
        result.push_back(hex[(str[i]>>4) & 0xf]);
        result.push_back(hex[str[i] & 0xf]);
    }
    return result;
}

void dump_fec_verity_metadata(struct fec_verity_metadata *verity)
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
    printf("  table length: %u\n", verity->table_length);
}

void dump_fec_ecc_metadata(struct fec_ecc_metadata *ecc)
{
    printf("\nfec ecc metadata:\n");
    printf("  valid: %s\n", ecc->valid ? "true" : "false");
    printf("  roots: %u\n", ecc->roots);
    printf("  blocks: %lu\n", ecc->blocks);
    printf("  rounds: %lu\n", ecc->rounds);
    printf("  start: %lu\n", ecc->start);
}

void dump_fec_header(fec_header *header)
{
    printf("\nfec header:\n");
    printf("  magic: 0x%x\n", header->magic);
    printf("  version: %d\n", header->version);
    printf("  size: %d\n", header->size);
    printf("  roots: %d\n", header->roots);
    printf("  fec_size: %d\n", header->fec_size);
    printf("  inp_size: %ld\n", header->inp_size);
    printf("  hash: 0x%s\n",
           str2hex(header->hash, strlen((const char*)header->hash)).c_str());
}

void dump_metadata_header(verity_header *header, const char *type)
{
    printf("\nandroid metadata header (%s):\n", type);
    printf("  magic: 0x%x\n", header->magic);
    printf("  version: %u\n", header->version);
    printf("  signature: 0x%s\n",
           str2hex(header->signature,
                   strlen((const char*)header->signature)).c_str());
    printf("  length: %u\n", header->length);
}
