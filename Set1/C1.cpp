#include "../cHelper.h"

int main() {
    string input;
    cout << "Hex String: ";
    cin >> input;
    cout << hexToBase64(input) << endl;
    return 0;
}