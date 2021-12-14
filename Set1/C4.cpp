#include "../cHelper.h"

int main() {
    cout << "Getting input..." << endl;
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
            pair<float, string> ret = freqDist(input, string(1, XORChar));

            // Check if invalid char due to overflow
            bool isValid = isValidString(ret.second, 0.5);

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