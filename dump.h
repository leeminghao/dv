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

#ifndef __DV_DUMP_H__
#define __DV_DUMP_H__

#include <fec/io.h>

#include "fec_private.h"

#ifdef __cplusplus
extern "C" {
#endif

extern bool g_dump_all;
extern bool g_dump_fec_header;
extern bool g_dump_fec_verity;
extern bool g_dump_fec_ecc;
extern bool g_dump_metadata_header;

void dump_fec_header(fec_header *header);
void dump_fec_verity_metadata(struct fec_verity_metadata *verity);
void dump_fec_ecc_metadata(struct fec_ecc_metadata *ecc);
void dump_metadata_header(verity_header *header, const char *type);

#ifdef __cplusplus
}
#endif

#endif /* __DV_DUMP__ */
