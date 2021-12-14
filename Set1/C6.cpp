#include "../cHelper.h"
#define DEBUG false

int main() {
    string input, output;
    string key = "";

    cout << "Getting input..." << endl;
    ifstream fin;
    fin.open("6.txt");
    std::stringstream buffer;
    buffer << fin.rdbuf();
    input = buffer.str();

    // Clean newlines

    input.erase(std::remove(input.begin(), input.end(), '\n'), input.end());

    // Base 64 decrypt to a normal string

    input = base64DecryptToHex(input);

    vector<bool> inputBoolArr = hexStringToBoolArr(input);

    float minNormDist = FLT_MAX;
    int minKeySize = -1;

    for(int keySize = 2; keySize <= 40; keySize++) {
        vector<bool> first, second, third, fourth;

        // Get first keySize block
        for(int i = 0; i < keySize * 8 && (i < inputBoolArr.size()); i++)
            first.push_back(inputBoolArr[i]);

        // Get second keySize block
        for(int i = 0; i < keySize * 8 && (keySize * 8 + i < inputBoolArr.size()); i++)
            second.push_back(inputBoolArr[keySize * 8 + i]);

        // Get third keySize block
        for(int i = 0; i < keySize * 8 && (2 * keySize * 8 + i < inputBoolArr.size()); i++)
            third.push_back(inputBoolArr[2 * keySize * 8 + i]);

        // Get fourth keySize block
        for(int i = 0; i < keySize * 8 && (3 * keySize * 8 + i < inputBoolArr.size()); i++)
            fourth.push_back(inputBoolArr[3 * keySize * 8 + i]);

        float totalHammingDistance = hammingDistance(first, second) + hammingDistance(first, third) + hammingDistance(first, fourth) + 
                                     hammingDistance(second, third) + hammingDistance(second, fourth) + 
                                     hammingDistance(third, fourth);

        float dist = float(totalHammingDistance) / float(6 * keySize);

        if (dist < minNormDist) {
            minNormDist = dist;
            minKeySize = keySize;

            if (DEBUG) cout << "New min dist... " << minNormDist << ':' << minKeySize << endl;
        }
    }

    if (DEBUG) cout << "Minimum normal distance: " << minNormDist << endl;
    if (DEBUG) cout << "Minimum key size: " << minKeySize << endl;

    vector<vector<bool>> sepBlocks(minKeySize, vector<bool>(0));

    for(int i = 0; i < minKeySize; i++) {
        for(int q = 8 * i; q < inputBoolArr.size(); q += 8 * minKeySize) {
            for(int n = q; n < q + 8 && n < inputBoolArr.size(); n++) {
                sepBlocks[i].push_back(inputBoolArr[n]);
            }
        }
    }

    if (DEBUG) cout << "Solving for strings" << endl;

    vector<string> sepStrings(minKeySize, ""), solvedStrings(minKeySize, "");
    for(int i = 0; i < sepBlocks.size(); i++) {
        sepStrings[i] = boolArrToHexString(sepBlocks[i]);
    }

    for(int n = 0; n < minKeySize; n++) {
        // Solve for single byte XOR

        // Calc min dist based on ideal frequency of characters
        string minOutput;
        float minDist = FLT_MAX;
        char minChar;

        // Iterate through characters to find min
        for(char XORChar = 0; 0 <= XORChar; XORChar++) {
            pair<float, string> ret = freqDist(sepStrings[n], string(1, XORChar));

            // Check if invalid char due to overflow
            bool isValid = isValidString(ret.second, 0.5);

            // If valid and less that the current min dist, save as current

            if (isValid && ret.first < minDist) {
                minDist = ret.first;
                minOutput = ret.second;
                minChar = XORChar;
            }
        }
        
        key += minChar;
        solvedStrings[n] = minOutput;
    }

    if (DEBUG) cout << "Finished solving for strings... arranging output" << endl;

    output = "";
    bool gettingInput = true;
    while(gettingInput) {
        gettingInput = false;
        for(int i = 0; i < solvedStrings.size(); i++) {
            if (solvedStrings[i].size() == 0) continue;
            gettingInput = true;
            output += solvedStrings[i][0];
            solvedStrings[i] = solvedStrings[i].substr(1);
        }
    }

    cout << output << endl;
    cout << "Key: " << key << endl;

    return 0;
}