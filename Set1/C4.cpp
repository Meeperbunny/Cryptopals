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

pair<float, string> freqDist(string s, char c, vector<float> idealFreq) {
    string first = s;
    string second = "";

    // Fill second string with hex version of c
    string cHex = charToHex(c); 
    for(int i = 0; i < first.size() / 2; i++)
        second += cHex;

    string XORed = hexXOR(first, second);
    string text = hexToAscii(XORed);

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
int main() {
    vector<float> idealFreq = { 0.08496, 0.020720, 0.04538, 0.03384, 0.11160, 0.018121, 0.024705, 0.030034, 0.07544, 0.001965, 0.011016, 0.05489, 0.030129, 0.06654, 0.07163, 0.03167, 0.001962, 0.07580, 0.05735, 0.06950, 0.03630, 0.010074, 0.012899, 0.002902, 0.017779, 0.002722 };
    
    ifstream fin;
    fin.open("4.txt");
    string input, output;
    float globMin = FLT_MAX;
    while(fin >> input) {
        // Calc min dist based on ideal frequency of characters
        string minOutput = "";
        float minDist = FLT_MAX;


        // Iterate through characters to find min
        for(char XORChar = 0; 0 <= XORChar; XORChar++) {
            pair<float, string> ret = freqDist(input, XORChar, idealFreq);

            // Check if invalid char due to overflow
            bool isValid = isValidString(ret.second, 0.7);

            // If valid and less that the current min dist, save as current
            if (isValid && ret.first < minDist) {
                minDist = ret.first;
                minOutput = ret.second;
            }
        }
        if (minOutput != "" && minDist < globMin) {
            globMin = minDist;
            output = minOutput;
        }
    }

    cout << output << endl;;
    return 0;
}