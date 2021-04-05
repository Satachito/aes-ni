# STATUS

CTR mode IN PROGRESS!

# Abstract

This repository is an implementation of the paper below:

[Intel® Advanced Encryption Standard (AES) New Instructions Set](https://www.intel.com/content/dam/doc/white-paper/advanced-encryption-standard-new-instructions-set-paper.pdf)

# Env

## Assembler

### Run on Ubuntu (GCP)

model name	: Intel(R) Xeon(R) CPU

flags		: fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush mmx fxsr sse sse2 ss ht syscall nx pdpe1gb rdtscp lm constant_tsc rep_good nopl xtopology nonstop_tsc cpuid tsc_known_freq pni pclmulqdq ssse3 fma cx16 pcid sse4_1 sse4_2 x2apic movbe popcnt aes xsave avx f16c rdrand hypervisor lahf_lm abm 3dnowprefetch invpcid_single ssbd ibrs ibpb stibp ibrs_enhanced fsgsbase tsc_adjust bmi1 hle avx2 smep bmi2 erms invpcid rtm mpx avx512f avx512dq rdseed adx smap clflushopt clwb avx512cd avx512bw avx512vl xsaveopt xsavec xgetbv1 xsaves arat avx512_vnni md_clear arch_capabilities


### Run on Amazon Linux

model name	: Intel(R) Xeon(R) CPU E5-2686 v4 @ 2.30GHz

flags		: fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush mmx fxsr sse sse2 ht syscall nx rdtscp lm constant_tsc rep_good nopl xtopology cpuid pni pclmulqdq ssse3 fma cx16 pcid sse4_1 sse4_2 x2apic movbe popcnt tsc_deadline_timer aes xsave avx f16c rdrand hypervisor lahf_lm abm cpuid_fault invpcid_single pti fsgsbase bmi1 avx2 smep bmi2 erms invpcid xsaveopt

## Intrinsic

### MacOS
### Ubuntu 20.04LTS
### Amazon Linux


# Usage:



```c++
#include	"Intrinsic.h"
#define	Nr	10
```

### Encode



```c++
unsigned char 
providedKey[ 16 ];

ALIGN16	__m128i
encryptKey[ Nr + 1 ];
AES_128_Key_Expansion( (__m128i*)providedKey, encryptKey );

#define	BUF_SIZE	1024
UI1	buffer[ BUF_SIZE ];
while ( true ) {
	auto numRead = (size_t)read( 0, buffer, BUF_SIZE );
	if ( !numRead ) break;
	UI1	encoded[ numRead ];
	AES_ECB_encrypt(
		buffer
	,	encoded
	,	numRead
	,	encryptKey
	,	Nr
	);
	write( 1, encoded, numRead );
}

```

### Decode

```c++
unsigned char 
providedKey[ 16 ];

ALIGN16	__m128i
encryptKey[ Nr + 1 ];
AES_128_Key_Expansion( (__m128i*)providedKey, encryptKey );

ALIGN16	__m128i
decryptKey[ Nr + 1 ];
AES_decrypt_key_128( encryptKey, decryptKey ); 

#define	BUF_SIZE	1024
UI1	buffer[ BUF_SIZE ];
while ( true ) {
	auto numRead = (size_t)read( 0, buffer, BUF_SIZE );
	if ( !numRead ) break;
	UI1	decoded[ numRead ];
	AES_ECB_decrypt(
		buffer
	,	decoded
	,	numRead
	,	decryptKey
	,	Nr
	);
	write( 1, decoded, numRead );
}


```

# Functions

```
inline void
AES_128_Key_Expansion( __m128i* userkey, __m128i* key );
inline void
AES_decrypt_key_128(
	__m128i* encryptKey
,	__m128i* decryptKey
);

inline void
AES_192_Key_Expansion( __m128i* userkey, __m128i* key );
inline void
AES_decrypt_key_192(
	__m128i* encryptKey
,	__m128i* decryptKey
);

inline void
AES_256_Key_Expansion( __m128i* userkey, __m128i* key );
inline void
AES_decrypt_key_256(
	__m128i* encryptKey
,	__m128i* decryptKey
);

inline void
AES_ECB_encrypt(
	UI1*		in		//pointer to the PLAINTEXT
,	UI1*		out		//pointer to the CIPHERTEXT buffer
,	size_t		length	//text length in bytes
,	__m128i*	key		//pointer to the expanded key schedule
,	int			Nr		//number of AES rounds 10,12 or 14
);
inline void
AES_ECB_decrypt(
	UI1*		in		//pointer to the CIPHERTEXT
,	UI1*		out		//pointer to the DECRYPTED TEXT buffer
,	size_t		length	//text length in bytes
,	__m128i*	key		//pointer to the expanded key schedule
,	int			Nr		//number of AES rounds 10,12 or 14
);

inline void
AES_CBC_encrypt(
	UI1*		in
,	UI1*		out
,	UI1*		ivec
,	size_t		length
,	__m128i*	key
,	int			Nr
);
inline void
AES_CBC_decrypt(
	UI1*		in
,	UI1*		out
,	UI1*		ivec
,	size_t		length
,	__m128i*	key
,	int			Nr
);

inline void
AES_CTR_crypt(
	UI1*		in
,	UI1*		out
,	UI1*		ivec
,	UI1*		nonce
,	size_t		length
,	__m128i*	key
,	int			Nr
);

inline int
Check_CPU_support_AES();

```

