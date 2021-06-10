//	Apr. 2021 Written by Satoru Ogura.	
//	https://www.intel.com/content/dam/doc/white-paper/advanced-encryption-standard-new-instructions-set-paper.pdf

#include	"AES-NI.h"

using namespace std;
#include	<iostream>
#include	<string>
#include	<vector>
#include	"Hex.h"

#include	<unistd.h>

void
Main( int argc, char** argv ) {

	if ( !Check_CPU_support_AES() ) {
		cerr << "This program needs AES support" << endl;
		exit( 2 );
	}

	if ( argc < 3 ) {
		cerr << "Usage: " << argv[ 0 ] << " key(in hex string) IV(in hex string)" << endl;
		exit( 1 );
	}

	auto key	= DecodeHex( argv[ 1 ] ); 
	auto IV		= DecodeHex( argv[ 2 ] ); 
	auto size	= argc > 3 ? atoi( argv[ 3 ] ) : 0;

	auto nBits	= key.size() * 8;
	auto Nk		= nBits / 32;
	auto Nr		= Nk + 6;

	ALIGN16	__m128i
	encryptoKey[ Nr + 1 ];

	switch ( nBits ) {
	case 128:	AES_128_Key_Expansion( (__m128i*)&key[ 0 ], encryptoKey );	break;
	case 192:	AES_192_Key_Expansion( (__m128i*)&key[ 0 ], encryptoKey );	break;
	case 256:	AES_256_Key_Expansion( (__m128i*)&key[ 0 ], encryptoKey );	break;
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

#if	( defined( ECB ) || defined( CBC ) ) && defined( DEC )
	ALIGN16	__m128i
	decryptoKey[ Nr + 1 ];

	switch ( nBits ) {
	case 128:	AES_decrypto_key_128( encryptoKey, decryptoKey ); break;
	case 192:	AES_decrypto_key_192( encryptoKey, decryptoKey ); break;
	case 256:	AES_decrypto_key_256( encryptoKey, decryptoKey ); break;
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
		unsigned char	buffer[ BUF_SIZE ];
		auto nRead = (size_t)read( 0, buffer, BUF_SIZE );
		if ( !nRead ) break;
		auto nBlocks = ( nRead + 15 ) / 16;
		auto nCrypto = nBlocks * 16;
		unsigned char	coded[ nCrypto ];

#ifdef	CTR
		cerr << EncodeHex( IV ) << endl;
		AES_CTR_crypto(
			buffer
		,	coded
		,	(__m128i*)&IV[ 0 ]
		,	nBlocks
		,	encryptoKey
		,	Nr
		);
#endif
#ifdef	CBC
	cerr << EncodeHex( IV ) << endl;
	#ifdef	ENC
		AES_CBC_encrypto(
			buffer
		,	coded
		,	(__m128i*)&IV[ 0 ]
		,	nBlocks
		,	encryptoKey
		,	Nr
		);
	#endif
	#ifdef	DEC
		AES_CBC_decrypto(
			buffer
		,	coded
		,	(__m128i*)&IV[ 0 ]
		,	nBlocks
		,	decryptoKey
		,	Nr
		);
	#endif
#endif
#ifdef	ECB
	#ifdef	ENC
		AES_ECB_encrypto(
			buffer
		,	coded
		,	nBlocks
		,	encryptoKey
		,	Nr
		);
	#endif
	#ifdef	DEC
		AES_ECB_decrypto(
			buffer
		,	coded
		,	nBlocks
		,	decryptoKey
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

