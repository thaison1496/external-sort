#include <bits/stdc++.h> 
using namespace std;

int main() {
		int start = clock();
        int count = 0;
        ofstream os("text.txt");
      	ios_base::sync_with_stdio(false);

        //char buffer[1024000];
        //os.rdbuf()->pubsetbuf(buffer, 1024000);

        for(int i = 1; i < 20000000; i++)
        {       
                os << "Hello world!\n";
                count++;
        }
                
        cout << clock() - start;
return 0;
}