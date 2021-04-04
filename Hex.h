inline unsigned char
HexNum( const char $ ) {
	switch ( $ ) {
	case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':	return $ - '0';
	case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':											return $ - 'A' + 10;
	case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':											return $ - 'a' + 10;
	default:	throw "eh?";
	}
}
inline vector< unsigned char >
DecodeHex( const string& $ ) {
	vector< unsigned char >	$$;
	for ( auto _ = 0; _ < $.size(); _ += 2 ) $$.emplace_back( HexNum( $[ _ ] ) << 4 | HexNum( $[ _ + 1 ] ) );
	return $$;
}

inline char
HexChar( unsigned char $ ) {
	switch ( $ ) {
	case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9:
		return $ + '0';
	case 10: case 11: case 12: case 13: case 14: case 15:
		return $ - 10 + 'a';
	default:	throw "eh?";
	}
}
inline string
HexStr( unsigned char $ ) {
	char _[] = {
		HexChar( ( $ >> 4 ) & 0x0f )
	,	HexChar( $ & 0x0f )
	,	0
	};
	return string( _ );
}
inline string
EncodeHex( const vector< unsigned char >& $ ) {
	string _;
	for ( auto& $: $ ) _ += HexStr( $ );
	return _;
}
inline string
EncodeHex( unsigned char* $, int length ) {
	vector< unsigned char >	_( length );
	for ( auto i = 0; i < length; i++ ) _[ i ] = $[ i ];
	return EncodeHex( _ );
}
