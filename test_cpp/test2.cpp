#include <bits/stdc++.h>
#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <unistd.h>

using namespace std;

void writeUni(){
  
}

void readUni(){
   wifstream wis("../test/5line_uni");
   //wofstream wos("test2.out");
   
   wstring s;
   int d = 0;
   //wcout << wchar_t(1233) << endl;
   while (getline(wis, s)) {
      //cout << d++ << endl;
      wcout << s << endl;
      //cout<< " " << int(s[1]) << endl;
   }
   wis.close();
   
}

void cmpUni() {
   wstring w1 = L"ột";
   wstring w2 = L"Hai";
   cout << (w1 < w2) << endl;
   string s1 = "ột";
   string s2 = "Hai";
   
   cout << (s1 < s2);

}

int main() {
   locale::global(locale("en_US.utf8"));
   //cout << ">>";
   //writeUni();
   readUni();
   //cmpUni();
   
   return 0; 
}