#include <iostream>
#include <string>
#include <assert.h>

#include "lib/bytestring.h"
#include "lib/utils.h"
#include "lib/base64.h"
#include "lib/aes128.h"

void Challenge1() {
    std::string s = "YELLOW SUBMARINE";
    auto b = BytestringFromString(s);
    b.pad(std::byte(0x04), 20);
    for(int i = 16; i < 20; ++i) {
        assert(b[i] == std::byte(0x04));
    }
    assert(b.size() == 20);

    std::cout << "Challenge 1 complete!" << std::endl;
}

void Challenge2() {
    auto key = BytestringFromString("YELLOW SUBMARINE");
    auto IV = Bytestring(8, std::vector<std::byte>(16, std::byte(0x00)));

    // Check that it works before opening file.
    std::string cbcTest = "ABCDEFGHIJKLMNOPABCDEFGHIJKLMNOP";
    assert(cbcTest == aes128::DecodeCBC(
            aes128::EncodeCBC(BytestringFromString(cbcTest), key, IV),
            key,
            IV
        ).toAsciiString()
    );

    std::string s = utils::StringFromFile("C:/Users/Ian McKibben/Cryptopals/set2/10.txt");
    auto encoded = base64::Decode(s);

    auto bs = aes128::DecodeCBC(encoded, key, IV);
    std::string decoded = bs.toAsciiString();

    std::cout << "Challenge 2 result:" << std::endl;
    std::cout << decoded << std::endl;
}

int main() {
    Challenge1();
    Challenge2();
}
