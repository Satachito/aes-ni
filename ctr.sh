g++ -std=c++2a -maes -no-pie ctr_main.cpp ctr.s key_expansion.s -o ctr
./ctr
rm ctr

