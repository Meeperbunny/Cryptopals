#ifndef AES128
#define AES128

#include <vector>
#include <cstddef>

#include "lib/bytestring.h"

namespace aes128 {
    extern std::vector<std::byte> sbox, isbox;
    void initSbox();
    void initISbox();
    std::byte sboxGet(std::byte);
    std::byte isboxGet(std::byte);
    const int rounds = 10;
    Bytestring nextRoundKey(Bytestring &prevKey, int round);
    Bytestring roundConstant(int round);
    Bytestring g(Bytestring b, int round);
};

#endif /* AES128 */