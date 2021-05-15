#include <iostream>
#include <vector>
#include <math.h>
#include<map>
#include <string>
#include <bitset>
#include <chrono>
#include<algorithm>
#include<map>
using namespace std::chrono;
using namespace std;

string permutation(string initial, vector<int>table, int new_size)
{
    string final="";
    for(int i=0; i<new_size;i++)
    {
        final+=initial[table[i]-1];
    }
    return final;

}

string shift(string initial, int n)
{
    string final="";
   
	for(int i=0; i<initial.length()-n;i++)
	{
		final+=initial[ i+n ];
	}
	for(int i=0; i<n;i++)
	{
		final+= initial[i];       
	}
    return final;
}


vector<string> processKey(string key)
{
    vector<int> pc_1={ 57,   49,    41,   33,    25 ,   17,    9,
                        1,   58,    50,   42,    34,    26,   18,
                        10,    2,    59,   51,    43,    35,   27,
                        19,   11,     3,   60,    52,    44,   36,
                        63,   55,    47,   39,    31,    23,   15,
                        7,   62,    54,   46,    38,    30,   22,
                        14,    6,    61,   53,    45,    37,   29,
                        21,   13,     5,   28,    20,    12,    4};

    vector<int> pc_2={14,	17,	 11,	24,	  1,	5,
                        3, 	 28,	 15,  	 6, 	21, 	10,
                        23,	 19,	 12,	  4,	 26,	  8,
                        16,	  7,	 27,	 20,	 13,	  2,
                        41,	 52,	 31,	 37,	 47,	 55,
                        30,	 40,	 51,	 45,	 33,	 48,
                        44,	 49,	 39,	 56,	 34,	 53,
                        46,	 42,	 50,	 36,	 29,	 32};

    vector<int>shift_table={1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};

    string key_bin=bitset<64>(stoull(key,0,16)).to_string();
    string key_pc1=permutation(key_bin, pc_1, 56);

    string C,D;
    vector<string> result(17);

    //split
    C=key_pc1.substr(0,28);
    D=key_pc1.substr(28,28);
        
    for(int i=1; i<=16; i++)
    {
        //shift
       C=shift(C,shift_table[i-1]);
       D=shift(D,shift_table[i-1]);
       result[i]=permutation(C+D,pc_2,48);

    }
   return result;
}

string encode_block(string plaintext, vector<string>keys)
{
    vector<int> ip={ 58,    50,   42,    34,    26,   18,    10,    2,
            60,    52,   44,    36,    28,   20,    12,    4,
            62,   54,   46,    38,    30,   22,    14,    6,
            64,   56 ,  48 ,   40,    32,   24 ,   16,    8,
            57,    49,   41,    33,    25,   17,     9,    1,
            59,    51,   43,    35,    27,   19,    11,    3,
            61,    53,   45,    37,    29,   21,    13,    5,
            63,    55,   47,    39,    31,   23,    15,    7};

    vector<int> E_table={32,     1,   2,     3,     4,    5,
                  4,     5,    6,     7,     8,    9,
                  8,     9,   10,    11,    12,   13,
                 12,    13,   14,    15,    16,   17,
                 16,    17,   18,    19,    20,   21,
                 20,    21,   22,    23,    24,   25,
                 24,    25,   26,    27,   28,   29,
                 28,    29,   30,    31,    32,    1};

    vector<vector<int>> S_boxes={{ 14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
                          0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
                          4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
                          15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13 },

                        { 15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
                          3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
                          0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
                          13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9 },
  
                        { 10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
                          13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
                          13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
                          1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12 },

                        { 7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
                          13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
                          10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
                          3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14 },

                        { 2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
                          14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
                          4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
                          11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3 },

                        { 12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
                          10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
                          9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
                          4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13 },

                        { 4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
                          13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
                          1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
                          6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12 },

                        { 13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
                          1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
                          7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
                          2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11 } };

    vector<int> p_last={ 16, 7, 20, 21,
                    29, 12, 28, 17,
                    1, 15, 23, 26,
                    5, 18, 31, 10,
                    2, 8, 24, 14,
                    32, 27, 3, 9,
                    19, 13, 30, 6,
                    22, 11, 4, 25 };

    vector<int> ip_inverse={ 40, 8, 48, 16, 56, 24, 64, 32,
                           39, 7, 47, 15, 55, 23, 63, 31,
                           38, 6, 46, 14, 54, 22, 62, 30,
                           37, 5, 45, 13, 53, 21, 61, 29,
                           36, 4, 44, 12, 52, 20, 60, 28,
                           35, 3, 43, 11, 51, 19, 59, 27,
                           34, 2, 42, 10, 50, 18, 58, 26,
                           33, 1, 41, 9, 49, 17, 57, 25 };
  
    string text_bin=bitset<64>(stoull(plaintext,0,16)).to_string();
    text_bin=permutation(text_bin, ip, 64);  
    vector<string> L(17);
    vector<string> R(17);

    //split
    L[0]=text_bin.substr(0,32);
    R[0]=text_bin.substr(32,32);

    //16-rounds
    string expanded_R;
    for(int i=1; i<=16; i++)
    {
        L[i]=R[i-1];
        expanded_R=permutation(R[i-1],E_table,48);
        //xor with key
        expanded_R=bitset<48>(stoull(expanded_R,0,2)^stoull( keys[i],0,2)). to_string();
        //split into 8 s-box inputs
        vector<string> B(8);
        string sbox_output="";
        int index=0, m, n;
        for(int j=0; j<8; j++)
        {

            //split into 6-bit blocks
            B[j]=expanded_R.substr(6*j, 6);

            //s-box index
            m=(B[j][0]-'0')*2+(B[j][5]-'0');//row

            n=(B[j][1]-'0')*8+(B[j][2]-'0')*4+(B[j][3]-'0')*2+(B[j][4]-'0');//col

            sbox_output+=bitset<4>(S_boxes[j][m*16+n]).to_string();

        }

        string after_per=permutation(sbox_output,p_last,32);

        //xor
        R[i]=bitset<32>(stoull(after_per,0,2)^stoull( L[i-1],0,2)). to_string();


    }
    return permutation(R[16]+L[16],ip_inverse,64);

} 

string toHex(string s)
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
int main()
{
  string k, plaintext;
  int encrypt_or_decrypt;
  cout<<"enter 0 to encrypt, 1 to decrypt: "<<endl;
  cin>>encrypt_or_decrypt;

  if(encrypt_or_decrypt==0)
  {
      cout<<"Please enter a 64-bit block of plaintext in hexadecimal: "<<endl;
      cin>>plaintext;

      cout<<"Please enter a 64-bit key in hexadecimal: "<<endl;
      cin>>k;

    cout<< "ecrypted message is: "<<endl;
    auto start = high_resolution_clock::now();
    vector<string>keys=processKey(k);
    string s=toHex(encode_block(plaintext,keys));
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(stop - start);
    cout<<s<<endl;
    cout<<"Time needed for encrytion in nanoseconds= "<<duration.count()<<endl;
  }
  else
  {

      cout<<"Please enter a 64-bit block of ciphertext in hexadecimal: "<<endl;
      cin>>plaintext;

      cout<<"Please enter a 64-bit key in hexadecimal: "<<endl;
      cin>>k;

    cout<< "decrypted message is: "<<endl;

    auto start = high_resolution_clock::now();
    vector<string>keys=processKey(k);

    keys.push_back("");
    reverse(keys.begin(),keys.end());
    string s= toHex(encode_block(plaintext,keys));
    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<nanoseconds>(stop - start);
    cout<<s<<endl;
    cout<<"Time needed for decrytion in nanoseconds= "<<duration.count()<<endl;
  }
  return 0;
}