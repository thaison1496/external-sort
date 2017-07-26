#include <bits/stdc++.h>
#include <string>
#include <pthread.h>
#include <unistd.h>

using namespace std;
typedef pair<wstring, int> psi;

// contain partitions currently in RAM that need to be sorted
vector< vector<wstring> > buffer; 

int num_thread; // number of sorting thread = max(1, number of CPU cores - 1)
vector<bool> done; // indicate if i-th partition is sorted and can be deleted from RAM


void sort_partition(int buffer_idx, int curr_partition) {
	vector<wstring>& buff = buffer[buffer_idx];
	char partition_file[10];
	sprintf(partition_file, "%s%d", "part/", curr_partition);
	wofstream wos(partition_file);

	sort(buff.begin(), buff.end());

	// write sorted buffer to file part/{curr_partition}
	for(int i = 0; i < buff.size(); i ++){
		wos << buff[i] << "\n";
	}

	// release the buffer from RAM
	buff.resize(0);
	wos.close();	
	done[buffer_idx] = true;
}


bool get_available_buffer(int& curr_idx) {
	for (int idx = 0; idx < num_thread; idx ++) {
		if (done[idx]) {
			curr_idx = idx;
			return true;
		}		
	}
	return false;
}


bool all_thread_done() {
	for (int idx = 0; idx < num_thread; idx ++) {
		if (!done[idx]) {
			return false;
		}
	}
	return true;
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
			//wos << s << "\n";
			buffer[curr_idx].push_back(s);
			if (curr_size > partition_size) {
				curr_size = 0;
				cont = true; // input file not end yet, continue to read next partition
				break;
			}
		}

		// reserve buffer[idx] for sorting
		done[curr_idx] = false;
		thread sort_thread(sort_partition, curr_idx, curr_partition);
		sort_thread.join();

		curr_partition ++;
		if (!cont)
			break;
	}
	wis.close();
	while (!all_thread_done()) {}
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


void init_buffer(){
	vector<wstring> temp;
	for (int i = 0; i < num_thread; i ++) {
		buffer.push_back(temp);
		done.push_back(true);
	}
}


int main(int argc, char** argv) {
	string infile, outfile;
	int memsize, partition_size;

	infile = argv[1];
	outfile = argv[2];
	memsize = atoi(argv[3]);
	//num_thread = 1;
	num_thread = max(1, (int) thread::hardware_concurrency() - 1);
	partition_size = memsize * 1000000 / 5 / num_thread;
	init_buffer();

	int start = clock();
	int num_partition = get_partition(infile.c_str(), partition_size);
	cout << (clock() - start) / (double) CLOCKS_PER_SEC << endl;
	//int num_partition = 23;
	merge_partition(num_partition, outfile.c_str());
	cout << (clock() - start) / (double) CLOCKS_PER_SEC << endl;
	return 0;
}