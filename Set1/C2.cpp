#include <iostream>
#include <vector>
#include <string>
#include <cmath>

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

int main() {
    string first, second;
    cout << "First Hex String: ";
    cin >> first;
    cout << "Second Hex String: ";
    cin >> second;
    cout << hexXOR(first, second) << endl;
    return 0;
}