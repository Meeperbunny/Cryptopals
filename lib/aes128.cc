
#include "lib/aes128.h"
#include "lib/bytestring.h"

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

    // TODO(ian): Calculate this the right way.
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
