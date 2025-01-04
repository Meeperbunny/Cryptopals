#ifndef BYTESTRING
#define BYTESTRING

#include <cstddef>
#include <vector>
#include <string>

class Bytestring {
private:
    std::vector<std::byte> m_data;
    int m_base;
public:
    Bytestring();
    Bytestring(int base);
    Bytestring(int base, int size);
    Bytestring(int base, Bytestring &other);
    Bytestring(int base, std::vector<std::byte> &data);
    inline int base() const { return m_base; }
    inline size_t size() const { return m_data.size(); }
    std::byte& operator[](int i) { return m_data[i]; }
    Bytestring operator^(Bytestring &other);
    Bytestring circularLeftShift(int n);
    Bytestring circularRightShift(int n);
    std::string toHexString();
    std::string toAsciiString();
    Bytestring substring(int i, int sz);
    void extend(Bytestring &other);
    void extend(std::byte &other);
    void pad(std::byte padding, int multiple);
};

Bytestring BytestringFromString(std::string s);
Bytestring BytestringFromHex(std::string hexString);

#endif /* BYTESTRING */