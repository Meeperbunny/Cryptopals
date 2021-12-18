#ifndef CHELPER
#define CHELPER

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <float.h>
#include <cmath>
#include <fstream>
#include <algorithm>
#define ROTL8(x,shift) ((uint8_t) ((x) << (shift)) | ((x) >> (8 - (shift))))

using namespace std;

string hexToBase64(string hexString);

string base64DecryptToHex(string s);

vector<bool> hexStringToBoolArr(string s);

string boolArrToHexString(vector<bool> arr);

string hexXOR(string firstString, string secondString);

string hexToAscii(string hexString);

string charToHex(char c);

float freqCalc(string text, vector<float> idealFreq);

string repeatStringXORString(string s, string key);

string stringToHexString(string s);

pair<float, string> freqDist(string s, string c);

bool isValidString(string s, float threshold);

int hammingDistance(string a, string b);

int hammingDistance(vector<bool> a, vector<bool> b);

uint8_t hexToByte(string hex);

vector<uint8_t> hexToByteArr(string hex);

namespace AES {
    vector<uint8_t> AES128Decrypt(vector<uint8_t> block, string key, bool DEBUG);
    
    void shiftRow(vector<uint8_t> &block, char mode, bool DEBUG);
    void byteSub(vector<uint8_t> &block, char mode, bool DEBUG);
    void addRound(vector<uint8_t> &block, vector<uint8_t> &eKey, string key, int round, char mode, bool DEBUG);
    void mixCol(vector<uint8_t> &block, char mode, bool DEBUG);

    namespace Helpers {
        void rotWord(vector<uint8_t> &bytes);
        void subWord(vector<uint8_t> &bytes, char mode);
        void getExpandedKey(vector<uint8_t> &eKey, string key, int round, char mode);
        vector<uint8_t> rCon(int n);

        vector<uint8_t> EK(vector<uint8_t> eKey, int n);
        vector<uint8_t> K(string key, int n);

        extern uint8_t E[256];
        extern uint8_t L[256];
    }
}

#endif