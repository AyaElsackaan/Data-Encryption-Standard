#include <map>
using namespace std;

#include "DataBlock.h"
DataBlock::DataBlock(string s)
{
    data=s;
}

string DataBlock::toHex(string s)
{
    map<string, string> val;
    val["0000"] = "0";
    val["0001"] = "1";
    val["0010"] = "2";
    val["0011"] = "3";
    val["0100"] = "4";
    val["0101"] = "5";
    val["0110"] = "6";
    val["0111"] = "7";
    val["1000"] = "8";
    val["1001"] = "9";
    val["1010"] = "A";
    val["1011"] = "B";
    val["1100"] = "C";
    val["1101"] = "D";
    val["1110"] = "E";
    val["1111"] = "F";
    string hex = "";
    for (int i = 0; i < s.length(); i += 4) {
        hex += val[s.substr(i,4)];
    }
    return hex;
}
