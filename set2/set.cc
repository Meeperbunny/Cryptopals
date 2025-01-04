#include <iostream>
#include <string>
#include <assert.h>
#include <unordered_map>
#include <functional>

#include "lib/bytestring.h"
#include "lib/utils.h"
#include "lib/base64.h"
#include "lib/aes128.h"

void Challenge1() {
    std::string s = "YELLOW SUBMARINE";
    auto b = BytestringFromString(s);
    b.pad(std::byte(0x04), 20);
    for(int i = 16; i < 20; ++i) {
        assert(b[i] == std::byte(0x04));
    }
    assert(b.size() == 20);

    std::cout << "Challenge 1 complete!" << std::endl;
}

void Challenge2() {
    auto key = BytestringFromString("YELLOW SUBMARINE");
    auto IV = Bytestring(8, std::vector<std::byte>(16, std::byte(0x00)));

    // Check that it works before opening file.
    std::string cbcTest = "ABCDEFGHIJKLMNOPABCDEFGHIJKLMNOP";
    assert(cbcTest == aes128::DecodeCBC(
            aes128::EncodeCBC(BytestringFromString(cbcTest), key, IV),
            key,
            IV
        ).toAsciiString()
    );

    std::string s = utils::StringFromFile("C:/Users/Ian McKibben/Cryptopals/set2/10.txt");
    auto encoded = base64::Decode(s);

    auto bs = aes128::DecodeCBC(encoded, key, IV);
    std::string decoded = bs.toAsciiString();

    std::cout << "Challenge 2 result:" << std::endl;
    std::cout << decoded << std::endl;
}

void Challenge3() {
    // Create some function for checking if given a Bytestring it is ECB or CBC.
    auto oracleFunction = [](std::function<Bytestring(const Bytestring&)> func) -> std::string {
        // Feed the function repeating text and see if there are repeats.
        auto malicious = BytestringFromString(std::string(16 * 32, 'A'));
        std::unordered_map<std::string, int> frequencyMap;
        auto blocks = aes128::bytestringToBlockVector(func(malicious));
        for(auto &block : blocks) {
            std::string hexString{};
            for(auto &row : block) hexString += row.toHexString();
            ++frequencyMap[hexString];
            if (frequencyMap[hexString] > 1) return "ECB";
        }
        return "CBC";
    };
    
    const int samples = 100;
    double ecbCount = 0;
    for(int i = 0; i < samples; ++i) {
        ecbCount += oracleFunction(aes128::randomModeKeyEncryption) == "ECB";
    }
    std::cout << "Challenge 3 has a ECB rate of: " << ecbCount / samples << std::endl;
}

void Challenge4() {
    Bytestring secretKey = aes128::randomKey();
    std::string secretMessage = base64::Decode(utils::StringFromFile("C:/Users/Ian McKibben/Cryptopals/set2/12.txt")).toAsciiString();

    auto paddedEncode = [&](const std::string &text) -> Bytestring {
        auto paddedText = BytestringFromString(text + secretMessage);
        return aes128::EncodeECB(paddedText, secretKey);
    };
    
    auto oracleFunction = [](std::function<Bytestring(const std::string&)> func, const std::string &text) -> std::string {
        std::unordered_map<std::string, int> frequencyMap;
        auto blocks = aes128::bytestringToBlockVector(func(text));
        for(auto &block : blocks) {
            std::string hexString{};
            for(auto &row : block) hexString += row.toHexString();
            ++frequencyMap[hexString];
            if (frequencyMap[hexString] > 1) return "ECB";
        }
        return "CBC";
    };

    int blockSizeBytes{};
    // First we will find the block size.
    for(int i = 1; i < 128; ++i) {
        // Find it for two repeating strings, as the first char of the unknown could be our char.
        auto A = std::string(i * 2, 'A');
        auto B = std::string(i * 2, 'B');
        if (oracleFunction(paddedEncode, A) == "ECB" && oracleFunction(paddedEncode, B) == "ECB") {
            blockSizeBytes = i;
            break;
        }
    }
    std::cout << "Block size of: " << blockSizeBytes << " bytes found!" << std::endl;

    // Idea is to try and guess the next char of the prefix of what we have found.
    // If we can't find any char that works, then it is the end. Need to be careful
    // about the positioning of the unknown text.
    // Will have the order be [Block to replicate][Block for padding][Unkown Text].
    std::cout << "Challenge 4 hidden text found is streaming: " << std::endl;
    std::string found = "";
    while (true) {
        bool hasFoundSomething = false;
        for(int guess = 0; guess < 256; ++guess) {
            auto toReplicate = std::string(blockSizeBytes - 1, 'A');
            // Take all of what we have found and put it at the end of this.
            toReplicate += found;
            // Take the last blockSizeBytes - 1 characters. This is our replication block.
            toReplicate = toReplicate.substr(toReplicate.size() - (blockSizeBytes - 1)) ;
            // Now we will add a guess at the end of this.
            toReplicate += char(guess);

            // Now we want the part we are guessing to line up with the block we have.
            // More specifically, we want the size(optional padding) + size(found) + 1
            // to be a multiple of the blockSizeBytes, so we solve for the padding size.
            int paddingSize = blockSizeBytes - (found.size() % blockSizeBytes + 1);
            auto padding = std::string(paddingSize, 'A');

            std::string toInsert = toReplicate + padding;
            if (oracleFunction(paddedEncode, toInsert) == "ECB") {
                found += char(guess);
                hasFoundSomething = true;
                std::cout << char(guess);
                break;
            }
        }
        if (!hasFoundSomething)
            break;
    }
    std::cout << std::endl;
}

int main() {
    Challenge1();
    Challenge2();
    Challenge3();
    Challenge4();
}
