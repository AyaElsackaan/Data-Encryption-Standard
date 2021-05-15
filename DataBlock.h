#pragma once
#include <string>
#include <vector>
using namespace std;

class DataBlock{
    private:
        string data;
    public:
        DataBlock(string s);
        string toHex(string s);
        string permutation(vector<int>table, int new_size);
        string shift( int n);

};