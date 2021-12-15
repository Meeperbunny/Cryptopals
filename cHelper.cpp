#include "cHelper.h"

string hexToBase64(string hexString) {
    // Convert hex to boolean array
    vector<bool> arr;
    string ret = "";
    for(int i = 0; i < hexString.size(); i++) {
        // Hex to num
        int num = hexString[i] - ((hexString[i] <= '9') ? '0' : 'a' - 10);

        // Num to 4-bits
        for(int q = 3; q >= 0; q--) {
            if (num >> q & 1) arr.push_back(1);
            else arr.push_back(0);
        }
    }
        
    // Add padding
    int paddingSize = (6 - hexString.size() % 6) % 6;
    for(int i = 0; i < paddingSize * 4; i++) {
        arr.push_back(0);
    }

    // Change back to bytes, not 4-bits
    paddingSize = paddingSize / 2;

    // Convert each 6-byte block into table
    for(int i = 0; i < arr.size(); i += 6) {
        // Get block value
        int num = 0;
        for(int q = 0; q < 6; q++)
            num += int(pow(2, 5 - q)) * int(arr[i + q]);

        // Convert to table value
        char appendChar;
        if (0 <= num && num <= 25) appendChar = char('A' + num);
        else if (26 <= num && num <= 51) appendChar = char('a' + num - 26);
        else if (52 <= num && num <= 61) appendChar = char('0' + num - 52);
        else if (num == 62) appendChar = '+';
        else if (num == 63) appendChar = '/';

        ret += appendChar;
    }

    // Pad end of string
    for(int i = 0; i < paddingSize; i++)
        ret[ret.size() - 1 - i] = '=';

    return ret;
}

string base64DecryptToHex(string base64String) {
    vector<bool> boolArr;

    // Iterate through each num, and convert to a bool array
    int padCnt = 0;
    for(int i = 0; i < base64String.size(); i++) {
        char curr = base64String[i];
        if (curr == '=') padCnt++;
        int num = -1;
        if ('A' <= curr && curr <= 'Z') num = int(curr - 'A');
        else if ('a' <= curr && curr <= 'z') num = int(curr - 'a' + 26);
        else if ('0' <= curr && curr <= '9') num = int(curr - '0' + 52);
        else if (curr == '+') num = 62;
        else if (curr == '/') num = 63;

        for(int i = 5; i >= 0; i--) {
            if (num >> i & 1 && curr != '=') boolArr.push_back(1);
            else {
                boolArr.push_back(0);
            }
        }
    }

    // Remove padding
    for(int i = 0; i < padCnt * 8; i++) boolArr.pop_back();

    // Convert bool to hex string
    return boolArrToHexString(boolArr);
}

vector<bool> hexStringToBoolArr(string s) {
    vector<bool> ret;
    for(unsigned int i = 0; i < s.size(); i++) {
        // Hex to num
        int num = tolower(s[i]) - ((s[i] <= '9') ? '0' : 'a' - 10);

        // Num to 4-bits
        for(int q = 3; q >= 0; q--) {
            if (num >> q & 1) ret.push_back(1);
            else ret.push_back(0);
        }
    }
    return ret;
}

string boolArrToHexString(vector<bool> arr) {
    string ret;
    for(unsigned int i = 0; i < arr.size(); i += 4) {
        // Hex to num
        int num = 0;
        for(int q = 0; q < 4; q++) 
            num += arr[i + q] * pow(2, 3 - q);

        // Add hex char to ret string
        ret += (num <= 9) ? '0' + num : 'a' + num - 10;
    }
    return ret;
}

string hexXOR(string firstString, string secondString) {
    vector<bool> first(hexStringToBoolArr(firstString));
    vector<bool> second(hexStringToBoolArr(secondString));

    for(int i = 0; i < first.size(); i++)
        first[i] = (first[i] ^ second[i]);

    return boolArrToHexString(first);
}

string hexToAscii(string hexString) {
    string ret = "";
    for(int i = 0; i < hexString.size(); i += 2) {
        string hexChar = hexString.substr(i, 2);
        int num = 0;
        num += 16 * (hexChar[0] - ((hexChar[0] <= '9') ? '0' : 'a' - 10));
        num += hexChar[1] - ((hexChar[1] <= '9') ? '0' : 'a' - 10);
        ret += char(num);
    }
    return ret;
}

string charToHex(char c) {
    string ret = "";
    for(int i = 0; i < 2; i++) {
        int sum = 0;
        for(int q = 0; q < 4; q++)
            sum += (c >> (q + 4 * (1 - i)) & 1) * pow(2, q);
        if (0 <= sum && sum <= 9) ret += char(sum + '0');
        else if (10 <= sum && sum <= 15) ret += char(sum + 'a' - 10);
    }
    return ret;
}

float freqCalc(string text, vector<float> idealFreq) {
    // Calc Freq
    vector<float> freq(26, 0.0);
    float dist = 0.0;
    float textSize = 0.0;
    for(int i = 0; i < text.size(); i++) {
        int curr = tolower(text[i]);
        if ('a' <= curr && curr <= 'z')
            freq[curr - 'a'] += 1.0;
        if (curr != ' ')
            dist += 1.0;
        textSize += 1.0;
    }
    

    // Calculate distance from idealFreq
    for(int i = 0; i < freq.size(); i++) {
        dist += abs(freq[i] / textSize - idealFreq[i]);
    }

    return dist;
}

string repeatStringXORString(string s, string key) {
    string first = s;
    string second = "";
    // Fill second string with hex version of c
    vector<string> hexArray;
    for(int i = 0; i < key.size(); i++) {
        hexArray.push_back(charToHex(key[i]));
    }
    for(int i = 0; second.size() < first.size(); i = (i + 1) % hexArray.size())
        second += hexArray[i];

    string XORed = hexXOR(first, second);
    string text = hexToAscii(XORed);

    return text;
}

string stringToHexString(string s) {
    string ret = "";
    for(int i = 0; i < s.size(); i++) {
        ret += charToHex(s[i]);
    }
    return ret;
}

pair<float, string> freqDist(string s, string c) {
    vector<float> idealFreq = { 0.08496, 0.020720, 0.04538, 0.03384, 0.11160, 0.018121, 0.024705, 0.030034, 0.07544, 0.001965, 0.011016, 0.05489, 0.030129, 0.06654, 0.07163, 0.03167, 0.001962, 0.07580, 0.05735, 0.06950, 0.03630, 0.010074, 0.012899, 0.002902, 0.017779, 0.002722 };
    string first = s;
    string second = "";

    string text = repeatStringXORString(s, c);

    float dist = freqCalc(text, idealFreq);

    return make_pair(dist, text);
}

bool isValidString(string s, float threshold) {
    bool isValid = true;
    float count = 0.0;
    for(int i = 0; i < s.size(); i++) {
        if ('a' <= tolower(s[i]) && tolower(s[i]) <= 'z') {
            count += 1.0;
        }
        if (s[i] < 0) {
            isValid = false;
            break;
        }
    }
    if (count / float(s.size()) < threshold) {
        return false;
    }
    return isValid;
}

int hammingDistance(string a, string b) {
    string hexA = stringToHexString(a);
    string hexB = stringToHexString(b);
    vector<bool> boolA = hexStringToBoolArr(hexA);
    vector<bool> boolB = hexStringToBoolArr(hexB);
    int dist = 0;
    for(int i = 0; i < boolA.size(); i++) {
        if (boolA[i] != boolB[i]) dist += 1;
    }
    return dist;
}

int hammingDistance(vector<bool> a, vector<bool> b) {
    int dist = 0;
    for(int i = 0; i < a.size(); i++) {
        if (a[i] != b[i]) dist += 1;
    }
    return dist;
}

uint8_t hexToByte(string hex) {
    int ret = 0;
    for(int i = 0; i < hex.size(); i += 2) {
        string hexChar = hex.substr(i, 2);
        uint8_t num = 0;
        num += 16 * (hexChar[0] - ((hexChar[0] <= '9') ? '0' : 'a' - 10));
        num += hexChar[1] - ((hexChar[1] <= '9') ? '0' : 'a' - 10);
        ret += num;
    }
    return ret;
}

vector<uint8_t> hexToByteArr(string hex) {
    vector<uint8_t> nums;
    for(int i = 0; i < hex.size() / 2; i++)
        nums.push_back(hexToByte(hex.substr(2 * i, 2)));
    return nums;
}

vector<uint8_t> initialize_aes_sbox(char mode) {
    vector<uint8_t> sbox(256, 0);
	uint8_t p = 1, q = 1;
	
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

		sbox[p] = xformed ^ 0x63;
	} while (p != 1);

	/* 0 is a special case since it has no inverse */
	sbox[0] = 0x63;

    if (mode == 'E')
        return sbox;

    // Find inverse for decryption
    vector<uint8_t> iSbox(256, 0);
    for(int i = 0; i < sbox.size(); i++) {
        iSbox[sbox[i]] = i;
    }

    return iSbox;
}

// AES FUNCTIONS

void AES::shiftRow(vector<uint8_t> &block, char mode) {
    vector<uint8_t> cp(block);
    int offset = (mode == 'E') ? 5 : 13;
    for(int i = 0; i < block.size(); i++) {
        block[i] = cp[(offset * i) % block.size()];
    }
    return;
}

void AES::byteSub(vector<uint8_t> &block, char mode) {
    // Get sBox
    vector<uint8_t> sBox = initialize_aes_sbox(mode);

    // Sub bytes
    for(int i = 0; i < block.size(); i++) {
        block[i] = sBox[block[i]];
    }
    return;
}

void AES::addRound(vector<uint8_t> &block) {
    
}

void AES::mixCol(vector<uint8_t> &block, char mode) {
    vector<uint8_t> mMat = {
        0x02, 0x03, 0x01, 0x01,
        0x01, 0x02, 0x03, 0x01,
        0x01, 0x01, 0x02, 0x03,
        0x03, 0x01, 0x01, 0x02
    };
    if (mode != 'E') {
        mMat = {
            0x0E, 0x0B, 0x0D, 0x09,
            0x09, 0x0E, 0x0B, 0x0D,
            0x0D, 0x09, 0x0E, 0x0B,
            0x0B, 0x0D, 0x09, 0x0E
        };
    }

    for(int i = 0; i < 4; i++) {
        vector<uint8_t> temp(block.begin() + 4 * i, block.begin() + 4 * i + 4);

        for(int q = 0; q < 4; q++) {
            uint8_t val = 0;
            for(int n = 0; n < 4; n++) {
                int eVal = int(AES::Helpers::L[temp[n]]) + int(AES::Helpers::L[mMat[q * 4 + n]]);
                if (eVal > 0xFF) eVal -= 0xFF;
                val = val ^ (AES::Helpers::E[uint8_t(eVal)]);
            }
            block[4 * i + q] = val;
        }
    }
    return;
}

vector<uint8_t> AES::AES128Decrypt(vector<uint8_t> block, string key) {
    char mode = 'D';
    const int rounds = 10;

    AES::addRound(block);
    
    for(int i = 0; i < rounds; i++) {
        shiftRow(block, mode);
        byteSub(block, mode);
        addRound(block);
        if (i != rounds - 1) // Mix col every round except last
            mixCol(block, mode);
    }

    return block;
}

uint8_t AES::Helpers::E[256] = {
    0x01, 0x03, 0x05, 0x0F, 0x11, 0x33, 0x55, 0xFF, 0x1A, 0x2E, 0x72, 0x96, 0xA1, 0xF8, 0x13, 0x35,
    0x5F, 0xE1, 0x38, 0x48, 0xD8, 0x73, 0x95, 0xA4, 0xF7, 0x02, 0x06, 0x0A, 0x1E, 0x22, 0x66, 0xAA,
    0xE5, 0x34, 0x5C, 0xE4, 0x37, 0x59, 0xEB, 0x26, 0x6A, 0xBE, 0xD9, 0x70, 0x90, 0xAB, 0xE6, 0x31,
    0x53, 0xF5, 0x04, 0x0C, 0x14, 0x3C, 0x44, 0xCC, 0x4F, 0xD1, 0x68, 0xB8, 0xD3, 0x6E, 0xB2, 0xCD,
    0x4C, 0xD4, 0x67, 0xA9, 0xE0, 0x3B, 0x4D, 0xD7, 0x62, 0xA6, 0xF1, 0x08, 0x18, 0x28, 0x78, 0x88,
    0x83, 0x9E, 0xB9, 0xD0, 0x6B, 0xBD, 0xDC, 0x7F, 0x81, 0x98, 0xB3, 0xCE, 0x49, 0xDB, 0x76, 0x9A,
    0xB5, 0xC4, 0x57, 0xF9, 0x10, 0x30, 0x50, 0xF0, 0x0B, 0x1D, 0x27, 0x69, 0xBB, 0xD6, 0x61, 0xA3,
    0xFE, 0x19, 0x2B, 0x7D, 0x87, 0x92, 0xAD, 0xEC, 0x2F, 0x71, 0x93, 0xAE, 0xE9, 0x20, 0x60, 0xA0,
    0xFB, 0x16, 0x3A, 0x4E, 0xD2, 0x6D, 0xB7, 0xC2, 0x5D, 0xE7, 0x32, 0x56, 0xFA, 0x15, 0x3F, 0x41,
    0xC3, 0x5E, 0xE2, 0x3D, 0x47, 0xC9, 0x40, 0xC0, 0x5B, 0xED, 0x2C, 0x74, 0x9C, 0xBF, 0xDA, 0x75,
    0x9F, 0xBA, 0xD5, 0x64, 0xAC, 0xEF, 0x2A, 0x7E, 0x82, 0x9D, 0xBC, 0xDF, 0x7A, 0x8E, 0x89, 0x80,
    0x9B, 0xB6, 0xC1, 0x58, 0xE8, 0x23, 0x65, 0xAF, 0xEA, 0x25, 0x6F, 0xB1, 0xC8, 0x43, 0xC5, 0x54,
    0xFC, 0x1F, 0x21, 0x63, 0xA5, 0xF4, 0x07, 0x09, 0x1B, 0x2D, 0x77, 0x99, 0xB0, 0xCB, 0x46, 0xCA,
    0x45, 0xCF, 0x4A, 0xDE, 0x79, 0x8B, 0x86, 0x91, 0xA8, 0xE3, 0x3E, 0x42, 0xC6, 0x51, 0xF3, 0x0E,
    0x12, 0x36, 0x5A, 0xEE, 0x29, 0x7B, 0x8D, 0x8C, 0x8F, 0x8A, 0x85, 0x94, 0xA7, 0xF2, 0x0D, 0x17,
    0x39, 0x4B, 0xDD, 0x7C, 0x84, 0x97, 0xA2, 0xFD, 0x1C, 0x24, 0x6C, 0xB4, 0xC7, 0x52, 0xF6, 0x01
};

uint8_t AES::Helpers::L[256] = {
    0x00, 0x00, 0X19, 0X01, 0X32, 0X02, 0X1A, 0XC6, 0X4B, 0XC7, 0X1B, 0X68, 0X33, 0XEE, 0XDF, 0X03,
    0x64, 0X04, 0XE0, 0X0E, 0X34, 0X8D, 0X81, 0XEF, 0X4C, 0X71, 0X08, 0XC8, 0XF8, 0X69, 0X1C, 0xC1,
    0x7D, 0XC2, 0X1D, 0XB5, 0XF9, 0XB9, 0X27, 0X6A, 0X4D, 0XE4, 0XA6, 0X72, 0X9A, 0XC9, 0X09, 0x78,
    0x65, 0X2F, 0X8A, 0X05, 0X21, 0X0F, 0XE1, 0X24, 0X12, 0XF0, 0X82, 0X45, 0X35, 0X93, 0XDA, 0x8E,
    0x96, 0X8F, 0XDB, 0XBD, 0X36, 0XD0, 0XCE, 0X94, 0X13, 0X5C, 0XD2, 0XF1, 0X40, 0X46, 0X83, 0x38,
    0x66, 0XDD, 0XFD, 0X30, 0XBF, 0X06, 0X8B, 0X62, 0XB3, 0X25, 0XE2, 0X98, 0X22, 0X88, 0X91, 0x10,
    0x7E, 0X6E, 0X48, 0XC3, 0XA3, 0XB6, 0X1E, 0X42, 0X3A, 0X6B, 0X28, 0X54, 0XFA, 0X85, 0X3D, 0xBA,
    0x2B, 0X79, 0X0A, 0X15, 0X9B, 0X9F, 0X5E, 0XCA, 0X4E, 0XD4, 0XAC, 0XE5, 0XF3, 0X73, 0XA7, 0x57,
    0xAF, 0X58, 0XA8, 0X50, 0XF4, 0XEA, 0XD6, 0X74, 0X4F, 0XAE, 0XE9, 0XD5, 0XE7, 0XE6, 0XAD, 0xE8,
    0x2C, 0XD7, 0X75, 0X7A, 0XEB, 0X16, 0X0B, 0XF5, 0X59, 0XCB, 0X5F, 0XB0, 0X9C, 0XA9, 0X51, 0xA0,
    0x7F, 0X0C, 0XF6, 0X6F, 0X17, 0XC4, 0X49, 0XEC, 0XD8, 0X43, 0X1F, 0X2D, 0XA4, 0X76, 0X7B, 0xB7,
    0xCC, 0XBB, 0X3E, 0X5A, 0XFB, 0X60, 0XB1, 0X86, 0X3B, 0X52, 0XA1, 0X6C, 0XAA, 0X55, 0X29, 0x9D,
    0x97, 0XB2, 0X87, 0X90, 0X61, 0XBE, 0XDC, 0XFC, 0XBC, 0X95, 0XCF, 0XCD, 0X37, 0X3F, 0X5B, 0xD1,
    0x53, 0X39, 0X84, 0X3C, 0X41, 0XA2, 0X6D, 0X47, 0X14, 0X2A, 0X9E, 0X5D, 0X56, 0XF2, 0XD3, 0xAB,
    0x44, 0X11, 0X92, 0XD9, 0X23, 0X20, 0X2E, 0X89, 0XB4, 0X7C, 0XB8, 0X26, 0X77, 0X99, 0XE3, 0xA5,
    0x67, 0X4A, 0XED, 0XDE, 0XC5, 0X31, 0XFE, 0X18, 0X0D, 0X63, 0X8C, 0X80, 0XC0, 0XF7, 0X70, 0x07
};