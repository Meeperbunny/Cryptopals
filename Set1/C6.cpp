#include "../cHelper.h"

int main() {
    string a, b;
    getline(cin, a);
    getline(cin, b);
    cout << hammingDistance(a, b) << endl;

    return 0;
}