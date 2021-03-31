g++ -std=c++2a -maes cbc_main.cpp cbc.s key_expansion.s -o cbc
./cbc
rm cbc

