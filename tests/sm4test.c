/*
 *  Copyright 2014-2022 The GmSSL Project. All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the License); you may
 *  not use this file except in compliance with the License.
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <gmssl/hex.h>
#include <gmssl/sm4.h>
#include <gmssl/error.h>
#include <gmssl/rand.h>


static int test_sm4(void)
{
	const uint8_t user_key[16] = {
		0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
		0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10,
	};
	const uint32_t rk[32] = {
		0xf12186f9, 0x41662b61, 0x5a6ab19a, 0x7ba92077,
		0x367360f4, 0x776a0c61, 0xb6bb89b3, 0x24763151,
		0xa520307c, 0xb7584dbd, 0xc30753ed, 0x7ee55b57,
		0x6988608c, 0x30d895b7, 0x44ba14af, 0x104495a1,
		0xd120b428, 0x73b55fa3, 0xcc874966, 0x92244439,
		0xe89e641f, 0x98ca015a, 0xc7159060, 0x99e1fd2e,
		0xb79bd80c, 0x1d2115b0, 0x0e228aeb, 0xf1780c81,
		0x428d3654, 0x62293496, 0x01cf72e5, 0x9124a012,
	};
	const uint8_t plaintext[16] = {
		0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
		0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10,
	};
	const uint8_t ciphertext[16] = {
		0x68, 0x1e, 0xdf, 0x34, 0xd2, 0x06, 0x96, 0x5e,
		0x86, 0xb3, 0xe9, 0x4f, 0x53, 0x6e, 0x42, 0x46,
	};
	const uint8_t ciphertext1m[16] = {
		0x59, 0x52, 0x98, 0xc7, 0xc6, 0xfd, 0x27, 0x1f,
		0x04, 0x02, 0xf8, 0x04, 0xc3, 0x3d, 0x3f, 0x66,
	};

	SM4_KEY key;
	unsigned char buf[16];
	int i;

	/* test key scheduling */
	sm4_set_encrypt_key(&key, user_key);

	if (memcmp(key.rk, rk, sizeof(rk)) != 0) {
		fprintf(stderr, "sm4 key scheduling not passed!\n");
		return -1;
	}

	/* test encrypt once */
	sm4_encrypt(&key, plaintext, buf);
	if (memcmp(buf, ciphertext, sizeof(ciphertext)) != 0) {
		fprintf(stderr, "sm4 encrypt not pass!\n");
		return -1;
	}

	/* test encrypt 1000000 times */
	memcpy(buf, plaintext, sizeof(plaintext));
	for (i = 0; i < 1000000; i++) {
		sm4_encrypt(&key, buf, buf);
	}
	if (memcmp(buf, ciphertext1m, sizeof(ciphertext1m)) != 0) {
		fprintf(stderr, "sm4 encrypt 1000000 times not pass!\n");
		return -1;
	}

	/* test decrypt */
	memset(&key, 0, sizeof(key));
	memset(buf, 0, sizeof(buf));
	sm4_set_decrypt_key(&key, user_key);
	sm4_encrypt(&key, ciphertext, buf);
	if (memcmp(buf, plaintext, sizeof(plaintext)) != 0) {
		fprintf(stderr, "sm4 decrypt not pass!\n");
		return -1;
	}

	printf("%s() ok\n", __FUNCTION__);
	return 1;
}




int main(void)
{
	
	if (test_sm4() != 1) goto err;
	/*
	if (test_sm4_cbc() != 1) goto err;
	if (test_sm4_cbc_padding() != 1) goto err;

	if (test_sm4_gcm() != 1) goto err;
	if (test_sm4_gcm_gbt36624_1() != 1) goto err;
	if (test_sm4_gcm_gbt36624_2() != 1) goto err;
	if (test_sm4_cbc_update() != 1) goto err;


	*/
	printf("%s all tests passed\n", __FILE__);
	return 0;
err:
	error_print();
	return 1;
}
