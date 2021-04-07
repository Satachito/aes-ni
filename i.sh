
SIZE=`wc -c _.txt | awk '{print $1}'`

echo ECB
g++ -DECB -DENC -std=c++17 -maes -msse4.1 Intrinsic.cpp -o enc
g++ -DECB -DDEC -std=c++17 -maes -msse4.1 Intrinsic.cpp -o dec
./enc 0bee89b07a248e27c83fc3d5951213c1 < _.txt > /tmp/x
./dec 0bee89b07a248e27c83fc3d5951213c1 $SIZE < /tmp/x > /tmp/y
diff _.txt /tmp/y
./enc ba77240f69ce746e8dd7d7579107959ea2a64828e6f9d7f1 < _.txt > /tmp/x
./dec ba77240f69ce746e8dd7d7579107959ea2a64828e6f9d7f1 $SIZE < /tmp/x > /tmp/y
diff _.txt /tmp/y
./enc fed79e6c15588f7e2e219baca34c93332dc65dabe6ba231272bec8f981db4595 < _.txt > /tmp/x
./dec fed79e6c15588f7e2e219baca34c93332dc65dabe6ba231272bec8f981db4595 $SIZE < /tmp/x > /tmp/y
diff _.txt /tmp/y

echo CBC
g++ -DCBC -DENC -std=c++17 -maes -msse4.1 Intrinsic.cpp -o enc
g++ -DCBC -DDEC -std=c++17 -maes -msse4.1 Intrinsic.cpp -o dec
./enc 0bee89b07a248e27c83fc3d5951213c1 < _.txt > /tmp/x
./dec 0bee89b07a248e27c83fc3d5951213c1 $SIZE < /tmp/x > /tmp/y
diff _.txt /tmp/y
./enc ba77240f69ce746e8dd7d7579107959ea2a64828e6f9d7f1 < _.txt > /tmp/x
./dec ba77240f69ce746e8dd7d7579107959ea2a64828e6f9d7f1 $SIZE < /tmp/x > /tmp/y
diff _.txt /tmp/y
./enc fed79e6c15588f7e2e219baca34c93332dc65dabe6ba231272bec8f981db4595 < _.txt > /tmp/x
./dec fed79e6c15588f7e2e219baca34c93332dc65dabe6ba231272bec8f981db4595 $SIZE < /tmp/x > /tmp/y
diff _.txt /tmp/y

echo CTR
g++ -DCTR -DENC -std=c++17 -maes -msse4.1 Intrinsic.cpp -o enc
g++ -DCTR -DDEC -std=c++17 -maes -msse4.1 Intrinsic.cpp -o dec
./enc 0bee89b07a248e27c83fc3d5951213c1 < _.txt > /tmp/x
./dec 0bee89b07a248e27c83fc3d5951213c1 $SIZE < /tmp/x > /tmp/y
diff _.txt /tmp/y
./enc ba77240f69ce746e8dd7d7579107959ea2a64828e6f9d7f1 < _.txt > /tmp/x
./dec ba77240f69ce746e8dd7d7579107959ea2a64828e6f9d7f1 $SIZE < /tmp/x > /tmp/y
diff _.txt /tmp/y
./enc fed79e6c15588f7e2e219baca34c93332dc65dabe6ba231272bec8f981db4595 < _.txt > /tmp/x
./dec fed79e6c15588f7e2e219baca34c93332dc65dabe6ba231272bec8f981db4595 $SIZE < /tmp/x > /tmp/y
diff _.txt /tmp/y

rm enc dec
