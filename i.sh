set -eu

TMP=`mktemp -d`
trap 'rm -rf "$TMP"' EXIT

cp Nist.cpp $TMP/plain.txt
SIZE=`wc -c < $TMP/plain.txt`

IV=f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff
K128=0bee89b07a248e27c83fc3d5951213c1
K192=ba77240f69ce746e8dd7d7579107959ea2a64828e6f9d7f1
K256=fed79e6c15588f7e2e219baca34c93332dc65dabe6ba231272bec8f981db4595

for MODE in ECB CBC CTR; do
	echo $MODE
	g++ -D$MODE -DENC -std=c++17 -maes -msse4.1 i.cpp -o $TMP/enc
	g++ -D$MODE -DDEC -std=c++17 -maes -msse4.1 i.cpp -o $TMP/dec
	for KEY in $K128 $K192 $K256; do
		$TMP/enc $KEY $IV < $TMP/plain.txt > $TMP/x
		$TMP/dec $KEY $IV $SIZE < $TMP/x > $TMP/y
		diff $TMP/plain.txt $TMP/y
	done
done
echo OK
