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

	if ( argc != 2 ) {
		cerr << "Usage: " << argv[ 0 ] << " key(in hex string)" << endl;
		exit( 1 );
	}

	if ( !Check_CPU_support_AES() ) {
		cerr << "This program needs AES support" << endl;
		exit( 2 );
	}

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

	UI1	IV[]	= { 0xC0,0x54,0x3B,0x59,0xDA,0x48,0xD9,0x0B };
#ifdef	CTR
	UI1	NONCE[]	= { 0x00,0x6C,0xB6,0xDB };
#else
	ALIGN16	__m128i
	decryptKey[ Nr + 1 ];

	switch ( nBits ) {
	case 128:	AES_decrypt_key_128( encryptKey, decryptKey ); break;
	case 192:	AES_decrypt_key_192( encryptKey, decryptKey ); break;
	case 256:	AES_decrypt_key_256( encryptKey, decryptKey ); break;
	}

#endif
	#define	BUF_SIZE	1024
	UI1	buffer[ BUF_SIZE ];
	while ( true ) {
		auto numRead = (size_t)read( 0, buffer, BUF_SIZE );
		if ( !numRead ) break;
		UI1	encoded[ numRead ];
		UI1	decoded[ numRead ];
#ifdef	CTR
		AES_CTR_crypt(
			buffer
		,	encoded
		,	IV
		,	NONCE
		,	numRead
		,	encryptKey
		,	Nr
		);
		AES_CTR_crypt(
			encoded
		,	decoded
		,	IV
		,	NONCE
		,	numRead
		,	encryptKey
		,	Nr
		);
#endif
#ifdef	CBC
		AES_CBC_encrypt(
			buffer
		,	encoded
		,	IV
		,	numRead
		,	encryptKey
		,	Nr
		);
		AES_CBC_decrypt(
			encoded
		,	decoded
		,	IV
		,	numRead
		,	decryptKey
		,	Nr
		);
#endif
#ifdef	ECB
		AES_ECB_encrypt(
			buffer
		,	encoded
		,	numRead
		,	encryptKey
		,	Nr
		);
		AES_ECB_decrypt(
			encoded
		,	decoded
		,	numRead
		,	decryptKey
		,	Nr
		);
#endif
		write( 1, decoded, numRead );
	}
}

int
main( int argc, char** argv ) {
	try { Main( argc, argv ); } catch ( const char* _ ) { cerr << _ << endl; }
}

