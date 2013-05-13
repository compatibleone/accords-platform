/* -------------------------------------------------------------------- */
/*  ACCORDS PLATFORM                                                    */
/*  (C) 2011 by Iain James Marshall (Prologue) <ijm667@hotmail.com>     */
/*  (c) 2012  by Hamid MEDJAHED     (Prologue) <hmedjahed@prologue.fr> */  
/* -------------------------------------------------------------------- */
/* Licensed under the Apache License, Version 2.0 (the "License"); 	*/
/* you may not use this file except in compliance with the License. 	*/
/* You may obtain a copy of the License at 				*/
/*  									*/
/*  http://www.apache.org/licenses/LICENSE-2.0 				*/
/*  									*/
/* Unless required by applicable law or agreed to in writing, software 	*/
/* distributed under the License is distributed on an "AS IS" BASIS, 	*/
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 	*/
/* implied. 								*/
/* See the License for the specific language governing permissions and 	*/
/* limitations under the License. 					*/
/* -------------------------------------------------------------------- */

#ifndef HMAC_SHA2_H
#define HMAC_SHA2_H

#include "sha2.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    sha224_ctx ctx_inside;
    sha224_ctx ctx_outside;

    /* for hmac_reinit */
    sha224_ctx ctx_inside_reinit;
    sha224_ctx ctx_outside_reinit;

    unsigned char block_ipad[SHA224_BLOCK_SIZE];
    unsigned char block_opad[SHA224_BLOCK_SIZE];
} hmac_sha224_ctx;

typedef struct {
    sha256_ctx ctx_inside;
    sha256_ctx ctx_outside;

    /* for hmac_reinit */
    sha256_ctx ctx_inside_reinit;
    sha256_ctx ctx_outside_reinit;

    unsigned char block_ipad[SHA256_BLOCK_SIZE];
    unsigned char block_opad[SHA256_BLOCK_SIZE];
} hmac_sha256_ctx;

typedef struct {
    sha384_ctx ctx_inside;
    sha384_ctx ctx_outside;

    /* for hmac_reinit */
    sha384_ctx ctx_inside_reinit;
    sha384_ctx ctx_outside_reinit;

    unsigned char block_ipad[SHA384_BLOCK_SIZE];
    unsigned char block_opad[SHA384_BLOCK_SIZE];
} hmac_sha384_ctx;

typedef struct {
    sha512_ctx ctx_inside;
    sha512_ctx ctx_outside;

    /* for hmac_reinit */
    sha512_ctx ctx_inside_reinit;
    sha512_ctx ctx_outside_reinit;

    unsigned char block_ipad[SHA512_BLOCK_SIZE];
    unsigned char block_opad[SHA512_BLOCK_SIZE];
} hmac_sha512_ctx;

void hmac_sha224_init(hmac_sha224_ctx *ctx, const unsigned char *key,
                      unsigned int key_size);
void hmac_sha224_reinit(hmac_sha224_ctx *ctx);
void hmac_sha224_update(hmac_sha224_ctx *ctx, const unsigned char *message,
                        unsigned int message_len);
void hmac_sha224_final(hmac_sha224_ctx *ctx, unsigned char *mac,
                       unsigned int mac_size);
void hmac_sha224(const unsigned char *key, unsigned int key_size,
                 const unsigned char *message, unsigned int message_len,
                 unsigned char *mac, unsigned mac_size);

void hmac_sha256_init(hmac_sha256_ctx *ctx, const unsigned char *key,
                      unsigned int key_size);
void hmac_sha256_reinit(hmac_sha256_ctx *ctx);
void hmac_sha256_update(hmac_sha256_ctx *ctx, const unsigned char *message,
                        unsigned int message_len);
void hmac_sha256_final(hmac_sha256_ctx *ctx, unsigned char *mac,
                       unsigned int mac_size);
void hmac_sha256(const unsigned char *key, unsigned int key_size,
                 const unsigned char *message, unsigned int message_len,
                 unsigned char *mac, unsigned mac_size);

void hmac_sha384_init(hmac_sha384_ctx *ctx, const unsigned char *key,
                      unsigned int key_size);
void hmac_sha384_reinit(hmac_sha384_ctx *ctx);
void hmac_sha384_update(hmac_sha384_ctx *ctx, const unsigned char *message,
                        unsigned int message_len);
void hmac_sha384_final(hmac_sha384_ctx *ctx, unsigned char *mac,
                       unsigned int mac_size);
void hmac_sha384(const unsigned char *key, unsigned int key_size,
                 const unsigned char *message, unsigned int message_len,
                 unsigned char *mac, unsigned mac_size);

void hmac_sha512_init(hmac_sha512_ctx *ctx, const unsigned char *key,
                      unsigned int key_size);
void hmac_sha512_reinit(hmac_sha512_ctx *ctx);
void hmac_sha512_update(hmac_sha512_ctx *ctx, const unsigned char *message,
                        unsigned int message_len);
void hmac_sha512_final(hmac_sha512_ctx *ctx, unsigned char *mac,
                       unsigned int mac_size);
void hmac_sha512(const unsigned char *key, unsigned int key_size,
                 const unsigned char *message, unsigned int message_len,
                 unsigned char *mac, unsigned mac_size);

char *test(const char *vector, unsigned char *digest,unsigned int digest_size);
char *signature_hmac(char *message, char *key);
char    * base_64( const unsigned char *src, size_t sz );

#ifdef __cplusplus
}
#endif

#endif /* !HMAC_SHA2_H */

