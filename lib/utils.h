#ifndef UTILS
#define UTILS

#include <cstddef>

namespace utils {

    inline int HexCharToInt(char c) { return int(c >= 'a' ? c - 'a' + 10 : c - '0'); }
    std::byte HexToByte(char c) { return std::byte(HexCharToInt(c)); };

} /* utils */

#endif /* UTILS */