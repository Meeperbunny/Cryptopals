#include "lib/repeatingxor.h"

Bytestring repeatingxor::Transform(Bytestring text, Bytestring key) {
    int keycounter = 0;
    for(int i = 0; i < text.size(); ++i) {
        text[i] ^= key[keycounter];
        keycounter = (keycounter + 1) % key.size();
    }
    return text;
}