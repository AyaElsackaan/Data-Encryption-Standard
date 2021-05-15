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

#include"StandardTables.h"

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
