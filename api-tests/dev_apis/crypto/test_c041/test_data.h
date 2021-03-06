/** @file
 * Copyright (c) 2019, Arm Limited or its affiliates. All rights reserved.
 * SPDX-License-Identifier : Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
**/

#include "val_crypto.h"

typedef struct {
    char                    test_desc[75];
    psa_key_handle_t        key_handle;
    psa_key_type_t          key_type;
    uint8_t                 key_data[16];
    uint32_t                key_length;
    psa_key_usage_t         usage;
    psa_algorithm_t         key_alg;
    uint8_t                 input[32];
    size_t                  input_length;
    size_t                  signature_size;
    uint8_t                 expected_signature[128];
    size_t                  expected_signature_length;
    size_t                  expected_bit_length;
    psa_status_t            expected_status;
} test_data;

static const uint8_t rsa_384_keypair[1];
static const uint8_t rsa_384_keydata[1];
static const uint8_t rsa_256_keypair[1];
static const uint8_t rsa_256_keydata[1];
static const uint8_t ec_keydata[1];

static const uint8_t ec_keypair[] = {
 0xab, 0x45, 0x43, 0x57, 0x12, 0x64,
 0x9c, 0xb3, 0x0b, 0xbd, 0xda, 0xc4, 0x91, 0x97, 0xee, 0xbf, 0x27, 0x40, 0xff, 0xc7,
 0xf8, 0x74, 0xd9, 0x24, 0x4c, 0x34, 0x60, 0xf5, 0x4f, 0x32, 0x2d, 0x3a};

static const uint8_t rsa_128_keydata[] = {
 0x30, 0x81, 0x9f, 0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d,
 0x01, 0x01, 0x01, 0x05, 0x00, 0x03, 0x81, 0x8d, 0x00, 0x30, 0x81, 0x89, 0x02,
 0x81, 0x81, 0x00, 0xaf, 0x05, 0x7d, 0x39, 0x6e, 0xe8, 0x4f, 0xb7, 0x5f, 0xdb,
 0xb5, 0xc2, 0xb1, 0x3c, 0x7f, 0xe5, 0xa6, 0x54, 0xaa, 0x8a, 0xa2, 0x47, 0x0b,
 0x54, 0x1e, 0xe1, 0xfe, 0xb0, 0xb1, 0x2d, 0x25, 0xc7, 0x97, 0x11, 0x53, 0x12,
 0x49, 0xe1, 0x12, 0x96, 0x28, 0x04, 0x2d, 0xbb, 0xb6, 0xc1, 0x20, 0xd1, 0x44,
 0x35, 0x24, 0xef, 0x4c, 0x0e, 0x6e, 0x1d, 0x89, 0x56, 0xee, 0xb2, 0x07, 0x7a,
 0xf1, 0x23, 0x49, 0xdd, 0xee, 0xe5, 0x44, 0x83, 0xbc, 0x06, 0xc2, 0xc6, 0x19,
 0x48, 0xcd, 0x02, 0xb2, 0x02, 0xe7, 0x96, 0xae, 0xbd, 0x94, 0xd3, 0xa7, 0xcb,
 0xf8, 0x59, 0xc2, 0xc1, 0x81, 0x9c, 0x32, 0x4c, 0xb8, 0x2b, 0x9c, 0xd3, 0x4e,
 0xde, 0x26, 0x3a, 0x2a, 0xbf, 0xfe, 0x47, 0x33, 0xf0, 0x77, 0x86, 0x9e, 0x86,
 0x60, 0xf7, 0xd6, 0x83, 0x4d, 0xa5, 0x3d, 0x69, 0x0e, 0xf7, 0x98, 0x5f, 0x6b,
 0xc3, 0x02, 0x03, 0x01, 0x00, 0x01};

static const uint8_t rsa_128_keypair[] = {
0x30, 0x82, 0x02, 0x5e, 0x02, 0x01, 0x00, 0x02, 0x81, 0x81, 0x00, 0xaf, 0x05,
0x7d, 0x39, 0x6e, 0xe8, 0x4f, 0xb7, 0x5f, 0xdb, 0xb5, 0xc2, 0xb1, 0x3c, 0x7f,
0xe5, 0xa6, 0x54, 0xaa, 0x8a, 0xa2, 0x47, 0x0b, 0x54, 0x1e, 0xe1, 0xfe, 0xb0,
0xb1, 0x2d, 0x25, 0xc7, 0x97, 0x11, 0x53, 0x12, 0x49, 0xe1, 0x12, 0x96, 0x28,
0x04, 0x2d, 0xbb, 0xb6, 0xc1, 0x20, 0xd1, 0x44, 0x35, 0x24, 0xef, 0x4c, 0x0e,
0x6e, 0x1d, 0x89, 0x56, 0xee, 0xb2, 0x07, 0x7a, 0xf1, 0x23, 0x49, 0xdd, 0xee,
0xe5, 0x44, 0x83, 0xbc, 0x06, 0xc2, 0xc6, 0x19, 0x48, 0xcd, 0x02, 0xb2, 0x02,
0xe7, 0x96, 0xae, 0xbd, 0x94, 0xd3, 0xa7, 0xcb, 0xf8, 0x59, 0xc2, 0xc1, 0x81,
0x9c, 0x32, 0x4c, 0xb8, 0x2b, 0x9c, 0xd3, 0x4e, 0xde, 0x26, 0x3a, 0x2a, 0xbf,
0xfe, 0x47, 0x33, 0xf0, 0x77, 0x86, 0x9e, 0x86, 0x60, 0xf7, 0xd6, 0x83, 0x4d,
0xa5, 0x3d, 0x69, 0x0e, 0xf7, 0x98, 0x5f, 0x6b, 0xc3, 0x02, 0x03, 0x01, 0x00,
0x01, 0x02, 0x81, 0x81, 0x00, 0x87, 0x4b, 0xf0, 0xff, 0xc2, 0xf2, 0xa7, 0x1d,
0x14, 0x67, 0x1d, 0xdd, 0x01, 0x71, 0xc9, 0x54, 0xd7, 0xfd, 0xbf, 0x50, 0x28,
0x1e, 0x4f, 0x6d, 0x99, 0xea, 0x0e, 0x1e, 0xbc, 0xf8, 0x2f, 0xaa, 0x58, 0xe7,
0xb5, 0x95, 0xff, 0xb2, 0x93, 0xd1, 0xab, 0xe1, 0x7f, 0x11, 0x0b, 0x37, 0xc4,
0x8c, 0xc0, 0xf3, 0x6c, 0x37, 0xe8, 0x4d, 0x87, 0x66, 0x21, 0xd3, 0x27, 0xf6,
0x4b, 0xbe, 0x08, 0x45, 0x7d, 0x3e, 0xc4, 0x09, 0x8b, 0xa2, 0xfa, 0x0a, 0x31,
0x9f, 0xba, 0x41, 0x1c, 0x28, 0x41, 0xed, 0x7b, 0xe8, 0x31, 0x96, 0xa8, 0xcd,
0xf9, 0xda, 0xa5, 0xd0, 0x06, 0x94, 0xbc, 0x33, 0x5f, 0xc4, 0xc3, 0x22, 0x17,
0xfe, 0x04, 0x88, 0xbc, 0xe9, 0xcb, 0x72, 0x02, 0xe5, 0x94, 0x68, 0xb1, 0xea,
0xd1, 0x19, 0x00, 0x04, 0x77, 0xdb, 0x2c, 0xa7, 0x97, 0xfa, 0xc1, 0x9e, 0xda,
0x3f, 0x58, 0xc1, 0x02, 0x41, 0x00, 0xe2, 0xab, 0x76, 0x08, 0x41, 0xbb, 0x9d,
0x30, 0xa8, 0x1d, 0x22, 0x2d, 0xe1, 0xeb, 0x73, 0x81, 0xd8, 0x22, 0x14, 0x40,
0x7f, 0x1b, 0x97, 0x5c, 0xbb, 0xfe, 0x4e, 0x1a, 0x94, 0x67, 0xfd, 0x98, 0xad,
0xbd, 0x78, 0xf6, 0x07, 0x83, 0x6c, 0xa5, 0xbe, 0x19, 0x28, 0xb9, 0xd1, 0x60,
0xd9, 0x7f, 0xd4, 0x5c, 0x12, 0xd6, 0xb5, 0x2e, 0x2c, 0x98, 0x71, 0xa1, 0x74,
0xc6, 0x6b, 0x48, 0x81, 0x13, 0x02, 0x41, 0x00, 0xc5, 0xab, 0x27, 0x60, 0x21,
0x59, 0xae, 0x7d, 0x6f, 0x20, 0xc3, 0xc2, 0xee, 0x85, 0x1e, 0x46, 0xdc, 0x11,
0x2e, 0x68, 0x9e, 0x28, 0xd5, 0xfc, 0xbb, 0xf9, 0x90, 0xa9, 0x9e, 0xf8, 0xa9,
0x0b, 0x8b, 0xb4, 0x4f, 0xd3, 0x64, 0x67, 0xe7, 0xfc, 0x17, 0x89, 0xce, 0xb6,
0x63, 0xab, 0xda, 0x33, 0x86, 0x52, 0xc3, 0xc7, 0x3f, 0x11, 0x17, 0x74, 0x90,
0x2e, 0x84, 0x05, 0x65, 0x92, 0x70, 0x91, 0x02, 0x41, 0x00, 0xb6, 0xcd, 0xbd,
0x35, 0x4f, 0x7d, 0xf5, 0x79, 0xa6, 0x3b, 0x48, 0xb3, 0x64, 0x3e, 0x35, 0x3b,
0x84, 0x89, 0x87, 0x77, 0xb4, 0x8b, 0x15, 0xf9, 0x4e, 0x0b, 0xfc, 0x05, 0x67,
0xa6, 0xae, 0x59, 0x11, 0xd5, 0x7a, 0xd6, 0x40, 0x9c, 0xf7, 0x64, 0x7b, 0xf9,
0x62, 0x64, 0xe9, 0xbd, 0x87, 0xeb, 0x95, 0xe2, 0x63, 0xb7, 0x11, 0x0b, 0x9a,
0x1f, 0x9f, 0x94, 0xac, 0xce, 0xd0, 0xfa, 0xfa, 0x4d, 0x02, 0x40, 0x71, 0x19,
0x5e, 0xec, 0x37, 0xe8, 0xd2, 0x57, 0xde, 0xcf, 0xc6, 0x72, 0xb0, 0x7a, 0xe6,
0x39, 0xf1, 0x0c, 0xbb, 0x9b, 0x0c, 0x73, 0x9d, 0x0c, 0x80, 0x99, 0x68, 0xd6,
0x44, 0xa9, 0x4e, 0x3f, 0xd6, 0xed, 0x92, 0x87, 0x07, 0x7a, 0x14, 0x58, 0x3f,
0x37, 0x90, 0x58, 0xf7, 0x6a, 0x8a, 0xec, 0xd4, 0x3c, 0x62, 0xdc, 0x8c, 0x0f,
0x41, 0x76, 0x66, 0x50, 0xd7, 0x25, 0x27, 0x5a, 0xc4, 0xa1, 0x02, 0x41, 0x00,
0xbb, 0x32, 0xd1, 0x33, 0xed, 0xc2, 0xe0, 0x48, 0xd4, 0x63, 0x38, 0x8b, 0x7b,
0xe9, 0xcb, 0x4b, 0xe2, 0x9f, 0x4b, 0x62, 0x50, 0xbe, 0x60, 0x3e, 0x70, 0xe3,
0x64, 0x75, 0x01, 0xc9, 0x7d, 0xdd, 0xe2, 0x0a, 0x4e, 0x71, 0xbe, 0x95, 0xfd,
0x5e, 0x71, 0x78, 0x4e, 0x25, 0xac, 0xa4, 0xba, 0xf2, 0x5b, 0xe5, 0x73, 0x8a,
0xae, 0x59, 0xbb, 0xfe, 0x1c, 0x99, 0x77, 0x81, 0x44, 0x7a, 0x2b, 0x24};

static test_data check1[] = {
#ifdef ARCH_TEST_RSA_1024
#ifdef ARCH_TEST_RSA_PKCS1V15_SIGN_RAW
{"Test psa_asymmetric_sign - RSA KEYPAIR PKCS1V15 RAW\n", 1, PSA_KEY_TYPE_RSA_KEYPAIR,
{0}, 610, PSA_KEY_USAGE_SIGN, PSA_ALG_RSA_PKCS1V15_SIGN_RAW,
{0x61, 0x62, 0x63}, 3, 128,
{0x2c, 0x77, 0x44, 0x98, 0x3f, 0x02, 0x3a, 0xc7, 0xbb, 0x1c, 0x55, 0x52, 0x9d,
 0x83, 0xed, 0x11, 0xa7, 0x6a, 0x78, 0x98, 0xa1, 0xbb, 0x5c, 0xe1, 0x91, 0x37,
 0x5a, 0x4a, 0xa7, 0x49, 0x5a, 0x63, 0x3d, 0x27, 0x87, 0x9f, 0xf5, 0x8e, 0xba,
 0x5a, 0x57, 0x37, 0x1c, 0x34, 0xfe, 0xb1, 0x18, 0x0e, 0x8b, 0x85, 0x0d, 0x55,
 0x24, 0x76, 0xeb, 0xb5, 0x63, 0x4d, 0xf6, 0x20, 0x26, 0x19, 0x92, 0xf1, 0x2e,
 0xbe, 0xe9, 0x09, 0x70, 0x41, 0xdb, 0xbe, 0xa8, 0x5a, 0x42, 0xd4, 0x5b, 0x34,
 0x4b, 0xe5, 0x07, 0x3c, 0xeb, 0x77, 0x2f, 0xfc, 0x60, 0x49, 0x54, 0xb9, 0x15,
 0x8b, 0xa8, 0x1e, 0xc3, 0xdc, 0x4d, 0x9d, 0x65, 0xe3, 0xab, 0x7a, 0xa3, 0x18,
 0x16, 0x5f, 0x38, 0xc3, 0x6f, 0x84, 0x1f, 0x1c, 0x69, 0xcb, 0x1c, 0xfa, 0x49,
 0x4a, 0xa5, 0xcb, 0xb4, 0xd6, 0xc0, 0xef, 0xba, 0xfb, 0x04, 0x3a},
 128, 1024, PSA_SUCCESS
},
#endif

#ifdef ARCH_TEST_RSA_PKCS1V15_SIGN
#ifdef ARCH_TEST_SHA256
{"Test psa_asymmetric_sign - RSA KEYPAIR PKCS1V15 SHA-256\n", 2, PSA_KEY_TYPE_RSA_KEYPAIR,
{0}, 610, PSA_KEY_USAGE_SIGN, PSA_ALG_RSA_PKCS1V15_SIGN(PSA_ALG_SHA_256),
{0xba, 0x78, 0x16, 0xbf, 0x8f, 0x01, 0xcf, 0xea, 0x41, 0x41, 0x40, 0xde, 0x5d,
 0xae, 0x22, 0x23, 0xb0, 0x03, 0x61, 0xa3, 0x96, 0x17, 0x7a, 0x9c, 0xb4, 0x10,
 0xff, 0x61, 0xf2, 0x00, 0x15, 0xad}, 32, 128,
{0xa7, 0x36, 0x64, 0xd5, 0x5b, 0x39, 0xc7, 0xea, 0x6c, 0x1e, 0x5b, 0x50, 0x11,
 0x72, 0x4a, 0x11, 0xe1, 0xd7, 0x07, 0x3d, 0x3a, 0x68, 0xf4, 0x8c, 0x83, 0x6f,
 0xad, 0x15, 0x3a, 0x1d, 0x91, 0xb6, 0xab, 0xdb, 0xc8, 0xf6, 0x9d, 0xa1, 0x3b,
 0x20, 0x6c, 0xc9, 0x6a, 0xf6, 0x36, 0x3b, 0x11, 0x44, 0x58, 0xb0, 0x26, 0xaf,
 0x14, 0xb2, 0x4f, 0xab, 0x89, 0x29, 0xed, 0x63, 0x4c, 0x6a, 0x2a, 0xca, 0xce,
 0x0b, 0xcc, 0x62, 0xd9, 0xbb, 0x6a, 0x98, 0x4a, 0xfb, 0xcb, 0xfc, 0xd3, 0xa0,
 0x60, 0x8d, 0x32, 0xa2, 0xba, 0xe5, 0x35, 0xb9, 0xcd, 0x1e, 0xcd, 0xf9, 0xdd,
 0x28, 0x1d, 0xb1, 0xe0, 0x02, 0x5c, 0x3b, 0xfb, 0x55, 0x12, 0x96, 0x3e, 0xc3,
 0xb9, 0x8d, 0xda, 0xa6, 0x9e, 0x38, 0xbc, 0x3c, 0x84, 0xb1, 0xb6, 0x1a, 0x04,
 0xe5, 0x64, 0x86, 0x40, 0x85, 0x6a, 0xac, 0xc6, 0xfc, 0x73, 0x11},
 128, 1024, PSA_SUCCESS
},
#endif
#endif
#endif

#ifdef ARCH_TEST_SHA256
#ifdef ARCH_TEST_DETERMINISTIC_ECDSA
#ifdef ARCH_TEST_ECC_CURVE_SECP256R1
{"Test psa_asymmetric_sign - ECDSA SECP256R1 SHA-256\n", 3,
 PSA_KEY_TYPE_ECC_KEYPAIR(PSA_ECC_CURVE_SECP256R1), {0}, 32,
 PSA_KEY_USAGE_SIGN, PSA_ALG_DETERMINISTIC_ECDSA(PSA_ALG_SHA_256),
{0x9a, 0xc4, 0x33, 0x5b, 0x46, 0x9b, 0xbd, 0x79, 0x14, 0x39, 0x24, 0x85, 0x04,
 0xdd, 0x0d, 0x49, 0xc7, 0x13, 0x49, 0xa2, 0x95, 0xfe, 0xe5, 0xa1, 0xc6, 0x85,
 0x07, 0xf4, 0x5a, 0x9e, 0x1c, 0x7b}, 32, 128,
{0x6a, 0x33, 0x99, 0xf6, 0x94, 0x21, 0xff, 0xe1, 0x49, 0x03, 0x77, 0xad, 0xf2,
 0xea, 0x1f, 0x11, 0x7d, 0x81, 0xa6, 0x3c, 0xf5, 0xbf, 0x22, 0xe9, 0x18, 0xd5,
 0x11, 0x75, 0xeb, 0x25, 0x91, 0x51, 0xce, 0x95, 0xd7, 0xc2, 0x6c, 0xc0, 0x4e,
 0x25, 0x50, 0x3e, 0x2f, 0x7a, 0x1e, 0xc3, 0x57, 0x3e, 0x3c, 0x24, 0x12, 0x53,
 0x4b, 0xb4, 0xa1, 0x9b, 0x3a, 0x78, 0x11, 0x74, 0x2f, 0x49, 0xf5, 0x0f},
 64, 512, PSA_SUCCESS
},
#endif
#endif

#ifdef ARCH_TEST_RSA_1024
#ifdef ARCH_TEST_RSA_PKCS1V15_SIGN
{"Test psa_asymmetric_sign - Invalid key type (RSA public key)\n", 4,
PSA_KEY_TYPE_RSA_PUBLIC_KEY,
{0}, 162, PSA_KEY_USAGE_SIGN, PSA_ALG_RSA_PKCS1V15_SIGN(PSA_ALG_SHA_256),
{0xba, 0x78, 0x16, 0xbf, 0x8f, 0x01, 0xcf, 0xea, 0x41, 0x41, 0x40, 0xde, 0x5d,
 0xae, 0x22, 0x23, 0xb0, 0x03, 0x61, 0xa3, 0x96, 0x17, 0x7a, 0x9c, 0xb4, 0x10,
 0xff, 0x61, 0xf2, 0x00, 0x15, 0xad}, 32, 128,
{0xa7, 0x36, 0x64, 0xd5, 0x5b, 0x39, 0xc7, 0xea, 0x6c, 0x1e, 0x5b, 0x50, 0x11,
 0x72, 0x4a, 0x11, 0xe1, 0xd7, 0x07, 0x3d, 0x3a, 0x68, 0xf4, 0x8c, 0x83, 0x6f,
 0xad, 0x15, 0x3a, 0x1d, 0x91, 0xb6, 0xab, 0xdb, 0xc8, 0xf6, 0x9d, 0xa1, 0x3b,
 0x20, 0x6c, 0xc9, 0x6a, 0xf6, 0x36, 0x3b, 0x11, 0x44, 0x58, 0xb0, 0x26, 0xaf,
 0x14, 0xb2, 0x4f, 0xab, 0x89, 0x29, 0xed, 0x63, 0x4c, 0x6a, 0x2a, 0xca, 0xce,
 0x0b, 0xcc, 0x62, 0xd9, 0xbb, 0x6a, 0x98, 0x4a, 0xfb, 0xcb, 0xfc, 0xd3, 0xa0,
 0x60, 0x8d, 0x32, 0xa2, 0xba, 0xe5, 0x35, 0xb9, 0xcd, 0x1e, 0xcd, 0xf9, 0xdd,
 0x28, 0x1d, 0xb1, 0xe0, 0x02, 0x5c, 0x3b, 0xfb, 0x55, 0x12, 0x96, 0x3e, 0xc3,
 0xb9, 0x8d, 0xda, 0xa6, 0x9e, 0x38, 0xbc, 0x3c, 0x84, 0xb1, 0xb6, 0x1a, 0x04,
 0xe5, 0x64, 0x86, 0x40, 0x85, 0x6a, 0xac, 0xc6, 0xfc, 0x73, 0x11},
 128, 1024, PSA_ERROR_INVALID_ARGUMENT
},

{"Test psa_asymmetric_sign - Small output buffer\n", 5, PSA_KEY_TYPE_RSA_KEYPAIR,
{0}, 610, PSA_KEY_USAGE_SIGN, PSA_ALG_RSA_PKCS1V15_SIGN(PSA_ALG_SHA_256),
{0xba, 0x78, 0x16, 0xbf, 0x8f, 0x01, 0xcf, 0xea, 0x41, 0x41, 0x40, 0xde, 0x5d,
 0xae, 0x22, 0x23, 0xb0, 0x03, 0x61, 0xa3, 0x96, 0x17, 0x7a, 0x9c, 0xb4, 0x10,
 0xff, 0x61, 0xf2, 0x00, 0x15, 0xad}, 32, 120,
{0xa7, 0x36, 0x64, 0xd5, 0x5b, 0x39, 0xc7, 0xea, 0x6c, 0x1e, 0x5b, 0x50, 0x11,
 0x72, 0x4a, 0x11, 0xe1, 0xd7, 0x07, 0x3d, 0x3a, 0x68, 0xf4, 0x8c, 0x83, 0x6f,
 0xad, 0x15, 0x3a, 0x1d, 0x91, 0xb6, 0xab, 0xdb, 0xc8, 0xf6, 0x9d, 0xa1, 0x3b,
 0x20, 0x6c, 0xc9, 0x6a, 0xf6, 0x36, 0x3b, 0x11, 0x44, 0x58, 0xb0, 0x26, 0xaf,
 0x14, 0xb2, 0x4f, 0xab, 0x89, 0x29, 0xed, 0x63, 0x4c, 0x6a, 0x2a, 0xca, 0xce,
 0x0b, 0xcc, 0x62, 0xd9, 0xbb, 0x6a, 0x98, 0x4a, 0xfb, 0xcb, 0xfc, 0xd3, 0xa0,
 0x60, 0x8d, 0x32, 0xa2, 0xba, 0xe5, 0x35, 0xb9, 0xcd, 0x1e, 0xcd, 0xf9, 0xdd,
 0x28, 0x1d, 0xb1, 0xe0, 0x02, 0x5c, 0x3b, 0xfb, 0x55, 0x12, 0x96, 0x3e, 0xc3,
 0xb9, 0x8d, 0xda, 0xa6, 0x9e, 0x38, 0xbc, 0x3c, 0x84, 0xb1, 0xb6, 0x1a, 0x04,
 0xe5, 0x64, 0x86, 0x40, 0x85, 0x6a, 0xac, 0xc6, 0xfc, 0x73, 0x11},
 128, 1024, PSA_ERROR_BUFFER_TOO_SMALL
},
#endif

{"Test psa_asymmetric_sign - Invalid algorithm\n", 6, PSA_KEY_TYPE_RSA_KEYPAIR,
{0}, 610, PSA_KEY_USAGE_SIGN, PSA_ALG_SHA_256,
{0x61, 0x62, 0x63}, 3, 128,
{0x2c, 0x77, 0x44, 0x98, 0x3f, 0x02, 0x3a, 0xc7, 0xbb, 0x1c, 0x55, 0x52, 0x9d,
 0x83, 0xed, 0x11, 0xa7, 0x6a, 0x78, 0x98, 0xa1, 0xbb, 0x5c, 0xe1, 0x91, 0x37,
 0x5a, 0x4a, 0xa7, 0x49, 0x5a, 0x63, 0x3d, 0x27, 0x87, 0x9f, 0xf5, 0x8e, 0xba,
 0x5a, 0x57, 0x37, 0x1c, 0x34, 0xfe, 0xb1, 0x18, 0x0e, 0x8b, 0x85, 0x0d, 0x55,
 0x24, 0x76, 0xeb, 0xb5, 0x63, 0x4d, 0xf6, 0x20, 0x26, 0x19, 0x92, 0xf1, 0x2e,
 0xbe, 0xe9, 0x09, 0x70, 0x41, 0xdb, 0xbe, 0xa8, 0x5a, 0x42, 0xd4, 0x5b, 0x34,
 0x4b, 0xe5, 0x07, 0x3c, 0xeb, 0x77, 0x2f, 0xfc, 0x60, 0x49, 0x54, 0xb9, 0x15,
 0x8b, 0xa8, 0x1e, 0xc3, 0xdc, 0x4d, 0x9d, 0x65, 0xe3, 0xab, 0x7a, 0xa3, 0x18,
 0x16, 0x5f, 0x38, 0xc3, 0x6f, 0x84, 0x1f, 0x1c, 0x69, 0xcb, 0x1c, 0xfa, 0x49,
 0x4a, 0xa5, 0xcb, 0xb4, 0xd6, 0xc0, 0xef, 0xba, 0xfb, 0x04, 0x3a},
 128, 1024, PSA_ERROR_INVALID_ARGUMENT
},
#endif

#ifdef ARCH_TEST_RSA_PKCS1V15_SIGN
#ifdef ARCH_TEST_AES_128
{"Test psa_asymmetric_sign - Invalid key type (AES Key)\n", 7, PSA_KEY_TYPE_AES,
{0x30, 0x82, 0x02, 0x5e, 0x02, 0x01, 0x00, 0x02, 0x81, 0x81, 0x00, 0xaf, 0x05,
 0x7d, 0x39, 0x6e}, AES_16B_KEY_SIZE, PSA_KEY_USAGE_SIGN,
 PSA_ALG_RSA_PKCS1V15_SIGN(PSA_ALG_SHA_256),
{0x61, 0x62, 0x63}, 3, 128,
{0x2c, 0x77, 0x44, 0x98, 0x3f, 0x02, 0x3a, 0xc7, 0xbb, 0x1c, 0x55, 0x52, 0x9d,
 0x83, 0xed, 0x11, 0xa7, 0x6a, 0x78, 0x98, 0xa1, 0xbb, 0x5c, 0xe1, 0x91, 0x37,
 0x5a, 0x4a, 0xa7, 0x49, 0x5a, 0x63, 0x3d, 0x27, 0x87, 0x9f, 0xf5, 0x8e, 0xba,
 0x5a, 0x57, 0x37, 0x1c, 0x34, 0xfe, 0xb1, 0x18, 0x0e, 0x8b, 0x85, 0x0d, 0x55,
 0x24, 0x76, 0xeb, 0xb5, 0x63, 0x4d, 0xf6, 0x20, 0x26, 0x19, 0x92, 0xf1, 0x2e,
 0xbe, 0xe9, 0x09, 0x70, 0x41, 0xdb, 0xbe, 0xa8, 0x5a, 0x42, 0xd4, 0x5b, 0x34,
 0x4b, 0xe5, 0x07, 0x3c, 0xeb, 0x77, 0x2f, 0xfc, 0x60, 0x49, 0x54, 0xb9, 0x15,
 0x8b, 0xa8, 0x1e, 0xc3, 0xdc, 0x4d, 0x9d, 0x65, 0xe3, 0xab, 0x7a, 0xa3, 0x18,
 0x16, 0x5f, 0x38, 0xc3, 0x6f, 0x84, 0x1f, 0x1c, 0x69, 0xcb, 0x1c, 0xfa, 0x49,
 0x4a, 0xa5, 0xcb, 0xb4, 0xd6, 0xc0, 0xef, 0xba, 0xfb, 0x04, 0x3a},
 128, BYTES_TO_BITS(AES_16B_KEY_SIZE), PSA_ERROR_INVALID_ARGUMENT
},
#endif
#endif
#endif

#ifdef ARCH_TEST_RSA_1024
#ifdef ARCH_TEST_RSA_PKCS1V15_SIGN_RAW
{"Test psa_asymmetric_sign - Invalid usage\n", 8, PSA_KEY_TYPE_RSA_KEYPAIR,
{0}, 610, PSA_KEY_USAGE_ENCRYPT, PSA_ALG_RSA_PKCS1V15_SIGN_RAW,
{0x61, 0x62, 0x63}, 3, 128,
{0x2c, 0x77, 0x44, 0x98, 0x3f, 0x02, 0x3a, 0xc7, 0xbb, 0x1c, 0x55, 0x52, 0x9d,
 0x83, 0xed, 0x11, 0xa7, 0x6a, 0x78, 0x98, 0xa1, 0xbb, 0x5c, 0xe1, 0x91, 0x37,
 0x5a, 0x4a, 0xa7, 0x49, 0x5a, 0x63, 0x3d, 0x27, 0x87, 0x9f, 0xf5, 0x8e, 0xba,
 0x5a, 0x57, 0x37, 0x1c, 0x34, 0xfe, 0xb1, 0x18, 0x0e, 0x8b, 0x85, 0x0d, 0x55,
 0x24, 0x76, 0xeb, 0xb5, 0x63, 0x4d, 0xf6, 0x20, 0x26, 0x19, 0x92, 0xf1, 0x2e,
 0xbe, 0xe9, 0x09, 0x70, 0x41, 0xdb, 0xbe, 0xa8, 0x5a, 0x42, 0xd4, 0x5b, 0x34,
 0x4b, 0xe5, 0x07, 0x3c, 0xeb, 0x77, 0x2f, 0xfc, 0x60, 0x49, 0x54, 0xb9, 0x15,
 0x8b, 0xa8, 0x1e, 0xc3, 0xdc, 0x4d, 0x9d, 0x65, 0xe3, 0xab, 0x7a, 0xa3, 0x18,
 0x16, 0x5f, 0x38, 0xc3, 0x6f, 0x84, 0x1f, 0x1c, 0x69, 0xcb, 0x1c, 0xfa, 0x49,
 0x4a, 0xa5, 0xcb, 0xb4, 0xd6, 0xc0, 0xef, 0xba, 0xfb, 0x04, 0x3a},
 128, 1024, PSA_ERROR_NOT_PERMITTED
},
#endif

#ifdef ARCH_TEST_RSA_PKCS1V15_SIGN
#ifdef ARCH_TEST_SHA256
{"Test psa_asymmetric_sign - Wrong hash size\n", 9, PSA_KEY_TYPE_RSA_KEYPAIR,
{0}, 610, PSA_KEY_USAGE_SIGN, PSA_ALG_RSA_PKCS1V15_SIGN(PSA_ALG_SHA_256),
{0xba, 0x78, 0x16, 0xbf, 0x8f, 0x01, 0xcf, 0xea, 0x41, 0x41, 0x40, 0xde, 0x5d,
 0xae, 0x22, 0x23, 0xb0, 0x03, 0x61, 0xa3, 0x96, 0x17, 0x7a, 0x9c, 0xb4, 0x10,
 0xff, 0x61, 0xf2, 0x00}, 30, 128,
{0xa7, 0x36, 0x64, 0xd5, 0x5b, 0x39, 0xc7, 0xea, 0x6c, 0x1e, 0x5b, 0x50, 0x11,
 0x72, 0x4a, 0x11, 0xe1, 0xd7, 0x07, 0x3d, 0x3a, 0x68, 0xf4, 0x8c, 0x83, 0x6f,
 0xad, 0x15, 0x3a, 0x1d, 0x91, 0xb6, 0xab, 0xdb, 0xc8, 0xf6, 0x9d, 0xa1, 0x3b,
 0x20, 0x6c, 0xc9, 0x6a, 0xf6, 0x36, 0x3b, 0x11, 0x44, 0x58, 0xb0, 0x26, 0xaf,
 0x14, 0xb2, 0x4f, 0xab, 0x89, 0x29, 0xed, 0x63, 0x4c, 0x6a, 0x2a, 0xca, 0xce,
 0x0b, 0xcc, 0x62, 0xd9, 0xbb, 0x6a, 0x98, 0x4a, 0xfb, 0xcb, 0xfc, 0xd3, 0xa0,
 0x60, 0x8d, 0x32, 0xa2, 0xba, 0xe5, 0x35, 0xb9, 0xcd, 0x1e, 0xcd, 0xf9, 0xdd,
 0x28, 0x1d, 0xb1, 0xe0, 0x02, 0x5c, 0x3b, 0xfb, 0x55, 0x12, 0x96, 0x3e, 0xc3,
 0xb9, 0x8d, 0xda, 0xa6, 0x9e, 0x38, 0xbc, 0x3c, 0x84, 0xb1, 0xb6, 0x1a, 0x04,
 0xe5, 0x64, 0x86, 0x40, 0x85, 0x6a, 0xac, 0xc6, 0xfc, 0x73, 0x11},
 128, 1024, PSA_ERROR_INVALID_ARGUMENT
},
#endif
#endif
#endif
};

static test_data check2[] = {
#ifdef ARCH_TEST_RSA_1024
#ifdef ARCH_TEST_RSA_PKCS1V15_SIGN_RAW
{"Test psa_asymmetric_sign - Negative case\n", 10, PSA_KEY_TYPE_RSA_KEYPAIR,
{0}, 610, PSA_KEY_USAGE_SIGN, PSA_ALG_RSA_PKCS1V15_SIGN_RAW,
{0x61, 0x62, 0x63}, 3, 128,
{0x2c, 0x77, 0x44, 0x98, 0x3f, 0x02, 0x3a, 0xc7, 0xbb, 0x1c, 0x55, 0x52, 0x9d,
 0x83, 0xed, 0x11, 0xa7, 0x6a, 0x78, 0x98, 0xa1, 0xbb, 0x5c, 0xe1, 0x91, 0x37,
 0x5a, 0x4a, 0xa7, 0x49, 0x5a, 0x63, 0x3d, 0x27, 0x87, 0x9f, 0xf5, 0x8e, 0xba,
 0x5a, 0x57, 0x37, 0x1c, 0x34, 0xfe, 0xb1, 0x18, 0x0e, 0x8b, 0x85, 0x0d, 0x55,
 0x24, 0x76, 0xeb, 0xb5, 0x63, 0x4d, 0xf6, 0x20, 0x26, 0x19, 0x92, 0xf1, 0x2e,
 0xbe, 0xe9, 0x09, 0x70, 0x41, 0xdb, 0xbe, 0xa8, 0x5a, 0x42, 0xd4, 0x5b, 0x34,
 0x4b, 0xe5, 0x07, 0x3c, 0xeb, 0x77, 0x2f, 0xfc, 0x60, 0x49, 0x54, 0xb9, 0x15,
 0x8b, 0xa8, 0x1e, 0xc3, 0xdc, 0x4d, 0x9d, 0x65, 0xe3, 0xab, 0x7a, 0xa3, 0x18,
 0x16, 0x5f, 0x38, 0xc3, 0x6f, 0x84, 0x1f, 0x1c, 0x69, 0xcb, 0x1c, 0xfa, 0x49,
 0x4a, 0xa5, 0xcb, 0xb4, 0xd6, 0xc0, 0xef, 0xba, 0xfb, 0x04, 0x3a},
 128, 1024, PSA_SUCCESS
},
#endif
#endif
};
