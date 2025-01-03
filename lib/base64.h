#ifndef BASE64
#define BASE64

#include "lib/bytestring.h"

#include <string>

namespace base64 {
    extern char lookup[64];

    std::string Encode(Bytestring, bool = true);
    Bytestring Decode(std::string);
}

#endif