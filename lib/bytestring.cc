#include "lib/bytestring.h"
#include "lib/utils.h"

#include <iostream>
#include <algorithm>

Bytestring::Bytestring() 
    : m_base(8) {}

Bytestring::Bytestring(int base) 
    : m_base(base) {}

Bytestring::Bytestring(int base, int size) 
    : m_base(base), m_data(std::vector<std::byte>(size)) {}

Bytestring::Bytestring(int base, Bytestring &other) 
    : m_base(base) {
    m_data.clear();
    int cnt = 0;
    for(const auto &byte : other.m_data) {
        for(int i = 0; i < other.m_base; ++i) {
            if (cnt == 0) {
                m_data.push_back(std::byte{});
            }
            std::byte bit = (byte >> (other.m_base - i - 1)) & std::byte{1};
            m_data.back() |= bit << (base - cnt - 1);
            cnt = (cnt + 1) % base;
        }
    }
}

Bytestring::Bytestring(int base, std::vector<std::byte> &data) 
    : m_base(base), m_data(data) {}

Bytestring BytestringFromHex(std::string hexString) {
    std::vector<std::byte> data;
    for(const auto &c : hexString) {
        data.push_back(utils::HexToByte(c));
    }
    Bytestring hexBytestring(4, data);
    return Bytestring(8, hexBytestring);
}

Bytestring BytestringFromString(std::string s) {
    std::vector<std::byte> data;
    for(const auto &c : s) {
        data.push_back(std::byte(c));
    }
    return Bytestring(8, data);
}

Bytestring Bytestring::operator^(Bytestring &other) {
    Bytestring a(8, *this), b(8, other);
    if (a.size() < b.size()) std::swap(a, b);

    for(int i = 0; i < b.size(); ++i) {
        a[i] ^= b[i];
    }

    return a;
}

std::string Bytestring::toHexString() {
    Bytestring b(4, *this);
    std::string hex{};
    for(int i = 0; i < b.size(); ++i) {
        hex += utils::IntToHexChar(std::to_integer<int>(b[i]));
    }
    return hex;
}

std::string Bytestring::toAsciiString() {
    std::string s{};
    for(int i = 0; i < m_data.size(); ++i) {
        s += std::to_integer<unsigned char>(m_data[i]);
    }
    return s;
}
