#include "../cHelper.h"

int main() {
    string input, output;
    string repeatingKey;

    cout << "Getting input..." << endl;
    ifstream fin;
    fin.open("5.txt");
    std::stringstream buffer;
    buffer << fin.rdbuf();
    input = buffer.str();

    cout << "Enter repeating key: ";
    cin >> repeatingKey;

    input = repeatStringXORString(stringToHexString(input), repeatingKey);

    output = stringToHexString(input);

    cout << output << endl;;
    return 0;
}