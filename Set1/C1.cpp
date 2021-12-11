#include <iostream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

string hexToBase64(string hexString) {
    // Convert hex to boolean array
    vector<bool> arr;
    string ret = "";
    for(unsigned int i = 0; i < hexString.size(); i++) {
        // Hex to num
        int num;
        char lowerChar = tolower(hexString[i]);
        if ('0' <= lowerChar && lowerChar <= '9') {
            num = int(hexString[i] - '0');
        }
        else if ('a' <= lowerChar && lowerChar <= 'f') {
            num = int(lowerChar - 'a') + 10;
        } 
        else continue;

        // Num to 4-bits
        for(int q = 3; q >= 0; q--) {
            int sub = int(pow(2, q));
            if (num >= sub) {
                arr.push_back(1);
                num -= sub;
            }
            else {
                arr.push_back(0);
            }
        }
    }
        
    // Add padding
    int paddingSize = (6 - (hexString.size() % 6)) % 6;
    for(int i = 0; i < paddingSize * 4; i++) {
        arr.push_back(0);
    }

    // Change back to bytes, not 4-bits
    paddingSize = paddingSize / 2;

    // Convert each 6-byte block into table
    for(int i = 0; i < arr.size(); i += 6) {
        // Get block value
        int num = 0;
        for(int q = 0; q < 6; q++) {
            num += int(pow(2, 5 - q)) * int(arr[i + q]);
        }

        // Convert to num
        char appendChar;
        if (0 <= num && num <= 25) {
            appendChar = char('A' + num);
        }
        else if (26 <= num && num <= 51) {
            appendChar = char('a' + num - 26);
        }
        else if (52 <= num && num <= 61) {
            appendChar = char('0' + num - 52);
        }
        else if (num == 62) {
            appendChar = '+';
        }
        else if (num == 63) {
            appendChar = '/';
        }

        ret += appendChar;
    }

    for(int i = 0; i < paddingSize; i++) {
        ret[ret.size() - 1 - i] = '=';
    }

    return ret;
}

int main() {
    string input;
    cout << "Hex String: ";
    cin >> input;
    cout << hexToBase64(input) << endl;
    return 0;
}