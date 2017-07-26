#include <bits/stdc++.h> 
using namespace std;

int main() {
    /*
	vector<wstring> buff;
    wofstream wos("text.out");
    for(int i = 0; i < 1000000; i++) {
        buff.push_back(L"0123456789");
        wos << L"0123456789";
    }
    wos.close();
*/
    // vector<char*> buff;
    cout << sizeof(wchar_t) << endl;
    //char** buff;
    //buff = new char*[1000000];
    wstring buff[1000000];
    wstring s;
    //stringstream ss;
    ofstream wos("text.out");
    for(int i = 0; i < 1000000; i++) {
       //buff.push_back("0123456789");
        buff[i] = L"0123456789";
        //ss << "0123456789";
        //s += L"0123456789";
        wos << "0123456789";
    }
    wos.close();
    while (true) {};
    return 0;
}