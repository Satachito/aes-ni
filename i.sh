SIZE=`wc -c _.txt | awk '{print $1}'`

IV=f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff
K128=0bee89b07a248e27c83fc3d5951213c1
K192=ba77240f69ce746e8dd7d7579107959ea2a64828e6f9d7f1
K256=fed79e6c15588f7e2e219baca34c93332dc65dabe6ba231272bec8f981db4595

echo ECB
g++ -DECB -DENC -std=c++17 -maes -msse4.1 i.cpp -o enc
g++ -DECB -DDEC -std=c++17 -maes -msse4.1 i.cpp -o dec
./enc $K128 $IV < _.txt > /tmp/x
./dec $K128 $IV $SIZE < /tmp/x > /tmp/y
diff _.txt /tmp/y
./enc $K192 $IV < _.txt > /tmp/x
./dec $K192 $IV $SIZE < /tmp/x > /tmp/y
diff _.txt /tmp/y
./enc $K256 $IV < _.txt > /tmp/x
./dec $K256 $IV $SIZE < /tmp/x > /tmp/y
diff _.txt /tmp/y

echo CBC
g++ -DCBC -DENC -std=c++17 -maes -msse4.1 i.cpp -o enc
g++ -DCBC -DDEC -std=c++17 -maes -msse4.1 i.cpp -o dec
./enc $K128 $IV < _.txt > /tmp/x
./dec $K128 $IV $SIZE < /tmp/x > /tmp/y
diff _.txt /tmp/y
./enc $K192 $IV < _.txt > /tmp/x
./dec $K192 $IV $SIZE < /tmp/x > /tmp/y
diff _.txt /tmp/y
./enc $K256 $IV < _.txt > /tmp/x
./dec $K256 $IV $SIZE < /tmp/x > /tmp/y
diff _.txt /tmp/y

echo CTR
g++ -DCTR -DENC -std=c++17 -maes -msse4.1 i.cpp -o enc
g++ -DCTR -DDEC -std=c++17 -maes -msse4.1 i.cpp -o dec
./enc $K128 $IV < _.txt > /tmp/x
./dec $K128 $IV $SIZE < /tmp/x > /tmp/y
diff _.txt /tmp/y
./enc $K192 $IV < _.txt > /tmp/x
./dec $K192 $IV $SIZE < /tmp/x > /tmp/y
diff _.txt /tmp/y
./enc $K256 $IV < _.txt > /tmp/x
./dec $K256 $IV $SIZE < /tmp/x > /tmp/y
diff _.txt /tmp/y

rm enc dec
