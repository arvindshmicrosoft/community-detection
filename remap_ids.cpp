#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>

using namespace std;

vector<long long> ids;

int main(){
	string line;

	ifstream in("data/affiliations_info.tsv");
	while(getline(in, line)){
		stringstream s(line);
		long long id;

		s >> id;
		cout << line;
		cout << id << endl;
		ids.push_back(id);
	}

	sort(ids.begin(), ids.end());

	ofstream out("data/remaped_affiliations_info.tsv");
	for(int i = 0; i < ids.size(); i++){
		out << i << "\t" << ids[i] << "\n";
	}
	in.close();
	out.close();

	return 0;
}