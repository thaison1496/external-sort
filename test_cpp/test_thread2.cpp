// thread example
#include <bits/stdc++.h>       // std::cout
#include <thread>         // std::thread
#include <unistd.h>
using namespace std;

int d = 0;
mutex mux;

void foo() 
{
    mux.lock();
for(int i=0; i<500; i++) cout<<"m";
    mux.unlock();
    return;

 /*for(int i=0; i < 1000000000; i++){
    //d = d + 1;
    d += 1;
    if (i % 100000000 == 0)
      cout << i << endl;
  }*/
  //mux.unlock();
}

void bar()
{ 
 mux.lock();
 for(int i=0; i<500; i++) cout<<"-";
 /*for(int i=0; i < 1000000000; i++){
    //d = d + 1;
    d -= 1;
    
    if (i % 100000000 == 0)
      cout << i << endl;
  }*/
  mux.unlock();
}

int main() 
{
  std::thread first (foo);     // spawn new thread that calls foo()
  std::thread second (bar);  // spawn new thread that calls bar(0)

  std::cout << "main, foo and bar now execute concurrently...\n";

  /*for(int i=0; i < 10000000000; i++){
    //d = d + 1;
    d += 1;
    if (d % 100000000 == 0)
      cout << d << endl;
  }*/

  mux.lock();
  for(int i=0; i<500; i++) cout<<"l";
    mux.unlock();
  second.join(); 
  first.join();
  

  cout << d << endl;
  std::cout << "foo and bar completed.\n";

  return 0;
}