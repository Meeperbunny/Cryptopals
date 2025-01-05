#ifndef AES128
#define AES128

#include <vector>
#include <cstddef>

#include "lib/bytestring.h"

namespace aes128 {
    const int blockSize = 4;
    const int rounds = 10;
    constexpr auto padding = std::byte(0x04);
    typedef std::vector<Bytestring> Block;
    inline Block emptyBlock() {
        auto emptyVec = std::vector(4, std::byte{});
        return Block(4, Bytestring(8, emptyVec));
    };
    extern std::vector<std::byte> sbox, isbox;
    void initSbox();
    void initISbox();
    std::byte sboxGet(std::byte);
    std::byte isboxGet(std::byte);
    Bytestring nextRoundKey(Bytestring &prevKey, int round);
    Bytestring roundConstant(int round);
    Bytestring g(Bytestring b, int round);
    Bytestring blockVectorToBytestring(const std::vector<Block> &blocks);
    std::vector<Block> bytestringToBlockVector(const Bytestring &text);
    Bytestring randomKey();
    Bytestring randomModeKeyEncryption(const Bytestring &text);
    Bytestring removePadding(const Bytestring &b);

    // AES STEPS
    void subByteEncode(Block &b);
    void subByteDecode(Block &b);
    void shiftRowEncode(Block &b);
    void shiftRowDecode(Block &b);
    void mixColumnsEncode(Block &b);
    void mixColumnsDecode(Block &b);
    std::vector<Bytestring> expandKeys(Bytestring &key);
    void addRoundKey(aes128::Block &b, Bytestring &key);
    void encodeBlock(Block &b, std::vector<Bytestring> &expandedKeys);
    void decodeBlock(Block &b, std::vector<Bytestring> &expandedKeys);

    // ECB Mode
    Bytestring EncodeECB(const Bytestring &text, Bytestring key);
    Bytestring DecodeECB(const Bytestring &text, Bytestring key);

    // CBC Mode
    Bytestring EncodeCBC(const Bytestring &text, Bytestring key, Bytestring IV);
    Bytestring DecodeCBC(const Bytestring &text, Bytestring key, Bytestring IV);
};

#endif /* AES128 */