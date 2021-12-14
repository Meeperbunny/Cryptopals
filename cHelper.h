#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <float.h>
#include <cmath>
#include <fstream>

using namespace std;

string hexToBase64(string hexString);

vector<bool> hexStringToBoolArr(string s);

string boolArrToHexString(vector<bool> arr);

string hexXOR(string firstString, string secondString);

string hexToAscii(string hexString);

string charToHex(char c);

float freqCalc(string text, vector<float> idealFreq);

string repeatStringXORString(string s, string key);

string stringToHexString(string s);

pair<float, string> freqDist(string s, string c, vector<float> idealFreq);

bool isValidString(string s, float threshold);