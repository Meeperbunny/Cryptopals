#ifndef UTILS
#define UTILS

#include <iostream>
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
    inline int PolyAdd(int a, int b) { return a ^ b; }
    inline int PolyMult(int a, int b) {
        int c = 0;
        for(int i = 0; i < 31; ++i) {
            if ((b >> i) & 1) {
                c = PolyAdd(c, a << i);
            }
        }
        // Rough check to try and catch when we multiply numbers that are too big.
        assert(!(c >> 31));
        return c;
    }

    // Taken from https://en.wikipedia.org/wiki/Rijndael_MixColumns
    inline std::byte GalMult(std::byte a, std::byte b) { // Galois Field (256) Multiplication of two Bytes
        int A = int(a);
        int B = int(b);
        int P = 0;
        for (int counter = 0; counter < 8; ++counter) {
            if ((B & 1) != 0) {
                P ^= A;
            }
            bool hi_bit_set = (A & 0x80) != 0;
            A <<= 1;
            if (hi_bit_set) {
                A ^= 0x1B; /* x^8 + x^4 + x^3 + x + 1 */
            }
            B >>= 1;
        }
        return std::byte(P);
    }
} /* utils */

#endif /* UTILS */