#include "lib/padding.h"

#include "lib/bytestring.h"

#include <vector>
#include <assert.h>

bool PKCS::isValidPadding(const Bytestring &b) {
    if (b.empty()) return false;
    int paddingCount = int(b.back());
    if (paddingCount > b.size()) return false;
    for(int i = b.size() - 1; i + paddingCount >= b.size(); --i) {
        if (b[i] != b.back()) return false;
    }
    return true;
}

void PKCS::Pad(Bytestring &b, int multiple) {
    // TODO: make this cleaner.
    assert(0 < multiple && multiple <= 256);
    int size = (multiple - (b.size() % multiple));
    b = b + Bytestring(b.base(), std::vector<std::byte>(size, std::byte(size)));
}

#include <iostream>
void PKCS::Unpad(Bytestring &b) {
    // Check if it has valid padding.
    if (PKCS::isValidPadding(b)) {
        // Is good, remove all.
        int paddingCount = int(b.back());
        while(paddingCount--) {
            b.pop_back();
        }
    }
    return;
}
