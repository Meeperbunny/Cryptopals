#include "lib/padding.h"

#include "lib/bytestring.h"
#include <vector>

void PKCS::Pad(Bytestring &b, int multiple) {
    // TODO: make this cleaner.
    int size = (multiple - (b.size() % multiple)) % multiple;
    b = b + Bytestring(b.base(), std::vector<std::byte>(size, std::byte(size)));
}

void PKCS::Unpad(Bytestring &b, int multiple) {
    if (b.size() % multiple != 0)
        return;
    // Check if it has valid padding.
    int paddingCount = int(b.back());
    if (paddingCount >= multiple)
        return;
    for(int i = b.size() - 1; i >= b.size() - paddingCount; --i) {
        if (b[i] != b.back()) return;
    }
    // Is good, remove all.
    while(paddingCount--) {
        b.pop_back();
    }
    return;
}
