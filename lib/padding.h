#ifndef PADDING
#define PADDING

#include "lib/bytestring.h"

namespace PKCS {
    const int multiple = 16;
    void Pad(Bytestring &b, int multiple = PKCS::multiple);
    void Unpad(Bytestring &b, int multiple = PKCS::multiple);
}

#endif /* PADDING */