#include "../cHelper.h"

int main() {
    string input;
    cout << "Enter encoded string: ";
    cin >> input;
    
    // Calc min dist based on ideal frequency of characters
    string minOutput;
    float minDist = FLT_MAX;

    // Iterate through characters to find min
    for(char XORChar = 0; 0 <= XORChar; XORChar++) {
        pair<float, string> ret = freqDist(input, string(1, XORChar));

        // Check if invalid char due to overflow
        bool isValid = isValidString(ret.second, 0.5);

        // If valid and less that the current min dist, save as current
        if (isValid && ret.first < minDist) {
            minDist = ret.first;
            minOutput = ret.second;
        }
    }

    cout << minOutput << endl;
    return 0;
}