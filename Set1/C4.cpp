#include "../cHelper.h"

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
            pair<float, string> ret = freqDist(input, string(1, XORChar), idealFreq);

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