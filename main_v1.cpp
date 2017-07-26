#include <bits/stdc++.h>
#include <string>
#include <pthread.h>
#include <unistd.h>

using namespace std;
typedef pair<wstring, int> psi;

vector<wstring> buffer;

int get_partition(const char* infile, int partition_size) {
	int curr_size = 0, curr_partition = 0;
	wstring s;
	wifstream wis(infile);
	system("rm -r part");
	system("mkdir part");
	while (true) {
		char partition_file[10];
		bool cont = false;

		sprintf(partition_file, "%s%d", "part/", curr_partition);
		wofstream wos(partition_file);

		curr_partition ++;
		//cout << curr_partition << endl;
		while (getline(wis, s)) {
			curr_size += s.length();
			//wos << s << endl;
			buffer.push_back(s);
			if (curr_size > partition_size) {
				curr_size = 0;
				cont = true;
				break;
			}
		}
		sort(buffer.begin(), buffer.end());
		for(int i = 0; i < buffer.size(); i ++){
			wos << buffer[i] << endl;
		}
		wos.close();	
		buffer.resize(0);
		if (!cont)
			break;
	}
	wis.close();
	return curr_partition;
}


void merge_partition(int num_partition, const char* outfile){
	wifstream wis[num_partition];
	wofstream wos(outfile);
	priority_queue< psi, vector<psi>, greater<psi> > pq;
	wstring s;
	char partition_file[10];
	int idx;

	for (int idx = 0; idx < num_partition; idx ++) {
		sprintf(partition_file, "%s%d", "part/", idx);
		wis[idx].open(partition_file);
		getline(wis[idx], s);
		pq.push(make_pair(s, idx));
	}
	
	while (!pq.empty()) {
		wos << pq.top().first << endl;
		idx = pq.top().second;
		pq.pop();
		if (getline(wis[idx], s)) {
			pq.push(make_pair(s, idx));
		} else {
			wis[idx].close();
		}
	}
	system("rm -r part");
	wos.close();
}


int main(int argc, char** argv) {
	string infile, outfile;
	int memsize, partition_size;

	infile = argv[1];
	outfile = argv[2];
	memsize = atoi(argv[3]);
	partition_size = memsize * 1000000 / 5;

	int start = clock();
	int num_partition = get_partition(infile.c_str(), partition_size);
	//int num_partition = 23;

	merge_partition(num_partition, outfile.c_str());

	cout << (clock() - start) / (double) CLOCKS_PER_SEC << endl;
	return 0;
}