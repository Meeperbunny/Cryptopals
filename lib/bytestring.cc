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

Bytestring::Bytestring(int base, const Bytestring &other) 
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

Bytestring::Bytestring(int base, const std::vector<std::byte> &data) 
    : m_base(base), m_data(data) {}

Bytestring Bytestring::FromHex(const std::string &hexString) {
    std::vector<std::byte> data;
    for(const auto &c : hexString) {
        data.push_back(utils::HexToByte(c));
    }
    Bytestring hexBytestring(4, data);
    return Bytestring(8, hexBytestring);
}

Bytestring Bytestring::FromString(const std::string &s) {
    std::vector<std::byte> data;
    for(const auto &c : s) {
        data.push_back(std::byte(c));
    }
    return Bytestring(8, data);
}

Bytestring Bytestring::operator^(const Bytestring &other) {
    Bytestring a(8, *this), b(8, other);
    if (a.size() < b.size()) std::swap(a, b);

    for(int i = 0; i < b.size(); ++i) {
        a[i] ^= b[i];
    }

    return a;
}

Bytestring Bytestring::operator+(const Bytestring &other) {
    Bytestring ret(m_base);
    assert(m_base == other.m_base);
    ret.m_data.insert(ret.m_data.end(), m_data.begin(), m_data.end());
    ret.m_data.insert(ret.m_data.end(), other.m_data.begin(), other.m_data.end());
    return ret;
}

bool Bytestring::operator==(const Bytestring &other) {
    if (m_data.size() != other.size() ||
        m_base != other.base()) return false;

    for(int i = 0; i < this->size(); ++i) {
        if (m_data[i] != m_data[i])
            return false;
    }
    return true;
}

Bytestring Bytestring::circularLeftShift(int n) {
    Bytestring b(*this);
    int sz = b.m_data.size();
    if (!sz) return b;
    n %= sz;
    // Save the first n because they will be overwritten.
    std::vector<std::byte> start;
    for(int i = 0; i < n; ++i) {
        start.push_back(m_data[i]);
    }
    for(int i = 0; i < sz - n; ++i) {
        b.m_data[i] = b.m_data[(i + n) % sz];
    }
    for(int i = 0; i < n; ++i) {
        b.m_data[sz - n + i] = start[i];
    }
    return b;
}

Bytestring Bytestring::circularRightShift(int n) {
    Bytestring b(*this);
    int sz = b.m_data.size();
    if (!sz) return b;
    n %= sz;
    // Save the first n because they will be overwritten.
    std::vector<std::byte> start;
    for(int i = sz - 1; i >= sz - n; --i) {
        start.push_back(b.m_data[i]);
    }
    for(int i = sz - 1; i >= n; --i) {
        b.m_data[i] = b.m_data[(i - n) % sz];
    }
    for(int i = n - 1; i >= 0; --i) {
        b.m_data[i] = start[n - i - 1];
    }
    return b;
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

Bytestring Bytestring::substring(int i, int sz) const {
    assert(i + sz <= m_data.size());
    std::vector<std::byte> data(m_data.begin() + i, m_data.begin() + i + sz);
    return Bytestring(m_base, data);
}

void Bytestring::extend(Bytestring &other) {
    assert(m_base == other.m_base);
    m_data.insert(m_data.end(), other.m_data.begin(), other.m_data.end());
}

void Bytestring::extend(const std::byte &other) {
    m_data.push_back(other);
}
