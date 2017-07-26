#include <bits/stdc++.h> 
#include <unistd.h>
#include <cstdlib>
using namespace std;

void ascii() {
	ofstream os("test/5gb_ascii.txt");
	ios_base::sync_with_stdio(false);
	
	int n;
	for(int i = 1; i < 20000*5000; i++)
	{       
		n = rand() % 90 + 10;
		stringstream ss;
		while (n--) {
				ss << char(rand() % 26 + 97);
		}
		os << ss.str() << "\n";
	}
}


void uni() {
	locale::global(locale("en_US.utf8"));
	wofstream wos("test/1mb_uni");
	
	int n;
	for(int i = 1; i < 20000/3; i++)
	{       
		n = rand() % 80 + 10;
		wostringstream ss;
		while (n--) {
				ss << wchar_t(rand() % 64000 + 300);
		}
		wos << ss.str() << L"\n";
	}
	wos.close();
}


int main() {
	srand(time(NULL));
	ascii();	

	return 0;
}