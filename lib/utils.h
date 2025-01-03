#ifndef UTILS
#define UTILS

#include <cstddef>

namespace utils {

    inline int HexCharToInt(char c) { return int(c >= 'a' ? c - 'a' + 10 : c - '0'); }
    inline char IntToHexChar(int i) { return int(i >= 10 ? i + 'a' - 10 : i + '0'); }
    std::byte HexToByte(char c) { return std::byte(HexCharToInt(c)); };

} /* utils */

#endif /* UTILS */