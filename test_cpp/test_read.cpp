#include <bits/stdc++.h> 
using namespace std;

int main() {
	wifstream wis("text.txt");
    wofstream wos("text.out");
    wstring s;
    int c = 0, d = 0;
    int start = clock();
    wstringstream wss;
    while (getline(wis,s)){
        wos << s << "\n";
        /*c += s.length();
        if (c > 1){
            wos << wss.str();
            wss.str(L"");
            c = 0;
            d++;
        }
        wss << s << endl;*/
    }
    cout << d << endl;
    wos << wss.str();
    wis.close(); wos.close();
    cout << c << endl;
    cout << (clock() - start) / (double) CLOCKS_PER_SEC << endl;
    return 0;
}