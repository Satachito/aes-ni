//	Apr. 2021 Written by Satoru Ogura.	
//	https://www.intel.com/content/dam/doc/white-paper/advanced-encryption-standard-new-instructions-set-paper.pdf

#if !defined (ALIGN16)
# if defined (__GNUC__)
#  define ALIGN16 __attribute__ ( (aligned (16)))
# else
#  define ALIGN16 __declspec (align (16))
# endif
#endif

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
	__m128i temp1, temp2, temp3;
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
AES_decrypto_key_128(
	__m128i* encryptoKey
,	__m128i* decryptoKey
) {
	decryptoKey[ 10 ] = encryptoKey[  0 ];
	decryptoKey[  9 ] = _mm_aesimc_si128( encryptoKey[  1 ] );
	decryptoKey[  8 ] = _mm_aesimc_si128( encryptoKey[  2 ] );
	decryptoKey[  7 ] = _mm_aesimc_si128( encryptoKey[  3 ] );
	decryptoKey[  6 ] = _mm_aesimc_si128( encryptoKey[  4 ] );
	decryptoKey[  5 ] = _mm_aesimc_si128( encryptoKey[  5 ] );
	decryptoKey[  4 ] = _mm_aesimc_si128( encryptoKey[  6 ] );
	decryptoKey[  3 ] = _mm_aesimc_si128( encryptoKey[  7 ] );
	decryptoKey[  2 ] = _mm_aesimc_si128( encryptoKey[  8 ] );
	decryptoKey[  1 ] = _mm_aesimc_si128( encryptoKey[  9 ] );
	decryptoKey[  0 ] = encryptoKey[ 10 ];
}

inline void
AES_decrypto_key_192(
	__m128i* encryptoKey
,	__m128i* decryptoKey
) {
	decryptoKey[ 12 ] = encryptoKey[  0 ];
	decryptoKey[ 11 ] = _mm_aesimc_si128( encryptoKey[  1 ] );
	decryptoKey[ 10 ] = _mm_aesimc_si128( encryptoKey[  2 ] );
	decryptoKey[  9 ] = _mm_aesimc_si128( encryptoKey[  3 ] );
	decryptoKey[  8 ] = _mm_aesimc_si128( encryptoKey[  4 ] );
	decryptoKey[  7 ] = _mm_aesimc_si128( encryptoKey[  5 ] );
	decryptoKey[  6 ] = _mm_aesimc_si128( encryptoKey[  6 ] );
	decryptoKey[  5 ] = _mm_aesimc_si128( encryptoKey[  7 ] );
	decryptoKey[  4 ] = _mm_aesimc_si128( encryptoKey[  8 ] );
	decryptoKey[  3 ] = _mm_aesimc_si128( encryptoKey[  9 ] );
	decryptoKey[  2 ] = _mm_aesimc_si128( encryptoKey[ 10 ] );
	decryptoKey[  1 ] = _mm_aesimc_si128( encryptoKey[ 11 ] );
	decryptoKey[  0 ] = encryptoKey[ 12 ];
}

inline void
AES_decrypto_key_256(
	__m128i* encryptoKey
,	__m128i* decryptoKey
) {
	decryptoKey[ 14 ] = encryptoKey[  0 ];
	decryptoKey[ 13 ] = _mm_aesimc_si128( encryptoKey[  1 ] );
	decryptoKey[ 12 ] = _mm_aesimc_si128( encryptoKey[  2 ] );
	decryptoKey[ 11 ] = _mm_aesimc_si128( encryptoKey[  3 ] );
	decryptoKey[ 10 ] = _mm_aesimc_si128( encryptoKey[  4 ] );
	decryptoKey[  9 ] = _mm_aesimc_si128( encryptoKey[  5 ] );
	decryptoKey[  8 ] = _mm_aesimc_si128( encryptoKey[  6 ] );
	decryptoKey[  7 ] = _mm_aesimc_si128( encryptoKey[  7 ] );
	decryptoKey[  6 ] = _mm_aesimc_si128( encryptoKey[  8 ] );
	decryptoKey[  5 ] = _mm_aesimc_si128( encryptoKey[  9 ] );
	decryptoKey[  4 ] = _mm_aesimc_si128( encryptoKey[ 10 ] );
	decryptoKey[  3 ] = _mm_aesimc_si128( encryptoKey[ 11 ] );
	decryptoKey[  2 ] = _mm_aesimc_si128( encryptoKey[ 12 ] );
	decryptoKey[  1 ] = _mm_aesimc_si128( encryptoKey[ 13 ] );
	decryptoKey[  0 ] = encryptoKey[ 14 ];
}

inline void
AES_ECB_encrypto(
	unsigned char*	in
,	unsigned char*	out
,	int				nBlocks
,	__m128i*		key
,	int				Nr
) {
	for ( auto i = 0; i < nBlocks; i++ ) {
		__m128i $ = _mm_xor_si128( _mm_loadu_si128( (__m128i*)in + i ), key[ 0 ] );
		for ( auto j = 1; j < Nr; j++ ) $ = _mm_aesenc_si128( $, key[ j ] );
		_mm_storeu_si128( (__m128i*)out + i, _mm_aesenclast_si128( $, key[ Nr ] ) );
	}
}

inline void
AES_ECB_decrypto(
	unsigned char*	in
,	unsigned char*	out
,	int				nBlocks
,	__m128i*		key
,	int				Nr
) {
	for ( auto i = 0; i < nBlocks; i++ ) {
		__m128i $ = _mm_xor_si128( _mm_loadu_si128( (__m128i*)in + i ), key[ 0 ] );
		for ( auto j = 1; j < Nr; j++ ) $ = _mm_aesdec_si128( $, key[ j ] );
		_mm_storeu_si128( (__m128i*)out + i, _mm_aesdeclast_si128( $, key[ Nr ] ) );
	}
}

inline void
AES_CBC_encrypto(
	unsigned char*	in
,	unsigned char*	out
,	__m128i*		ivec
,	int				nBlocks
,	__m128i*		key
,	int				Nr
) {
	for ( auto i = 0; i < nBlocks; i++ ) {
		__m128i $ = _mm_xor_si128( 
			_mm_xor_si128( _mm_loadu_si128( (__m128i*)in + i ), *ivec )
		,	key[ 0 ]
		);
		for ( auto j = 1; j < Nr; j++ ) $ = _mm_aesenc_si128( $, key[ j ] );
		$ = _mm_aesenclast_si128( $, key[ Nr ] );
		_mm_storeu_si128( (__m128i*)out + i, $ );
		*ivec = $;
	}
}

inline void
AES_CBC_decrypto(
	unsigned char*	in
,	unsigned char*	out
,	__m128i*		ivec
,	int				nBlocks
,	__m128i*		key
,	int				Nr
) {
	for ( auto i = 0; i < nBlocks; i++ ) {
		__m128i _ = _mm_loadu_si128( (__m128i*)in + i );
		__m128i $ = _mm_xor_si128( _, key[ 0 ] );
		for ( auto j = 1; j < Nr; j++ ) $ = _mm_aesdec_si128( $, key[ j ] );
		_mm_storeu_si128(
			(__m128i*)out + i
		,	_mm_xor_si128( 
				_mm_aesdeclast_si128( $, key[ Nr ] )
			,	*ivec
			)
		);
		*ivec = _;
	}
}

inline void
AES_CTR_crypto(
	unsigned char*	in
,	unsigned char*	out
,	__m128i*		counter
,	int				nBlocks
,	__m128i*		key
,	int				Nr
) {

	static __m128i	ONE	= _mm_set_epi8( 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 );
	static __m128i	SWP	= _mm_set_epi8( 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 );

	for ( auto i = 0; i < nBlocks; i++ ) {
		__m128i $ = _mm_xor_si128( *counter, key[ 0 ] );
		for ( auto j = 1; j < Nr; j++ ) $ = _mm_aesenc_si128( $, key[ j ] );
		_mm_storeu_si128(
			(__m128i*)out + i
		,	_mm_xor_si128( 
				_mm_aesenclast_si128( $, key[ Nr ] )
			,	_mm_loadu_si128( (__m128i*)in + i )
			)
		);
		*counter = _mm_shuffle_epi8( _mm_add_epi64( _mm_shuffle_epi8( *counter, SWP ), ONE ), SWP );
	}
}

#define cpuid( func, ax, bx, cx, dx ) __asm__ __volatile__ ( "cpuid": "=a" (ax), "=b" (bx), "=c" (cx), "=d" (dx) : "a" (func) )
inline int
Check_CPU_support_AES() {
	unsigned int a,b,c,d;
	cpuid( 1, a, b, c, d );
	return ( c & 0x2000000 );
}



