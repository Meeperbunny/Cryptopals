#include <iostream>
#include <string>
#include <assert.h>

#include "lib/bytestring.h"
#include "lib/base64.h"

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

int main() {
    Challenge1();
    Challenge2();
}