
#include "lib/aes128.h"
#include "lib/bytestring.h"
#include "lib/utils.h"

#include <stdint.h>
#include <vector>
#include <cstddef>
#include <assert.h>

std::vector<std::byte> aes128::sbox{};
std::vector<std::byte> aes128::isbox{};

// Taken from https://en.wikipedia.org/wiki/Rijndael_S-box
#define ROTL8(x,shift) ((uint8_t) ((x) << (shift)) | ((x) >> (8 - (shift))))
void aes128::initSbox() {
    if (!sbox.empty()) return;
    aes128::sbox = std::vector<std::byte>(256, std::byte{});
	unsigned char p = 1, q = 1;
	/* loop invariant: p * q == 1 in the Galois field */
	do {
		/* multiply p by 3 */
		p = p ^ (p << 1) ^ (p & 0x80 ? 0x1B : 0);
		/* divide q by 3 (equals multiplication by 0xf6) */
		q ^= q << 1;
		q ^= q << 2;
		q ^= q << 4;
		q ^= q & 0x80 ? 0x09 : 0;
		/* compute the affine transformation */
		uint8_t xformed = q ^ ROTL8(q, 1) ^ ROTL8(q, 2) ^ ROTL8(q, 3) ^ ROTL8(q, 4);
		aes128::sbox[p] = std::byte(xformed ^ 0x63);
	} while (p != 1);
	/* 0 is a special case since it has no inverse */
	aes128::sbox[0] = std::byte(0x63);
}

void aes128::initISbox() {
    if (!isbox.empty()) return;
    aes128::isbox = std::vector<std::byte>(256, std::byte{});
    aes128::sboxGet(std::byte{}); // Constructs the sbox if not made.

    // isbox[i] = j for all sbox[j] = i.
    for(int i = 0; i < 256; i++) {
        isbox[std::to_integer<int>(sboxGet(std::byte(i)))] = std::byte(i);
    }
}

std::byte aes128::sboxGet(std::byte b) {
    if (aes128::sbox.empty())
        aes128::initSbox();

    return aes128::sbox.at(std::to_integer<int>(b));
}

std::byte aes128::isboxGet(std::byte b) {
    if (aes128::isbox.empty())
        aes128::initISbox();

    return aes128::isbox.at(std::to_integer<int>(b));
}

Bytestring aes128::nextRoundKey(Bytestring &prevKey, int round) {
    std::vector<Bytestring> w(8, Bytestring{});
    for(int i = 0; i < 4; ++i) {
        w[i] = prevKey.substring(i * 4, 4);
    }
    
    auto gw3 = aes128::g(w[3], round);
    w[4] = w[0] ^ gw3;
    w[5] = w[1] ^ w[4];
    w[6] = w[2] ^ w[5];
    w[7] = w[3] ^ w[6];

    Bytestring key{};
    for(int i = 4; i < 8; ++i)
        key.extend(w[i]);
    
    return key;
}

Bytestring aes128::roundConstant(int round) {
    assert(1 <= round && round <= aes128::rounds);

    // TODO: Calculate this the right way.
    std::vector<int> lookup = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36 };
    
    auto vec = std::vector<std::byte>(4, std::byte{});
    vec[0] = std::byte(lookup[round - 1]);
    return Bytestring(8, vec);
}

Bytestring aes128::g(Bytestring b, int round) {
    // Left shift.
    auto bshifted = b.circularLeftShift(1);
    // Byte sub.
    for(int i = 0; i < bshifted.size(); ++i) {
        bshifted[i] = aes128::sboxGet(bshifted[i]);
    }
    // Rounding constant.
    return aes128::roundConstant(round) ^ bshifted;
}

// Maps every byte through sbox.
void aes128::subByteEncode(aes128::Block &b) {
    for(auto &bs : b) {
        for(int i = 0; i < bs.size(); ++i) {
            bs[i] = sboxGet(bs[i]);
        }
    }
}

// Maps every byte through inverse sbox.
void aes128::subByteDecode(aes128::Block &b) {
    for(auto &bs : b) {
        for(int i = 0; i < bs.size(); ++i) {
            bs[i] = isboxGet(bs[i]);
        }
    }
}

void aes128::shiftRowEncode(aes128::Block &b) {
    for(int i = 1; i < 4; i++) {
        b[i] = b[i].circularLeftShift(i);
    }
}

void aes128::shiftRowDecode(aes128::Block &b) {
    for(int i = 1; i < 4; i++) {
        b[i] = b[i].circularRightShift(i);
    }
}

void aes128::mixColumnsEncode(aes128::Block &b) {
    aes128::Block mixed = aes128::emptyBlock();
    for(int col = 0; col < aes128::blockSize; ++col) {
        mixed[0][col] = utils::GalMult(std::byte(0x02), b[0][col]) ^ utils::GalMult(std::byte(0x03), b[1][col]) ^ utils::GalMult(std::byte(0x01), b[2][col]) ^ utils::GalMult(std::byte(0x01), b[3][col]);
        mixed[1][col] = utils::GalMult(std::byte(0x01), b[0][col]) ^ utils::GalMult(std::byte(0x02), b[1][col]) ^ utils::GalMult(std::byte(0x03), b[2][col]) ^ utils::GalMult(std::byte(0x01), b[3][col]);
        mixed[2][col] = utils::GalMult(std::byte(0x01), b[0][col]) ^ utils::GalMult(std::byte(0x01), b[1][col]) ^ utils::GalMult(std::byte(0x02), b[2][col]) ^ utils::GalMult(std::byte(0x03), b[3][col]);
        mixed[3][col] = utils::GalMult(std::byte(0x03), b[0][col]) ^ utils::GalMult(std::byte(0x01), b[1][col]) ^ utils::GalMult(std::byte(0x01), b[2][col]) ^ utils::GalMult(std::byte(0x02), b[3][col]);
    }
    b = mixed;
}

void aes128::mixColumnsDecode(aes128::Block &b) {
    aes128::Block mixed = aes128::emptyBlock();
    for(int col = 0; col < aes128::blockSize; ++col) {
        mixed[0][col] = utils::GalMult(std::byte(0x0e), b[0][col]) ^ utils::GalMult(std::byte(0x0b), b[1][col]) ^ utils::GalMult(std::byte(0x0d), b[2][col]) ^ utils::GalMult(std::byte(0x09), b[3][col]);
        mixed[1][col] = utils::GalMult(std::byte(0x09), b[0][col]) ^ utils::GalMult(std::byte(0x0e), b[1][col]) ^ utils::GalMult(std::byte(0x0b), b[2][col]) ^ utils::GalMult(std::byte(0x0d), b[3][col]);
        mixed[2][col] = utils::GalMult(std::byte(0x0d), b[0][col]) ^ utils::GalMult(std::byte(0x09), b[1][col]) ^ utils::GalMult(std::byte(0x0e), b[2][col]) ^ utils::GalMult(std::byte(0x0b), b[3][col]);
        mixed[3][col] = utils::GalMult(std::byte(0x0b), b[0][col]) ^ utils::GalMult(std::byte(0x0d), b[1][col]) ^ utils::GalMult(std::byte(0x09), b[2][col]) ^ utils::GalMult(std::byte(0x0e), b[3][col]);
    }
    b = mixed;
}

std::vector<Bytestring> aes128::expandKeys(Bytestring &key) {
    std::vector<Bytestring> keys = { key };
    for(int round = 1; round <= aes128::rounds; ++round) {
        auto &prevKey = keys.back();
        keys.push_back(aes128::nextRoundKey(prevKey, round));
    }
    return keys;
}

void aes128::addRoundKey(aes128::Block &b, Bytestring &key) {
    assert(key.size() == 16);
    for(int i = 0; i < 16; i++) {
        b[i % 4][i / 4] ^= key[i];
    }
}

void aes128::encodeBlock(aes128::Block &b, std::vector<Bytestring> &keys) {
    assert(keys.size() == aes128::rounds + 1);
    // Initial key addition.
    aes128::addRoundKey(b, keys[0]);
    for(int round = 1; round <= aes128::rounds - 1; ++round) {
        aes128::subByteEncode(b);
        aes128::shiftRowEncode(b);
        aes128::mixColumnsEncode(b);
        aes128::addRoundKey(b, keys[round]);
    }
    // Final round.
    aes128::subByteEncode(b);
    aes128::shiftRowEncode(b);
    aes128::addRoundKey(b, keys[aes128::rounds]);
    return;
}

void aes128::decodeBlock(aes128::Block &b, std::vector<Bytestring> &keys) {
    // Do everything but in reverse.
    assert(keys.size() == aes128::rounds + 1);
    // Invserse of final round.
    aes128::addRoundKey(b, keys[aes128::rounds]);
    aes128::shiftRowDecode(b);
    aes128::subByteDecode(b);
    // Inverse of each round in descending order.
    for(int round = aes128::rounds - 1; round >= 1; --round) {
        aes128::addRoundKey(b, keys[round]);
        aes128::mixColumnsDecode(b);
        aes128::shiftRowDecode(b);
        aes128::subByteDecode(b);
    }
    // Finally inverse of the start.
    aes128::addRoundKey(b, keys[0]);
    return;
}

std::vector<aes128::Block> bytestringToBlockVector(Bytestring &text) {
    std::vector<aes128::Block> blocks;
    for(int i = 0, cnt = 0; i < text.size(); ++i) {
        if (cnt == 0) {
            blocks.push_back(aes128::emptyBlock());
        }
        blocks.back()[cnt % 4][cnt / 4] = text[i];
        cnt = (cnt + 1) % 16;
    }
    return blocks;
}

Bytestring blockVectorToBytestring(std::vector<aes128::Block> &blocks) {
    Bytestring encoded;
    for(auto &block : blocks) {
        for(int row = 0; row < aes128::blockSize; ++row) {
            for(int col = 0; col < aes128::blockSize; ++col) {
                encoded.extend(block[col][row]);
            }
        }
    }
    return encoded;
}

Bytestring aes128::Encode(Bytestring &text, Bytestring key) {
    assert(text.base() == 8 && key.base() == 8);
    // Seperate it into 128-bit blocks.
    auto blocks = bytestringToBlockVector(text);
    // Get keys.
    auto keys = aes128::expandKeys(key);

    // Now that we have blocks, encode each.
    for(auto &block : blocks) {
        aes128::encodeBlock(block, keys);
    }
    // Now unpack back into a bytestring.
    return blockVectorToBytestring(blocks);
}

Bytestring aes128::Decode(Bytestring &text, Bytestring key) {
    assert(text.base() == 8 && key.base() == 8);
    // Seperate it into 128-bit blocks.
    auto blocks = bytestringToBlockVector(text);
    // Get keys.
    auto keys = aes128::expandKeys(key);

    // Now that we have blocks, decode each.
    for(auto &block : blocks) {
        aes128::decodeBlock(block, keys);
    }
    // Now unpack back into a bytestring.
    return blockVectorToBytestring(blocks);
}
