#ifndef PADDING
#define PADDING

#include "lib/bytestring.h"

namespace PKCS {
    const int multiple = 16;
    bool isValidPadding(const Bytestring &b);
    void Pad(Bytestring &b, int multiple = PKCS::multiple);
    void Unpad(Bytestring &b);
}

#endif /* PADDING */