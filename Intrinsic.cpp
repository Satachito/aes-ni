//	4/4/2021 Written by Satoru Ogura.	
//	https://www.intel.com/content/dam/doc/white-paper/advanced-encryption-standard-new-instructions-set-paper.pdf

#include	"Intrinsic.h"

using namespace std;
#include	<iostream>
#include	<string>
#include	<vector>
#include	"Hex.h"

#include	<unistd.h>

void
Main( int argc, char** argv ) {

	if ( argc < 2 ) {
		cerr << "Usage: " << argv[ 0 ] << " key(in hex string)" << endl;
		exit( 1 );
	}

	if ( !Check_CPU_support_AES() ) {
		cerr << "This program needs AES support" << endl;
		exit( 2 );
	}

	auto size	= argc > 2 ? atoi( argv[ 2 ] ) : 0;
	auto key	= DecodeHex( argv[ 1 ] ); 

	auto nBits	= key.size() * 8;
	auto Nk		= nBits / 32;
	auto Nr		= Nk + 6;

	ALIGN16	__m128i
	encryptKey[ Nr + 1 ];

	switch ( nBits ) {
	case 128:	AES_128_Key_Expansion( (__m128i*)&key[ 0 ], encryptKey );	break;
	case 192:	AES_192_Key_Expansion( (__m128i*)&key[ 0 ], encryptKey );	break;
	case 256:	AES_256_Key_Expansion( (__m128i*)&key[ 0 ], encryptKey );	break;
	default	:
		cerr
			<< "Key length must be 128, 192 or 256, length of the given key("
			<< argv[ 1 ]
			<< ") is "
			<< nBits
			<< endl
		;
		exit( 3 );
	}

#ifdef	CBC
	ALIGN16 UI1
	IV_CBC[]	= { 0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f };
#endif
#ifdef	CTR
	ALIGN16 UI1
	NONCE[]		= { 0x00,0x6C,0xB6,0xDB };
	ALIGN16 UI1
	IV_CTR[]	= { 0xC0,0x54,0x3B,0x59,0xDA,0x48,0xD9,0x0B };
#endif

#if	( defined( ECB ) || defined( CBC ) ) && defined( DEC )
	ALIGN16	__m128i
	decryptKey[ Nr + 1 ];

	switch ( nBits ) {
	case 128:	AES_decrypt_key_128( encryptKey, decryptKey ); break;
	case 192:	AES_decrypt_key_192( encryptKey, decryptKey ); break;
	case 256:	AES_decrypt_key_256( encryptKey, decryptKey ); break;
	}
#endif

#ifdef	BUF_SIZE
	if ( BUF_SIZE % 16 ) {
		cerr << "BUF_SIZE must be 16 * N" << endl;
		exit( 4 );
	}
#else
	#define	BUF_SIZE	1024	//	Must be 16 * N
#endif

	size_t	nBytes = 0;

	while ( true ) {
		UI1	buffer[ BUF_SIZE ];
		auto nRead = (size_t)read( 0, buffer, BUF_SIZE );
		if ( !nRead ) break;
		auto nCrypto = ( nRead + 15 ) / 16 * 16;
		UI1	coded[ nCrypto ];

#ifdef	CTR
		AES_CTR_crypt(
			buffer
		,	coded
		,	IV_CTR
		,	NONCE
		,	nCrypto
		,	encryptKey
		,	Nr
		);
cerr << EncodeHex( IV_CTR, 8 ) << endl;
	#ifdef	ENC
//		for ( auto _ = 0; _ < 16; _++ ) IV_CTR[ _ ] = coded[ nRead - 16 + _ ];
	#endif
	#ifdef	DEC
//		for ( auto _ = 0; _ < 16; _++ ) IV_CTR[ _ ] = buffer[ nCrypto - 16 + _ ];
	#endif
#endif
#ifdef	CBC
	#ifdef	ENC
		AES_CBC_encrypt(
			buffer
		,	coded
		,	IV_CBC
		,	nCrypto
		,	encryptKey
		,	Nr
		);
		for ( auto _ = 0; _ < 16; _++ ) IV_CBC[ _ ] = coded[ nRead - 16 + _ ];
	#endif
	#ifdef	DEC
		AES_CBC_decrypt(
			buffer
		,	coded
		,	IV_CBC
		,	nCrypto
		,	decryptKey
		,	Nr
		);
		for ( auto _ = 0; _ < 16; _++ ) IV_CBC[ _ ] = buffer[ nCrypto - 16 + _ ];
	#endif
#endif
#ifdef	ECB
	#ifdef	ENC
		AES_ECB_encrypt(
			buffer
		,	coded
		,	nCrypto
		,	encryptKey
		,	Nr
		);
	#endif
	#ifdef	DEC
		AES_ECB_decrypt(
			buffer
		,	coded
		,	nCrypto
		,	decryptKey
		,	Nr
		);
	#endif
#endif
		auto nWrite = size && nBytes + nCrypto > size ? size - nBytes : nCrypto;
		if ( write( 1, coded, nWrite ) != nWrite ) throw "Write Error";
		nBytes += nWrite;

//cerr << size << ':' << nRead << ':' << nCrypto << ':' << nWrite << endl;
//if ( nRead < BUF_SIZE ) cerr << EncodeHex( buffer, nCrypto ) << endl;
//if ( nRead < BUF_SIZE ) cerr << EncodeHex( coded, nCrypto ) << endl;
	}
}
  
int
main( int argc, char** argv ) {
	try { Main( argc, argv ); } catch ( const char* _ ) { cerr << _ << endl; }
}

