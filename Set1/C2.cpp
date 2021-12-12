#include <iostream>
#include <vector>
#include <string>
#include <cmath>

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

int main() {
    string first, second;
    cout << "First Hex String: ";
    cin >> first;
    cout << "Second Hex String: ";
    cin >> second;
    cout << hexXOR(first, second) << endl;
    return 0;
}