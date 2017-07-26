#include <bits/stdc++.h>
#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <unistd.h>

using namespace std;


string s;

int main() {
   vector<string> buff;
   vector<string> emp;
   int partition_size = 100 * 1000000 / 5;
   ifstream wis("text.txt");
   int d = 0;
   while (getline(wis, s)) {
      buff.push_back(s);
      d += s.length();
      if (d > partition_size){
         cout << buff.size() << " " << buff.capacity() << endl;
         struct timespec start, finish;
         double elapsed;
         clock_gettime(CLOCK_MONOTONIC, &start);

         sort(buff.begin(), buff.end());      

         clock_gettime(CLOCK_MONOTONIC, &finish);
         elapsed = (finish.tv_sec - start.tv_sec);
         elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
         cout << elapsed << endl;

         ofstream wos("text.out");
         for(int i = 0; i < buff.size(); i++ ){
            wos << buff[i] << "\n";
         }
         wos.close();
         buff = vector<string>();
         // buff.clear();
         d = 0;
      }
   }
   wis.close();
   
   return 0; 
}