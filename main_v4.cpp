#include <bits/stdc++.h>
#include <string>
#include <pthread.h>
#include <unistd.h>

using namespace std;
typedef pair<wstring, int> psi;

int num_thread = max(1, (int) thread::hardware_concurrency() - 1); 

// contain partitions currently in RAM that need to be sorted
// buffer[i] is sorted by thread i-th
vector<wstring> buffer[100]; 

// thread_lock[i] = k if thread i-th is sorting k-th partition
// thread_lock[i] = -1 if thread i-th available
int thread_lock[100]; 

thread threads[100];
bool done_read_input = false;


void sort_thread(int idx) {
	vector<wstring>& buff = buffer[idx];
	
	while (!done_read_input) {
		while (thread_lock[idx] == -1) { sleep(0.1); }
		char partition_file[10];
		sprintf(partition_file, "%s%d", "part/", thread_lock[idx]);
		wofstream wos(partition_file);

		sort(buff.begin(), buff.end());	

		for(int i = 0; i < buff.size(); i ++) {
			wos << buff[i] << "\n";
		}
		buff.resize(0);
		buff.shrink_to_fit();
		wos.close();	
		thread_lock[idx] = -1;
	}
}


bool get_available_buffer(int& curr_idx) {
	for (int idx = 0; idx < num_thread; idx ++) {
		if (thread_lock[idx] == -1) {
			curr_idx = idx;
			return true;
		}		
	}
	return false;
}


int get_partition(const char* infile, int partition_size) {
	int curr_size = 0; // number of char on the current buffer for reading input
	int curr_partition = 0; 
	int curr_idx = 0; // indicate buffer[idx] is currently used for reading input
	wstring s;
	wifstream wis(infile);

	system("mkdir part"); // temporary files is stored in "part" folder
	while (true) {
		bool cont = false; // check end of input file

		while (getline(wis, s)) {
			curr_size += s.length();
			buffer[curr_idx].push_back(s);
			if (curr_size > partition_size) {
				curr_size = 0;
				cont = true; // input file not end yet, continue to read next partition
				break;
			}
		}
		thread_lock[curr_idx] = curr_partition;

		// find an available sort thread
		while (!get_available_buffer(curr_idx)) {
		 	sleep(0.1);
		}
	
		curr_partition ++;
		if (!cont)
			break;
	}
	wis.close();
	return curr_partition;
}


bool all_thread_done() {
	for(int i = 0; i < num_thread; i ++) {
		if (thread_lock[i] != -1) {
			return false;
		}
	}
	return true;
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
		if (getline(wis[idx], s)) { // not empty partition file
			pq.push(make_pair(s, idx));
		}
	}
	
	while (!pq.empty()) {
		wos << pq.top().first << "\n";
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
	locale::global(locale("en_US.utf8"));
	string infile, outfile;
	int memsize, partition_size;

	infile = argv[1];
	outfile = argv[2];
	memsize = atoi(argv[3]);
	
	partition_size = memsize * 1000000 / 5 / num_thread;
	
	for (int idx = 0; idx < num_thread; idx ++) {
		thread_lock[idx] = -1;
		threads[idx] = thread(sort_thread, idx);
		threads[idx].detach();
	}

	int num_partition = get_partition(infile.c_str(), partition_size);

	done_read_input = true;
	while (!all_thread_done()) {
		sleep(0.1);
	}

	merge_partition(num_partition, outfile.c_str());
	return 0;
}