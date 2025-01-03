#ifndef UTILS
#define UTILS

#include <algorithm>
#include <bit>
#include <cstddef>
#include <string>
#include <assert.h>

#include "lib/bytestring.h"

namespace utils {

    inline int HexCharToInt(char c) { return int(c >= 'a' ? c - 'a' + 10 : c - '0'); }
    inline char IntToHexChar(int i) { return int(i >= 10 ? i + 'a' - 10 : i + '0'); }
    inline std::byte HexToByte(char c) { return std::byte(HexCharToInt(c)); };
    inline bool IsValidAsciiString(std::string s) {
        for(const auto &c : s) if (c < 0 || c > 128)
            return false;
        return true;
    };
    inline int HammingDistance(Bytestring a, Bytestring b) {
        assert(a.base() == b.base());
        int dist = 0;
        for(int i = 0; i < std::min(a.size(), b.size()); ++i) {
            dist += __popcnt(std::to_integer<int>(a[i] ^ b[i]));
        }
        return dist;
    }

} /* utils */

#endif /* UTILS */