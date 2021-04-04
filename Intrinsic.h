//	4/4/2021 Written by Satoru Ogura.	
//	https://www.intel.com/content/dam/doc/white-paper/advanced-encryption-standard-new-instructions-set-paper.pdf

#if !defined (ALIGN16)
# if defined (__GNUC__)
#  define ALIGN16 __attribute__ ( (aligned (16)))
# else
#  define ALIGN16 __declspec (align (16))
# endif
#endif

typedef	unsigned char	UI1;
typedef	unsigned int	UI4;

#include	<wmmintrin.h>
#include	<emmintrin.h>
#include	<smmintrin.h>

inline __m128i
AES_128_ASSIST( __m128i temp1, __m128i temp2 ) {
	__m128i temp3;
	temp2 = _mm_shuffle_epi32	( temp2 ,0xff	);
	temp3 = _mm_slli_si128		( temp1, 0x4	);
	temp1 = _mm_xor_si128		( temp1, temp3	);
	temp3 = _mm_slli_si128		( temp3, 0x4	);
	temp1 = _mm_xor_si128		( temp1, temp3	);
	temp3 = _mm_slli_si128		( temp3, 0x4	);
	temp1 = _mm_xor_si128		( temp1, temp3	);
	temp1 = _mm_xor_si128		( temp1, temp2	);
	return temp1;
}

inline void
KEY_192_ASSIST( __m128i* temp1, __m128i* temp2, __m128i* temp3 ) {
	__m128i temp4;
	*temp2 = _mm_shuffle_epi32	( *temp2, 0x55	);
	 temp4 = _mm_slli_si128		( *temp1, 0x4	);
	*temp1 = _mm_xor_si128		( *temp1, temp4	);
	 temp4 = _mm_slli_si128		(  temp4, 0x4	);
	*temp1 = _mm_xor_si128		( *temp1, temp4	);
	 temp4 = _mm_slli_si128		(  temp4, 0x4	);
	*temp1 = _mm_xor_si128		( *temp1, temp4	);
	*temp1 = _mm_xor_si128		( *temp1, *temp2);
	*temp2 = _mm_shuffle_epi32	( *temp1, 0xff	);
	 temp4 = _mm_slli_si128		( *temp3, 0x4	);
	*temp3 = _mm_xor_si128		( *temp3, temp4	);
	*temp3 = _mm_xor_si128		( *temp3, *temp2);
}

inline void
KEY_256_ASSIST_1( __m128i* temp1, __m128i* temp2 ) {
	__m128i temp4;
	*temp2 = _mm_shuffle_epi32	( *temp2, 0xff	);
	 temp4 = _mm_slli_si128		( *temp1, 0x4	);
	*temp1 = _mm_xor_si128		( *temp1, temp4	);
	 temp4 = _mm_slli_si128		(  temp4, 0x4	);
	*temp1 = _mm_xor_si128		( *temp1, temp4	);
	 temp4 = _mm_slli_si128		(  temp4, 0x4	);
	*temp1 = _mm_xor_si128		( *temp1, temp4	);
	*temp1 = _mm_xor_si128		( *temp1, *temp2);
}
inline void
KEY_256_ASSIST_2( __m128i* temp1, __m128i* temp3 ) {
	__m128i temp2,temp4;
	 temp4 = _mm_aeskeygenassist_si128( *temp1, 0x0 );
	 temp2 = _mm_shuffle_epi32	(  temp4, 0xaa	);
	 temp4 = _mm_slli_si128		( *temp3, 0x4	);
	*temp3 = _mm_xor_si128		( *temp3, temp4	);
	 temp4 = _mm_slli_si128		(  temp4, 0x4	);
	*temp3 = _mm_xor_si128		( *temp3, temp4	);
	 temp4 = _mm_slli_si128		(  temp4, 0x4	);
	*temp3 = _mm_xor_si128		( *temp3, temp4	);
	*temp3 = _mm_xor_si128		( *temp3, temp2	);
}

inline void
AES_128_Key_Expansion( __m128i* userkey, __m128i* key ) {
	__m128i temp1, temp2;
	temp1 = _mm_loadu_si128( userkey )															; key[  0 ] = temp1;
	temp2 = _mm_aeskeygenassist_si128( temp1, 0x01 ); temp1 = AES_128_ASSIST( temp1, temp2 )	; key[  1 ] = temp1;
	temp2 = _mm_aeskeygenassist_si128( temp1, 0x02 ); temp1 = AES_128_ASSIST( temp1, temp2 )	; key[  2 ] = temp1;
	temp2 = _mm_aeskeygenassist_si128( temp1, 0x04 ); temp1 = AES_128_ASSIST( temp1, temp2 )	; key[  3 ] = temp1;
	temp2 = _mm_aeskeygenassist_si128( temp1, 0x08 ); temp1 = AES_128_ASSIST( temp1, temp2 )	; key[  4 ] = temp1;
	temp2 = _mm_aeskeygenassist_si128( temp1, 0x10 ); temp1 = AES_128_ASSIST( temp1, temp2 )	; key[  5 ] = temp1;
	temp2 = _mm_aeskeygenassist_si128( temp1, 0x20 ); temp1 = AES_128_ASSIST( temp1, temp2 )	; key[  6 ] = temp1;
	temp2 = _mm_aeskeygenassist_si128( temp1, 0x40 ); temp1 = AES_128_ASSIST( temp1, temp2 )	; key[  7 ] = temp1;
	temp2 = _mm_aeskeygenassist_si128( temp1, 0x80 ); temp1 = AES_128_ASSIST( temp1, temp2 )	; key[  8 ] = temp1;
	temp2 = _mm_aeskeygenassist_si128( temp1, 0x1b ); temp1 = AES_128_ASSIST( temp1, temp2 )	; key[  9 ] = temp1;
	temp2 = _mm_aeskeygenassist_si128( temp1, 0x36 ); temp1 = AES_128_ASSIST( temp1, temp2 )	; key[ 10 ] = temp1;
}

inline void
AES_192_Key_Expansion( __m128i* userkey, __m128i* key ) {
	__m128i temp1, temp2, temp3, temp4;
	temp1 = _mm_loadu_si128( userkey );
	temp3 = _mm_loadu_si128( userkey + 1 );
	key[  0 ] = temp1;
	key[  1 ] = temp3;
	temp2 = _mm_aeskeygenassist_si128( temp3, 0x1 );
	KEY_192_ASSIST( &temp1, &temp2, &temp3 );
	key[  1 ] = (__m128i)_mm_shuffle_pd( (__m128d)key[ 1 ], (__m128d)temp1, 0 );
	key[  2 ] = (__m128i)_mm_shuffle_pd( (__m128d)temp1, (__m128d)temp3, 1 );
	temp2 =_mm_aeskeygenassist_si128( temp3,0x2 );
	KEY_192_ASSIST(&temp1, &temp2, &temp3);
	key[  3 ] = temp1;
	key[  4 ] = temp3;
	temp2 = _mm_aeskeygenassist_si128( temp3, 0x4 );
	KEY_192_ASSIST( &temp1, &temp2, &temp3 );
	key[  4 ] = (__m128i)_mm_shuffle_pd( (__m128d)key[ 4 ], (__m128d)temp1, 0 );
	key[  5 ] = (__m128i)_mm_shuffle_pd( (__m128d)temp1, (__m128d)temp3, 1 );
	temp2 = _mm_aeskeygenassist_si128( temp3,0x8 );
	KEY_192_ASSIST( &temp1, &temp2, &temp3 );
	key[  6 ] = temp1;
	key[  7 ] = temp3;
	temp2=_mm_aeskeygenassist_si128( temp3, 0x10 );
	KEY_192_ASSIST( &temp1, &temp2, &temp3 );
	key[  7 ] = (__m128i)_mm_shuffle_pd( (__m128d)key[ 7 ], (__m128d)temp1, 0 );
	key[  8 ] = (__m128i)_mm_shuffle_pd( (__m128d)temp1, (__m128d)temp3, 1 );
	temp2 = _mm_aeskeygenassist_si128( temp3, 0x20 );
	KEY_192_ASSIST( &temp1, &temp2, &temp3 );
	key[  9 ]=temp1;
	key[ 10 ]=temp3;
	temp2 = _mm_aeskeygenassist_si128( temp3, 0x40 );
	KEY_192_ASSIST( &temp1, &temp2, &temp3 );
	key[ 10 ] = (__m128i)_mm_shuffle_pd( (__m128d)key[ 10 ], (__m128d)temp1, 0 );
	key[ 11 ] = (__m128i)_mm_shuffle_pd( (__m128d)temp1, (__m128d)temp3, 1 );
	temp2=_mm_aeskeygenassist_si128( temp3, 0x80 );
	KEY_192_ASSIST( &temp1, &temp2, &temp3 );
	key[ 12 ] = temp1;
}

inline void
AES_256_Key_Expansion( __m128i* userkey, __m128i* key ) {
	__m128i temp1, temp2, temp3;
	temp1 = _mm_loadu_si128( userkey );
	temp3 = _mm_loadu_si128( userkey + 1 );
	key[ 0 ] = temp1;
	key[ 1 ] = temp3;
	temp2 = _mm_aeskeygenassist_si128( temp3, 0x01 );
	KEY_256_ASSIST_1( &temp1, &temp2 );
	key[2]=temp1;
	KEY_256_ASSIST_2(&temp1, &temp3);
	key[3]=temp3;
	temp2 = _mm_aeskeygenassist_si128(temp3,0x02);
	KEY_256_ASSIST_1(&temp1, &temp2);
	key[4]=temp1;
	KEY_256_ASSIST_2(&temp1, &temp3);
	key[5]=temp3;
	temp2 = _mm_aeskeygenassist_si128(temp3,0x04);
	KEY_256_ASSIST_1(&temp1, &temp2);
	key[6]=temp1;
	KEY_256_ASSIST_2(&temp1, &temp3);
	key[7]=temp3;
	temp2 = _mm_aeskeygenassist_si128(temp3,0x08);
	KEY_256_ASSIST_1(&temp1, &temp2);
	key[8]=temp1;
	KEY_256_ASSIST_2(&temp1, &temp3);
	key[9]=temp3;
	temp2 = _mm_aeskeygenassist_si128(temp3,0x10);
	KEY_256_ASSIST_1(&temp1, &temp2);
	key[10]=temp1;
	KEY_256_ASSIST_2(&temp1, &temp3);
	key[11]=temp3;
	temp2 = _mm_aeskeygenassist_si128(temp3,0x20);
	KEY_256_ASSIST_1(&temp1, &temp2);
	key[12]=temp1;
	KEY_256_ASSIST_2(&temp1, &temp3);
	key[13]=temp3;
	temp2 = _mm_aeskeygenassist_si128(temp3,0x40);
	KEY_256_ASSIST_1(&temp1, &temp2);
	key[14]=temp1;
}

inline void
AES_ECB_encrypt(
	UI1*		in		//pointer to the PLAINTEXT
,	UI1*		out		//pointer to the CIPHERTEXT buffer
,	size_t		length	//text length in bytes
,	__m128i*	key		//pointer to the expanded key schedule
,	int			Nr		//number of AES rounds 10,12 or 14
) {
	length = ( length + 15 ) / 16;
	for ( auto i = 0; i < length; i++ ) {
		__m128i tmp = _mm_loadu_si128( (__m128i*)in + i );
		tmp = _mm_xor_si128( tmp, key[ 0 ] );
		for ( auto j = 1; j < Nr; j++ ) tmp = _mm_aesenc_si128( tmp, key[ j ] );
		tmp = _mm_aesenclast_si128( tmp, key[ Nr ] );
		_mm_storeu_si128( (__m128i*)out + i, tmp );
	}
}

inline void
AES_ECB_decrypt(
	UI1*		in		//pointer to the CIPHERTEXT
,	UI1*		out		//pointer to the DECRYPTED TEXT buffer
,	size_t		length	//text length in bytes
,	__m128i*	key		//pointer to the expanded key schedule
,	int			Nr		//number of AES rounds 10,12 or 14
) {
	length = ( length + 15 ) / 16;
	for ( auto i = 0; i < length; i++ ) {
		__m128i tmp = _mm_loadu_si128( (__m128i*)in + i );
		tmp = _mm_xor_si128( tmp, key[ 0 ] );
		for ( auto j = 1; j < Nr; j++ ) tmp = _mm_aesdec_si128( tmp, key[ j ] );
		tmp = _mm_aesdeclast_si128( tmp, key[ Nr ] );
		_mm_storeu_si128( (__m128i*)out + i, tmp );
	}
}

inline void
AES_CBC_encrypt(
	UI1*		in
,	UI1*		out
,	UI1*		ivec
,	size_t		length
,	__m128i*	key
,	int			Nr
) {
	length = ( length + 15 ) / 16;
	__m128i feedback=_mm_loadu_si128( (__m128i*)ivec );
	for ( auto i = 0; i < length; i++ ) {
		__m128i data = _mm_loadu_si128( (__m128i*)in + i );
		feedback = _mm_xor_si128( data, feedback );
		feedback = _mm_xor_si128( feedback, key[ 0 ] );
		for ( auto j = 1; j < Nr; j++ ) feedback = _mm_aesenc_si128( feedback, key[ j ] );
		feedback = _mm_aesenclast_si128( feedback, key[ Nr ] );
		_mm_storeu_si128( (__m128i*)out + i, feedback );
	}
}

inline void
AES_CBC_decrypt(
	UI1*		in
,	UI1*		out
,	UI1*		ivec
,	size_t		length
,	__m128i*	key
,	int			Nr
) {
	length = ( length + 15 ) / 16;
	__m128i feedback=_mm_loadu_si128( (__m128i*)ivec );
	for ( auto i = 0; i < length; i++ ) {
		__m128i last_in =_mm_loadu_si128( (__m128i*)in + i );
		__m128i data = _mm_xor_si128( last_in, key[ 0 ] );
		for ( auto j = 1; j < Nr; j++ ) data = _mm_aesdec_si128( data, key[ j ] );
		data = _mm_aesdeclast_si128( data, key[ Nr ] );
		data = _mm_xor_si128( data, feedback );
		_mm_storeu_si128( (__m128i*)out + i, data );
		feedback = last_in;
	}
}

inline void
AES_CTR_crypt(
	UI1*		in
,	UI1*		out
,	UI1*		ivec
,	UI1*		nonce
,	size_t		length
,	__m128i*	key
,	int			Nr
) {
	__m128i ctr_block, tmp, ONE, BSWAP_EPI64;
	length = ( length + 15 ) / 16;
	ONE = _mm_set_epi32(0,1,0,0);
	BSWAP_EPI64 = _mm_setr_epi8( 7,6,5,4,3,2,1,0,15,14,13,12,11,10,9,8 );
	ctr_block = _mm_insert_epi64( ctr_block, *(long long*)ivec, 1 );
	ctr_block = _mm_insert_epi32( ctr_block, *(long*)nonce, 1 );
	ctr_block = _mm_srli_si128( ctr_block, 4 );
	ctr_block = _mm_shuffle_epi8( ctr_block, BSWAP_EPI64 );
	ctr_block = _mm_add_epi64( ctr_block, ONE );
	for ( auto i = 0; i < length; i++ ) {
		tmp = _mm_shuffle_epi8( ctr_block, BSWAP_EPI64 );
		ctr_block = _mm_add_epi64( ctr_block, ONE );
		tmp = _mm_xor_si128( tmp, key[ 0 ] );
		for( auto j = 1; j < Nr; j++ ) tmp = _mm_aesenc_si128( tmp, key[ j ] );
		tmp = _mm_aesenclast_si128( tmp, key[ Nr ] );
		tmp = _mm_xor_si128( tmp, _mm_loadu_si128( (__m128i*)in + i ) );
		_mm_storeu_si128( (__m128i*)out + i, tmp );
	}
}

inline void
AES_decrypt_key_128(
	__m128i* encryptKey
,	__m128i* decryptKey
) {
	decryptKey[ 10 ] = encryptKey[  0 ];
	decryptKey[  9 ] = _mm_aesimc_si128( encryptKey[  1 ] );
	decryptKey[  8 ] = _mm_aesimc_si128( encryptKey[  2 ] );
	decryptKey[  7 ] = _mm_aesimc_si128( encryptKey[  3 ] );
	decryptKey[  6 ] = _mm_aesimc_si128( encryptKey[  4 ] );
	decryptKey[  5 ] = _mm_aesimc_si128( encryptKey[  5 ] );
	decryptKey[  4 ] = _mm_aesimc_si128( encryptKey[  6 ] );
	decryptKey[  3 ] = _mm_aesimc_si128( encryptKey[  7 ] );
	decryptKey[  2 ] = _mm_aesimc_si128( encryptKey[  8 ] );
	decryptKey[  1 ] = _mm_aesimc_si128( encryptKey[  9 ] );
	decryptKey[  0 ] = encryptKey[ 10 ];
}

inline void
AES_decrypt_key_192(
	__m128i* encryptKey
,	__m128i* decryptKey
) {
	decryptKey[ 12 ] = encryptKey[  0 ];
	decryptKey[ 11 ] = _mm_aesimc_si128( encryptKey[  1 ] );
	decryptKey[ 10 ] = _mm_aesimc_si128( encryptKey[  2 ] );
	decryptKey[  9 ] = _mm_aesimc_si128( encryptKey[  3 ] );
	decryptKey[  8 ] = _mm_aesimc_si128( encryptKey[  4 ] );
	decryptKey[  7 ] = _mm_aesimc_si128( encryptKey[  5 ] );
	decryptKey[  6 ] = _mm_aesimc_si128( encryptKey[  6 ] );
	decryptKey[  5 ] = _mm_aesimc_si128( encryptKey[  7 ] );
	decryptKey[  4 ] = _mm_aesimc_si128( encryptKey[  8 ] );
	decryptKey[  3 ] = _mm_aesimc_si128( encryptKey[  9 ] );
	decryptKey[  2 ] = _mm_aesimc_si128( encryptKey[ 10 ] );
	decryptKey[  1 ] = _mm_aesimc_si128( encryptKey[ 11 ] );
	decryptKey[  0 ] = encryptKey[ 12 ];
}

inline void
AES_decrypt_key_256(
	__m128i* encryptKey
,	__m128i* decryptKey
) {
	decryptKey[ 14 ] = encryptKey[  0 ];
	decryptKey[ 13 ] = _mm_aesimc_si128( encryptKey[  1 ] );
	decryptKey[ 12 ] = _mm_aesimc_si128( encryptKey[  2 ] );
	decryptKey[ 11 ] = _mm_aesimc_si128( encryptKey[  3 ] );
	decryptKey[ 10 ] = _mm_aesimc_si128( encryptKey[  4 ] );
	decryptKey[  9 ] = _mm_aesimc_si128( encryptKey[  5 ] );
	decryptKey[  8 ] = _mm_aesimc_si128( encryptKey[  6 ] );
	decryptKey[  7 ] = _mm_aesimc_si128( encryptKey[  7 ] );
	decryptKey[  6 ] = _mm_aesimc_si128( encryptKey[  8 ] );
	decryptKey[  5 ] = _mm_aesimc_si128( encryptKey[  9 ] );
	decryptKey[  4 ] = _mm_aesimc_si128( encryptKey[ 10 ] );
	decryptKey[  3 ] = _mm_aesimc_si128( encryptKey[ 11 ] );
	decryptKey[  2 ] = _mm_aesimc_si128( encryptKey[ 12 ] );
	decryptKey[  1 ] = _mm_aesimc_si128( encryptKey[ 13 ] );
	decryptKey[  0 ] = encryptKey[ 14 ];
}

#define cpuid( func, ax, bx, cx, dx ) __asm__ __volatile__ ( "cpuid": "=a" (ax), "=b" (bx), "=c" (cx), "=d" (dx) : "a" (func) )
inline int
Check_CPU_support_AES() {
	unsigned int a,b,c,d;
	cpuid( 1, a, b, c, d );
	return ( c & 0x2000000 );
}


