#include <iostream>
#include <string>
#include <assert.h>
#include <cstddef>
#include <map>
#include <fstream>
#include <limits.h>

#include "lib/bytestring.h"
#include "lib/base64.h"
#include "lib/frequency.h"
#include "lib/utils.h"

void Challenge1() {
    std::string input = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
    std::string answer = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t";

    auto bs = BytestringFromHex(input);
    std::string s = base64::Encode(bs, false);

    assert(s == answer);

    std::cout << "Challenge one passed!" << std::endl;
}

void Challenge2() {
    std::string inputA = "1c0111001f010100061a024b53535009181c";
    std::string inputB = "686974207468652062756c6c277320657965";
    std::string answer = "746865206b696420646f6e277420706c6179";

    auto bsa = BytestringFromHex(inputA);
    auto bsb = BytestringFromHex(inputB);

    std::string s = (bsa ^ bsb).toHexString();

    assert(s == answer);

    std::cout << "Challenge two passed!" << std::endl;
}

void Challenge3() {
    std::string input = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";

    std::cout << "Challenge three result string:\n\t" << "\"" << frequency::singleCharXORDecrypt(BytestringFromHex(input)).first << "\"" << std::endl;
}

void Challenge4() {
    std::ifstream fin("C:/Users/Ian McKibben/Cryptopals/set1/4.txt");
    assert(fin.is_open());
    std::string line;

    std::string bests;
    double bestscore = DBL_MAX;
    while(std::getline(fin, line)) {
        auto [s, score] = frequency::singleCharXORDecrypt(BytestringFromHex(line));
        if (!s.empty()) {
            if (score < bestscore) {
                bestscore = score;
                bests = s;
            }
        }
    }
    std::cout << "Challenge four result string:\n\t" << "\"" << bests << "\"" << std::endl;
}

int main() {
    Challenge1();
    Challenge2();
    Challenge3();
    Challenge4();
}