#include <iostream>
#include <string>
#include <assert.h>
#include <unordered_map>
#include <functional>

#include "lib/bytestring.h"
#include "lib/utils.h"
#include "lib/base64.h"
#include "lib/aes128.h"

void Challenge9() {
    std::string s = "YELLOW SUBMARINE";
    auto b = Bytestring::FromString(s);
    b.pad(std::byte(0x04), 20);
    for(int i = 16; i < 20; ++i) {
        assert(b[i] == std::byte(0x04));
    }
    assert(b.size() == 20);

    std::cout << "Challenge 9 complete!" << std::endl;
}

void Challenge10() {
    auto key = Bytestring::FromString("YELLOW SUBMARINE");
    auto IV = Bytestring(8, std::vector<std::byte>(16, std::byte(0x00)));

    // Check that it works before opening file.
    std::string cbcTest = "ABCDEFGHIJKLMNOPABCDEFGHIJKLMNOP";
    assert(cbcTest == aes128::DecodeCBC(
            aes128::EncodeCBC(Bytestring::FromString(cbcTest), key, IV),
            key,
            IV
        ).toAsciiString()
    );

    std::string s = utils::StringFromFile("C:/Users/Ian McKibben/Cryptopals/set2/10.txt");
    auto encoded = base64::Decode(s);

    auto bs = aes128::DecodeCBC(encoded, key, IV);
    std::string decoded = bs.toAsciiString();

    std::cout << "Challenge 10 result:" << std::endl;
    std::cout << decoded << std::endl;
}

void Challenge11() {
    // Create some function for checking if given a Bytestring it is ECB or CBC.
    auto oracleFunction = [](std::function<Bytestring(const Bytestring&)> func) -> std::string {
        // Feed the function repeating text and see if there are repeats.
        auto malicious = Bytestring::FromString(std::string(16 * 32, 'A'));
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
    std::cout << "Challenge 11 has a ECB rate of: " << ecbCount / samples << std::endl;
}

void Challenge12() {
    Bytestring secretKey = aes128::randomKey();
    std::string secretMessage = base64::Decode(utils::StringFromFile("C:/Users/Ian McKibben/Cryptopals/set2/12.txt")).toAsciiString();

    auto paddedEncode = [&](const std::string &text) -> Bytestring {
        auto paddedText = Bytestring::FromString(text + secretMessage);
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
    std::cout << "Challenge 12 hidden text found is streaming: " << std::endl;
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

namespace profile {
    std::string profileFor(const std::string &email) {
        if (email.find('&') != std::string::npos ||
            email.find('=') != std::string::npos) {
                return ""; // Can't container either of these symbols.
        }
        return "email=" + email + "&uid=10&role=user";
    }

    std::unordered_map<std::string, std::string> mapFromString(std::string s) {
        char delim = '&';
        std::vector<std::string> fields;
        std::stringstream ss(s);
        std::unordered_map<std::string, std::string> m;
        for (std::string token; std::getline(ss, token, delim);) {
            fields.push_back(token);
        }
        for(auto &field : fields) {
            if (count(field.begin(), field.end(), '=') != 1)
                return {};
            int ind = field.find('=');
            if (ind + 1 == field.size())
                return {};
            m.insert({field.substr(0, ind), field.substr(ind + 1)});
        }
        return m;
    }

    Bytestring key;
    Bytestring EncodedFromEmail(const std::string &email) {
        if (profile::key.empty()) profile::key = aes128::randomKey();
        auto s = profile::profileFor(email);
        return aes128::EncodeECB(Bytestring::FromString(s), profile::key);
    }

    std::unordered_map<std::string, std::string> DecodeProfile(const Bytestring &profile) {
        assert(!profile::key.empty());
        std::string s = aes128::DecodeECB(profile, profile::key).toAsciiString();
        return profile::mapFromString(s);
    }

    bool UserIsAdmin(const std::unordered_map<std::string, std::string> &user) {
        auto it = user.find("role");
        return (it != user.end() && it->second == "admin");
    }
}

void Challenge13() {
    // Example user info string: "email=foo@bar.com&uid=10&role=user"
    // There are two steps to this. The first is we want to see what
    // "email=foo@bar.com&uid=10&role=" encodes into, and we want the
    // end of the string to align at the end of a block. That way we
    // can add something after the "role=" part. The second part is
    // finding what "admin" looks like. If the string is too big, it
    // gets padded with 0's, so we will insert an "admin" at the start
    // of some block in the middle, and have it padded with 0's. Once
    // these are done, we combine the first and put the second on it.

    std::string withoutEmail = "email=&uid=10&role=";
    int emailSize = 16 - (withoutEmail.size() % 16);
    std::string mockEmail = std::string(emailSize, 'A');
    int relevantBlockCount = mockEmail.size() + withoutEmail.size();

    // Check what the first part encodes into.
    auto firstBytestring = profile::EncodedFromEmail(mockEmail);
    Bytestring prefixBlock = firstBytestring.substring(0, relevantBlockCount);

    // Now we get the second part.
    std::string prefix = "email=";
    std::string maliciousBlock = "admin";
    int prefixPadding = 16 - prefix.size();
    int blockPadding = 16 - maliciousBlock.size();
    std::string adminTest = std::string(prefixPadding, 0x04) + maliciousBlock + std::string(blockPadding, 0x04);

    auto secondBytestring = profile::EncodedFromEmail(adminTest);
    // We only want the second block.
    Bytestring adminBlock = secondBytestring.substring(16, 16);

    // Combine them and decode the user.
    auto userBytestring = prefixBlock + adminBlock;
    auto user = profile::DecodeProfile(userBytestring);
    std::cout << "Printing user information:" << std::endl;
    std::cout << "{" << std::endl;
    for(auto e : user)
        std::cout << "\t" << e.first << ": " << e.second << "," << std::endl;
    std::cout << "}" << std::endl;

    std::cout << "Checking if a user is an admin: " << (profile::UserIsAdmin(user) ? "TRUE" : "FALSE") << std::endl;
    assert(profile::UserIsAdmin(user));

    std::cout << "Challenge 13 completed!" << std::endl;
}

void Challenge14() {
    auto secretKey = aes128::randomKey();
    std::string secretMessage = base64::Decode(utils::StringFromFile("C:/Users/Ian McKibben/Cryptopals/set2/12.txt")).toAsciiString();

    auto prefixEncode = [&](const std::string &text) -> Bytestring {
        auto randomPrefix = utils::RandomBytes(utils::UniformInt(6, 24));
        return aes128::EncodeECB(randomPrefix + Bytestring::FromString(text + secretMessage), secretKey);
    };

    // Idea here is we can just run it a bunch of times and if it doesn't work after a lot of times, then the guess
    // was not correct.
    std::cout << "Challenge 14 hidden text found is streaming (may take a long time due to the confidence): " << std::endl;
    auto oracleFunction = [](std::function<Bytestring(const std::string&)> func, const std::string &text) -> std::string {
        std::unordered_map<std::string, int> frequencyMap;
        auto blocks = aes128::bytestringToBlockVector(func(text));
        for(auto &block : blocks) {
            std::string hexString{};
            for(auto &row : block) hexString += row.toHexString();
            ++frequencyMap[hexString];
            if (frequencyMap[hexString] > 1) {
                return "ECB";
            }
        }
        return "CBC";
    };
    const int blockSizeBytes = 16;
    std::string found = "";
    while (true) {
        bool hasFoundSomething = false;
        char paddingChar = 0x01;
        for(int guess = 0x02; guess < 256; ++guess) { // Do this so that the guess doesn't overlap with the padding.
            auto toReplicate = std::string(blockSizeBytes - 1, paddingChar);
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
            auto padding = std::string(paddingSize, paddingChar);
            // Prepend a few different characters to avoid randomness.
            std::string toInsert = paddingChar + toReplicate + padding;
            // We will run this multiple times until the confidence interval is hit.
            const double confidence = 0.998;
            // Assume chance of getting the right offset is above 1/32.
            for(double chanceOfTrueMiss = 1.0; chanceOfTrueMiss > 1 - confidence; chanceOfTrueMiss *= 31.0 / 32.0) {
                if (oracleFunction(prefixEncode, toInsert) == "ECB") {
                    found += char(guess);
                    hasFoundSomething = true;
                    std::cout << char(guess);
                    break;
                }
            }
            if (hasFoundSomething) break;
        }
        if (!hasFoundSomething)
            break;
    }
    std::cout << std::endl;
}

void Challenge15() {
    std::string test = "ICE ICE BABY";
    auto bs = Bytestring::FromString(test);

    auto t1 = bs + Bytestring::FromHex("04040404");
    auto t2 = bs + Bytestring::FromHex("05050505");
    auto t3 = bs + Bytestring::FromHex("01020304");

    assert(aes128::removePadding(t1).size() == 12);
    assert(aes128::removePadding(t2).size() == 16);
    assert(aes128::removePadding(t3).size() == 16);

    std::cout << "Challenge 15 completed!" << std::endl;
}

namespace credentials {
    Bytestring secretKey = aes128::randomKey();
    Bytestring secretIV = utils::RandomBytes(16); // For clarity.
    Bytestring EncryptString(const std::string &s) {
        std::string filtered = "";
        for(const auto &c : s) {
            if (c != ';' && c != '=') filtered += c;
        }
        const std::string prefix = "comment1=cooking%20MCs;userdata=";
        const std::string suffix = ";comment2=%20like%20a%20pound%20of%20bacon";
        auto bs = Bytestring::FromString(prefix + filtered + suffix);
        bs.pad(aes128::padding, 16);
        return aes128::EncodeCBC(bs, credentials::secretKey, credentials::secretIV);
    }
    bool DecodeAndCheckIfAdmin(const Bytestring &encoded) {
        auto decoded = aes128::DecodeCBC(encoded, credentials::secretKey, credentials::secretIV);
        auto s = decoded.toAsciiString();
        return s.find(";admin=true;") != std::string::npos;
    }
}

void Challenge16() {
    // There are 2 blocks, then we will add 2. We will have a
    // string that is the "target", and one that we change.
    char fillerChar = '0';
    std::string targetBytestring = ";admin=true";
    std::string initialString = std::string(0x10, fillerChar) + std::string(targetBytestring.size(), fillerChar);
    Bytestring encrypted = credentials::EncryptString(initialString);

    // Flip to the diff.
    for(int i = 0; i < targetBytestring.size(); ++i) {
        encrypted[0x20 + i] ^= std::byte(fillerChar) ^ std::byte(targetBytestring[i]);
    }

    bool isAdmin = credentials::DecodeAndCheckIfAdmin(encrypted);
    std::cout << "Checking if is admin: " << (isAdmin ? "TRUE!" : "FALSE") << std::endl;
    assert(isAdmin);

    std::cout << "Challenge 16 completed!" << std::endl;
}

int main() {
    const bool RUN_SLOW_CHALLENGES = false;
    Challenge9();
    Challenge10();
    Challenge11();
    Challenge12();
    Challenge13();
    if (RUN_SLOW_CHALLENGES) {
        Challenge14();
    }
    else {
        std::cout << "Skipping challenge 14 due to it taking too long" << std::endl;
    }
    Challenge15();
    Challenge16();
}
