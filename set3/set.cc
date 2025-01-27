#include <assert.h>
#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <fstream>
#include <utility>

#include "lib/utils.h"
#include "lib/aes128.h"
#include "lib/bytestring.h"
#include "lib/base64.h"
#include "lib/padding.h"

void Challenge17() {
    std::ifstream fin("C:/Users/Ian McKibben/Cryptopals/set3/17.txt");
    assert(fin.is_open());
    std::vector<std::string> lines;
    std::string line{};
    while(std::getline(fin, line)) {
        lines.push_back(line);
    }
    assert(lines.size() == 10);

    std::cout << "Starting challenge 17" << std::endl;

    // First function picks a random line, creates a random key, pads the string, encrypts under cbc, then returns ciphertext and IV.
    Bytestring secretKey{};
    auto Encrypt = [&]() -> std::pair<Bytestring, Bytestring> {
        std::string line = lines[utils::UniformInt(0, 9)];
        if (secretKey.empty())
            secretKey = aes128::randomKey();
        // Decrypt line and pad.
        Bytestring b = base64::Decode(line);
        Bytestring IV = utils::RandomBytes(16);
        return { aes128::EncodeCBC(b, secretKey, IV, true), IV };
    };

    auto HasValidPadding = [&](Bytestring b, Bytestring IV) -> bool {
        Bytestring decoded = aes128::DecodeCBC(b, secretKey, IV, false);
        return PKCS::isValidPadding(decoded);
    };
    auto [encryptedTest, IVTest] = Encrypt();
    assert(HasValidPadding(encryptedTest, IVTest));

    // Idea is that we will bitflip until the suffix is all 0x10. At this point, we find the
    // original by taking the valid flipping xor block and xor by 0x10, which should be the original.
    auto GetOriginal = [&]() -> std::string {
        auto [bs, iv] = Encrypt();
        assert(bs.size() % 16 == 0);
        // Append the iv to the start of the text so that invariant of left modifies right is consistent.
        auto ivBs = iv + bs;
        auto ModifiedHasValidPadding = [&](Bytestring &b) -> bool {
            return HasValidPadding(b.substring(16, b.size() - 16), b.substring(0, 16));
        };
        Bytestring answer{};
        int blockCount = ivBs.size() / 16;
        for(int modifying = blockCount - 1; modifying >= 1; --modifying) {
            int toChange = modifying - 1;
            int toChangeOffset = toChange * 16;
            Bytestring subBs = ivBs.substring(0, (modifying + 1) * 16);
            bool isRightmostBlock = modifying == blockCount - 1;
            // If is the rightmost block, will have padding, so figure out how much it is and
            // try to guess the byte to the left. Otherwise, start at byte index 15.
            int startIndex = 15;
            // It should have valid padding. If we change the leftmost byte, will not be valid.
            if (isRightmostBlock) {
                Bytestring tmp(subBs);
                startIndex = 15;
                for(int i = 0; i < 16; ++i) {
                    tmp[toChangeOffset + i] ^= std::byte(0x42);
                    if (!ModifiedHasValidPadding(tmp)) {
                        // We broke it, so i'th byte is part of the padding.
                        startIndex = i - 1;
                        break;
                    }
                }
            }
            Bytestring mask(8, subBs.size());
            for(int guessing = startIndex; guessing >= 0; --guessing) {
                int lastPadding = (15 - guessing);
                int lastPaddingBytes = lastPadding;
                int nextPadding = lastPadding + 1;
                auto diff = std::byte(lastPadding ^ nextPadding);
                // Update last bytes to xor to new one.
                for(int i = 0; i < lastPaddingBytes; ++i) {
                    mask[toChangeOffset + 15 - i] ^= diff;
                }
                // Guess current one.
                int theOne = -1;
                for(int g = 0; g < 256; ++g) {
                    auto masked = subBs ^ mask;
                    masked[toChangeOffset + guessing] ^= std::byte(g);
                    if (ModifiedHasValidPadding(masked)) {
                        // Double check
                        if (guessing == 0) {
                            theOne = g;
                        }
                        else {
                            masked[toChangeOffset + guessing - 1] ^= std::byte(0x42);
                            if (ModifiedHasValidPadding(masked)) {
                                theOne = g;
                            }
                        }
                    }
                }
                mask[toChangeOffset + guessing] ^= std::byte(theOne);
            }
            // Once we guess all, the server side decoded is string(16, 0x10). If we xor with
            // 0x10, we will get the exact decoded text in our mask.
            mask = mask.substring(toChangeOffset, 16);
            mask = mask ^ Bytestring(8, std::vector<std::byte>(16, std::byte(0x10)));
            answer = mask + answer;
        }
        PKCS::Unpad(answer);
        return answer.toAsciiString();
    };

    std::cout << "Challenge 17 result is:" << std::endl;
    std::set<std::string> answers;
    while(answers.size() != 10) {
        std::cout << "Found " << answers.size() << " out of 10..." << std::endl;
        answers.insert(GetOriginal());
    }
    std::cout << "Found all answers! Reconstructed gives" << std::endl;
    for(const auto &e : answers)
        std::cout << e << std::endl;
}

void Challenge18() {
    std::string encrypted = "L77na/nrFsKvynd6HzOoG7GHTLXsTVu9qvY/2syLXzhPweyyMTJULu/6/kXX0KSvoOLSFQ==";
    auto cyphertext = base64::Decode(encrypted);
    auto key = Bytestring::FromString("YELLOW SUBMARINE");
    
    int counter = 0;
    for(int i = 0; i < cyphertext.size(); i += 16) {
        int sz = std::min(16, (int)cyphertext.size() - i);
        auto substring = cyphertext.substring(i, sz);
        auto counterBytes = Bytestring(8, 16);
        counterBytes[8] = std::byte(counter);
        auto keyStream = aes128::EncodeECB(counterBytes, key, false);
        if (sz != 16)
            keyStream = keyStream.substring(0, sz);
        std::cout << (substring ^ keyStream).toAsciiString();
        ++counter;
    }
    std::cout << std::endl;
}

int main() {
    Challenge17();
    Challenge18();
}
