#include <bits/stdc++.h>
using namespace std;

typedef pair<string, int> psi;

int main(){

	priority_queue< psi, vector<psi>, greater<psi> > pq;
	pq.push(make_pair("b",1));
	pq.push(make_pair("a",2));

	cout<<pq.top().first<<endl;
	return 0;
}