#include "../cHelper.h"

int main() {
    string input, output = "";
    string key = "YELLOW SUBMARINE";

    cout << "Getting input..." << endl;
    ifstream fin;
    fin.open("7.txt");
    std::stringstream buffer;
    buffer << fin.rdbuf();
    input = buffer.str();

    // Clean newlines

    input.erase(std::remove(input.begin(), input.end(), '\n'), input.end());

    string hexInput = base64DecryptToHex(input);

    // ##### AES decode: #####

    vector<uint8_t> bytes = hexToByteArr(hexInput);

    for(int i = 0; i < 32; i++) {
        cout << int(bytes[i]) << ' ';
    } cout << endl;
    
    const int blockSize = 16;
    for(int i = 0; i < bytes.size(); i += 16) {
        // Get current block
        vector<uint8_t> block(bytes.begin() + i, bytes.begin() + i + blockSize);
        // Decrypt using AES-128
        block = AES::AES128Decrypt(block, key);
        // Append block to text
        for(int q = 0; q < blockSize; q++) {
            output += block[q];
        }
    }

    cout << output << endl;

    return 0;
}