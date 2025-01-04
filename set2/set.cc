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
    std::string s = utils::StringFromFile("C:/Users/Ian McKibben/Cryptopals/set2/10.txt");
    auto encoded = base64::Decode(s);

    auto key = BytestringFromString("YELLOW SUBMARINE");
    auto IV = Bytestring(8, std::vector<std::byte>(16, std::byte(0x00)));

    // Seperate it into blocks.
    auto blocks = aes128::bytestringToBlockVector(encoded);
    // Get keys.
    auto keys = aes128::expandKeys(key);
    // Now that we have blocks, decode each, and xor with the IV on each.
    auto lastBlock = IV;
    for(auto &block : blocks) {
        // Get the prev before any decryption.
        auto nextBlock = aes128::blockVectorToBytestring({block});

        aes128::decodeBlock(block, keys);

        // XOR the block after because we are decrypting.
        auto currentBytestring = aes128::blockVectorToBytestring({block});
        currentBytestring = currentBytestring ^ lastBlock;
        block = aes128::bytestringToBlockVector(currentBytestring)[0];

        // Set the last block
        lastBlock = nextBlock;
    }
    // Now unpack back into a bytestring.
    auto bs = aes128::blockVectorToBytestring(blocks);
    std::cout << "Challenge 2 result:" << std::endl;
    std::cout << bs.toAsciiString() << std::endl;
}

int main() {
    Challenge1();
    Challenge2();
}
