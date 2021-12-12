#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <float.h>
#include <cmath>
#include <fstream>

using namespace std;

vector<bool> hexStringToBoolArr(string s) {
    vector<bool> ret;
    for(unsigned int i = 0; i < s.size(); i++) {
        // Hex to num
        int num;
        char lowerChar = tolower(s[i]);
        if ('0' <= lowerChar && lowerChar <= '9') {
            num = int(s[i] - '0');
        }
        else if ('a' <= lowerChar && lowerChar <= 'f') {
            num = int(lowerChar - 'a') + 10;
        } 
        else continue;

        // Num to 4-bits
        for(int q = 3; q >= 0; q--) {
            int sub = int(pow(2, q));
            if (num >= sub) {
                ret.push_back(1);
                num -= sub;
            }
            else {
                ret.push_back(0);
            }
        }
    }
    return ret;
}

string hexXOR(string firstString, string secondString) {
    // Convert hex to boolean array
    vector<bool> first(hexStringToBoolArr(firstString)), second(hexStringToBoolArr(secondString));
    string ret = "";

    for(int i = 0; i < first.size(); i += 4) {
        int num = 0;
        for(int q = 0; q < 4; q++) {
            num += int(pow(2, 3 - q)) * (first[i + q] ^ second[i + q]);
        }

        char appendChar;
        if (0 <= num && num <= 9) {
            appendChar = char('0' + num);
        }
        else if (10 <= num && num <= 15) {
            appendChar = char(num + 'a' - 10);
        }

        ret += appendChar;
    }

    return ret;
}

pair<float, string> freqDist(string s, char c, vector<float> idealFreq) {
    string first = s;
    string second = "";

    // Get hex version of c
    string cInHex = "";
    for(int i = 0; i < 2; i++) {
        int sum = 0;
        for(int q = 0; q < 4; q++) {
            if (c >= pow(2, 4 * (1 - i) + 3 - q)) {
                c -= pow(2, 4 * (1 - i) + 3 - q);
                sum += int(pow(2, 3 - q));
            }
        }
        if (0 <= sum && sum <= 9) cInHex += char(sum + '0');
        else if (10 <= sum && sum <= 15) cInHex += char(sum + 'a' - 10);
    }

    for(int i = 0; i < first.size() / 2; i++) second += cInHex;

    string XORed = hexXOR(first, second);
    string text = "";

    for(int i = 0; i < XORed.size(); i += 2) {
        string hexChar = XORed.substr(i, 2);
        int num = 0;
        num += 16 * (hexChar[0] - ((hexChar[0] <= '9') ? '0' : 'a' - 10));
        num += hexChar[1] - ((hexChar[1] <= '9') ? '0' : 'a' - 10);


        text += char(num);
    }

    // Calc Freq
    vector<float> freq(26, 0.0);    
    float textSize = 0.0;
    for(int i = 0; i < text.size(); i++) {
        int curr = tolower(text[i]);
        if ('a' <= curr && curr <= 'z') {
            freq[curr - 'a'] += 1.0;
        }
        textSize += 1.0;
    }
    

    // Calculate distance from idealFreq
    float dist = 0.0;
    for(int i = 0; i < freq.size(); i++) {
        dist += abs(freq[i] / textSize - idealFreq[i]);
    }

    return make_pair(dist, text);
}

int main() {
    vector<float> idealFreq = { 0.08496, 0.020720, 0.04538, 0.03384, 0.11160, 0.018121, 0.024705, 0.030034, 0.07544, 0.001965, 0.011016, 0.05489, 0.030129, 0.06654, 0.07163, 0.03167, 0.001962, 0.07580, 0.05735, 0.06950, 0.03630, 0.010074, 0.012899, 0.002902, 0.017779, 0.002722 };
    
    string input, output;
    ifstream fin;
    fin.open("4.txt");
    
    // Calc min dist based on ideal frequency of characters
    float mingGlobalDist = FLT_MAX;
    while(fin >> input) {
        float minDist = FLT_MAX;
        char minChar;
        string localMin;
        for(unsigned char XORChar = 0; XORChar <= 254; XORChar++) {
            pair<float, string> ret = freqDist(input, XORChar, idealFreq);
            bool isValid = true;
            for(int i = 0; i < ret.second.size(); i++) {
                if (32 <= ret.second[i] && ret.second[i] <= 126) {
                    // Valid
                } else {
                    isValid = false;
                    break;
                }
            }
            if (isValid) {
                cout << ret.second << endl;
            }
            if (isValid && ret.first < minDist) {
                minDist = ret.first;
                minChar = XORChar;
                localMin = ret.second;
            }
        }
        if (minDist < mingGlobalDist) {
            mingGlobalDist = minDist;
            output = localMin;
        }
    }

    cout << output << endl;
    
    return 0;
}