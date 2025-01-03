#include "lib/bytestring.h"
#include "lib/base64.h"

#include <cstddef>

char base64::lookup[64] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3',
    '4', '5', '6', '7', '8', '9', '+', '/',
};

std::string base64::Encode(Bytestring b, bool padding /* true */) {
    if (b.base() != 6) {
        b = Bytestring(6, b);
    }
    std::string encoded{};
    for(int i = 0; i < b.size(); ++i) {
        // For each byte, try to encode it using the table.
        encoded += lookup[std::to_integer<int>(b[i])];
    }
    // Pad the string if it is not a multiple of 3.
    while (padding && encoded.size() % 3)
        encoded += "=";
    return encoded;
}

Bytestring base64::Decode(std::string s) {
    return Bytestring();
}