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
    Bytestring(int base, const Bytestring &other);
    Bytestring(int base, const std::vector<std::byte> &data);
    inline int base() const { return m_base; }
    inline size_t size() const { return m_data.size(); }
    inline bool empty() const { return m_data.empty(); }
    inline std::byte back() const { return m_data.back(); }
    inline void pop_back() { return m_data.pop_back(); }
    std::byte& operator[](int i) { return m_data[i]; }
    std::byte const operator[](int i) const { return m_data[i]; }
    Bytestring operator^(const Bytestring &other);
    Bytestring operator+(const Bytestring &other);
    bool operator==(const Bytestring &other);
    Bytestring circularLeftShift(int n);
    Bytestring circularRightShift(int n);
    std::string toHexString();
    std::string toAsciiString();
    Bytestring substring(int i, int sz) const;
    void extend(Bytestring &other);
    void extend(const std::byte &other);
    static Bytestring FromString(const std::string &s);
    static Bytestring FromHex(const std::string &hexString);
};

#endif /* BYTESTRING */