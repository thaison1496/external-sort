#include <bits/stdc++.h>
#include <string>
#include <pthread.h>
#include <unistd.h>

using namespace std;

int main() {
	ifstream is("_list_test.txt");
	system("rm _test_result.txt");
	string s;
	while (getline(is, s)) {
		stringstream ss;

		ss << "g++ main_v4.cpp -std=c++11 -pthread"
		<< " && time ./a.out test/"
		<< s
		<< " _mine.out 500 && time sort test/"
		<< s
		<< " > _unix.out && echo \"\n"
		<< s
		<< "\n\" >> _test_result.txt"
		<< " && diff -q _unix.out _mine.out >> _test_result.txt";

		//cout << ss.str() << endl;
		system(ss.str().c_str());
	}
	is.close();

}