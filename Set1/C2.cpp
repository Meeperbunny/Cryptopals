#include "../cHelper.h"

int main() {
    string first, second;
    cout << "First Hex String: ";
    cin >> first;
    cout << "Second Hex String: ";
    cin >> second;
    cout << hexXOR(first, second) << endl;
    return 0;
}