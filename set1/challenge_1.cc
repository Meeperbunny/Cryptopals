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

int main() {
    Challenge1();
}