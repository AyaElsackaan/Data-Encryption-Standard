#pragma once
#include <string>
using namespace std;

class DataBlock{
    private:
        string data;
    public:
        DataBlock(string s);
        string toHex(string s);
};