#include <iostream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

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

int main() {
    string input;
    cout << "Hex String: ";
    cin >> input;
    cout << hexToBase64(input) << endl;
    return 0;
}