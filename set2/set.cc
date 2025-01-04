#include <iostream>
#include <string>
#include <assert.h>

#include "lib/bytestring.h"

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

int main() {
    Challenge1();
}
