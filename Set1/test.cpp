#include "../cHelper.h"

int main() {
    vector<uint8_t> byte = {
        0x04, 0x66, 0x81, 0xE5,
        0x04, 0x66, 0x81, 0xE5,
        0x04, 0x66, 0x81, 0xE5,
        0x04, 0x66, 0x81, 0xE5
    };
    AES::mixCol(byte, 'D');
    for(int i = 0; i < byte.size(); i++) cout << int(byte[i]) << ' ';
    cout << endl;
    return 0;
}