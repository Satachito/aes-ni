echo ECB
g++ -DECB -std=c++17 -maes Intrinsic.cpp
rm /tmp/x /tmp/y /tmp/z
./a.out 0bee89b07a248e27c83fc3d5951213c1 < Intrinsic.cpp > /tmp/x
diff Intrinsic.cpp /tmp/x
./a.out ba77240f69ce746e8dd7d7579107959ea2a64828e6f9d7f1 < Intrinsic.cpp > /tmp/y
diff Intrinsic.cpp /tmp/y
./a.out fed79e6c15588f7e2e219baca34c93332dc65dabe6ba231272bec8f981db4595 < Intrinsic.cpp > /tmp/z
diff Intrinsic.cpp /tmp/z
rm a.out

echo CBC
g++ -DCBC -std=c++17 -maes Intrinsic.cpp
rm /tmp/x /tmp/y /tmp/z
./a.out 0bee89b07a248e27c83fc3d5951213c1 < Intrinsic.cpp > /tmp/x
diff Intrinsic.cpp /tmp/x
./a.out ba77240f69ce746e8dd7d7579107959ea2a64828e6f9d7f1 < Intrinsic.cpp > /tmp/y
diff Intrinsic.cpp /tmp/y
./a.out fed79e6c15588f7e2e219baca34c93332dc65dabe6ba231272bec8f981db4595 < Intrinsic.cpp > /tmp/z
diff Intrinsic.cpp /tmp/z
rm a.out

echo CTR
g++ -DCTR -std=c++17 -maes -msse4.1 Intrinsic.cpp
rm /tmp/x /tmp/y /tmp/z
./a.out 0bee89b07a248e27c83fc3d5951213c1 < Intrinsic.cpp > /tmp/x
diff Intrinsic.cpp /tmp/x
./a.out ba77240f69ce746e8dd7d7579107959ea2a64828e6f9d7f1 < Intrinsic.cpp > /tmp/y
diff Intrinsic.cpp /tmp/y
./a.out fed79e6c15588f7e2e219baca34c93332dc65dabe6ba231272bec8f981db4595 < Intrinsic.cpp > /tmp/z
diff Intrinsic.cpp /tmp/z
rm a.out
