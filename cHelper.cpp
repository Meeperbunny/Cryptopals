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

pair<float, string> freqDist(string s, string c, vector<float> idealFreq) {
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