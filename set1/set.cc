#include <iostream>
#include <string>
#include <assert.h>
#include <cstddef>
#include <set>
#include <fstream>
#include <limits.h>
#include <iterator>
#include <map>

#include "lib/bytestring.h"
#include "lib/base64.h"
#include "lib/frequency.h"
#include "lib/utils.h"
#include "lib/repeatingxor.h"
#include "lib/aes128.h"

void Challenge1() {
    std::string input = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
    std::string answer = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t";

    auto bs = BytestringFromHex(input);
    std::string s = base64::Encode(bs, false);

    assert(s == answer);

    std::cout << "Challenge one passed!" << std::endl;
}

void Challenge2() {
    std::string inputA = "1c0111001f010100061a024b53535009181c";
    std::string inputB = "686974207468652062756c6c277320657965";
    std::string answer = "746865206b696420646f6e277420706c6179";

    auto bsa = BytestringFromHex(inputA);
    auto bsb = BytestringFromHex(inputB);

    std::string s = (bsa ^ bsb).toHexString();

    assert(s == answer);

    std::cout << "Challenge two passed!" << std::endl;
}

void Challenge3() {
    std::string input = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";

    std::cout << "Challenge three result string:\n\t" << "\"" << std::get<0>(frequency::singleCharXORDecrypt(BytestringFromHex(input))) << "\"" << std::endl;
}

void Challenge4() {
    std::ifstream fin("C:/Users/Ian McKibben/Cryptopals/set1/4.txt");
    assert(fin.is_open());
    std::string line;

    std::string bests;
    double bestscore = DBL_MAX;
    while(std::getline(fin, line)) {
        auto [s, c, score] = frequency::singleCharXORDecrypt(BytestringFromHex(line));
        if (!s.empty()) {
            if (score < bestscore) {
                bestscore = score;
                bests = s;
            }
        }
    }
    std::cout << "Challenge four result string:\n\t" << "\"" << bests << "\"" << std::endl;
}

void Challenge5() {
    std::string line = "Burning 'em, if you ain't quick and nimble\nI go crazy when I hear a cymbal";

    Bytestring key = BytestringFromString("ICE");

    std::cout << "Challenge five result hex:" << std::endl;
    auto bs = BytestringFromString(line);
    auto encoded = repeatingxor::Transform(bs, key);
    std::cout << '\t' << encoded.toHexString() << std::endl;
}

void Challenge6() {
    // Testing hamming distance.
    int hammingTest = utils::HammingDistance(BytestringFromString("this is a test"), BytestringFromString("wokka wokka!!!"));
    assert(hammingTest == 37);
    std::cout << "[TEST] Hamming distance works!" << std::endl;
    // Testing base64 encode and decode.
    std::string tester = "this is a test!";
    assert(base64::Decode(base64::Encode(BytestringFromString(tester))).toAsciiString() == tester);
    std::cout << "[TEST] Base64 works!" << std::endl;

    std::ifstream fin("C:/Users/Ian McKibben/Cryptopals/set1/6.txt");
    assert(fin.is_open());

    std::string s{}, line;
    while(std::getline(fin, line))
        s += line;

    // First need to decode the b64.
    Bytestring decoded = base64::Decode(s);

    // Maintain a record of the k smallest keysizes.
    int k = 30;
    std::set<std::pair<double, int>> normalizedDistToKeysize;

    for(int keysize = 2; keysize <= 40; ++keysize) {
        // Get the first keysize of bytes.
        std::vector<std::byte> first, second;
        for(int i = 0; i < keysize; ++i) {
            first.push_back(decoded[i]);
        }
        for(int i = keysize; i < 2 * keysize; ++i) {
            second.push_back(decoded[i]);
        }

        int dist = utils::HammingDistance(Bytestring(1, first), Bytestring(1, second));
        double normalizedDist = double(dist) / double(keysize);

        // Add new one, and get rid of the largest normalized dist.
        normalizedDistToKeysize.insert({normalizedDist, keysize});
        while(normalizedDistToKeysize.size() > k)
            normalizedDistToKeysize.erase(std::prev(normalizedDistToKeysize.end()));
    }

    double bestdistance = DBL_MAX;
    std::string bests{};
    for(const auto &[keyscore, keysize] : normalizedDistToKeysize) {
        std::string candidateKey{};
        for(int i = 0; i < keysize; ++i) {
            // Group the similar blocks.
            std::vector<std::byte> keygroup;
            for(int q = i; q < decoded.size(); q += keysize) {
                keygroup.push_back(decoded[q]);
            }
            auto [s, c, score] = frequency::singleCharXORDecrypt(Bytestring(8, keygroup));
            candidateKey += c;
        }

        auto text = repeatingxor::Transform(decoded, BytestringFromString(candidateKey)).toAsciiString();
        frequency::FrequencyMap fm(text);
        if (fm.distance() < bestdistance) {
            bestdistance = fm.distance();
            bests = text;
        }
    }
    std::cout << "Challenge 6 result text:" << std::endl;
    std::cout << bests << std::endl;
}

void Challenge7() {
    std::vector<int> v = {0x54, 0x68, 0x61, 0x74, 0x73, 0x20, 0x6D, 0x79, 0x20, 0x4B, 0x75, 0x6E, 0x67, 0x20, 0x46, 0x75};
    std::vector<std::byte> byteVec;
    for(auto &e : v)
        byteVec.push_back(std::byte(e));
    Bytestring keyTest(8, byteVec);

    for(int round = 1; round <= aes128::rounds; ++round) {
        keyTest = aes128::nextRoundKey(keyTest, round);
    }
    // Checking that a test key works.
    assert(keyTest.toHexString() == "28fddef86da4244accc0a4fe3b316f26");

    std::cout << "[TEST] Rotkey works!" << std::endl;

    int a = 0b1101, b = 0b11;
    assert(utils::PolyMult(a, b) == 0b10111);
    std::cout << "[TEST] PolyMult works!" << std::endl;

    std::string testKey = "Thats my Kung Fu";
    std::string testText = "Two One Nine Two";

    auto testKeyBs = BytestringFromString(testKey);
    auto testTextBs = BytestringFromString(testText);

    auto testEncoded = aes128::Encode(testTextBs, testKeyBs);
    assert(testEncoded.toHexString() == "29c3505f571420f6402299b31a02d73a");
    std::cout << "[TEST] Encode works!" << std::endl;

    auto testDecoded = aes128::Decode(testEncoded, testKeyBs);
    assert(testDecoded.toAsciiString() == testText);
    std::cout << "[TEST] Decode works!" << std::endl;

    auto keyBs = BytestringFromString("YELLOW SUBMARINE");
    
    std::ifstream fin("C:/Users/Ian McKibben/Cryptopals/set1/7.txt");
    assert(fin.is_open());

    std::string s{}, line;
    while(std::getline(fin, line))
        s += line;

    Bytestring text = base64::Decode(s);
    Bytestring decoded = aes128::Decode(text, keyBs);
    std::cout << "Challenge 7 result text:" << std::endl;
    std::cout << decoded.toAsciiString() << std::endl;
}

void Challenge8() {
    std::ifstream fin("C:/Users/Ian McKibben/Cryptopals/set1/8.txt");
    assert(fin.is_open());

    std::string line;
    int lineCount = 0;
    std::map<int, std::pair<int, std::string>> maxCollisionsToString;
    while(std::getline(fin, line)) {
        ++lineCount;
        // What should I look at? Let's look at block collisions?
        std::unordered_map<std::string, int> hexCount;
        int maxCount = 0;
        for(int i = 0; i < line.size(); i += 32) {
            ++hexCount[line.substr(i, 32)];
            maxCount = std::max(maxCount, hexCount[line.substr(i, 32)]);
        }
        maxCollisionsToString[maxCount] = {lineCount, line};
    }
    auto &[cnt, p] = *maxCollisionsToString.rbegin();
    auto &[lc, s] = p;
    std::cout << "Challenge 8 result:" << std::endl;
    std::cout << "Line " << lc << " has " << cnt << " block collisions. Hex data is:\n\t" << s << std::endl;
}

int main() {
    Challenge1();
    Challenge2();
    Challenge3();
    Challenge4();
    Challenge5();
    Challenge6();
    Challenge7();
    Challenge8();
}
