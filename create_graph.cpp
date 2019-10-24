#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <map>

using namespace std;

const int max_organizations = 27000;

map<long long, long long> mapped_ids;
map<long long, long long> adjacent[max_organizations];
vector<vector<long long>> edges;


int main(){
	string line;
	long long new_id, id;

	ifstream in("data/remaped_affiliations_info.tsv");
	while(in >> new_id >> id){
		mapped_ids[id] = new_id; 
	}
	in.close();


	int cnt = 0;
	long long number_of_nodes = 0;
	ifstream in1("data/papers_info.tsv");
	while(getline(in1, line)){
		stringstream s(line);
		long long id, year, organization;
		set<long long> organizations;

		s >> id >> year;
		while(s >> organization){
			organizations.insert(mapped_ids[organization]);
		}

		for(auto org1 = organizations.begin(); org1 != organizations.end(); org1++){
			number_of_nodes = max(number_of_nodes, *org1 + 1);
			for(auto org2 = organizations.begin(); org2 != organizations.end(); org2++){
				if(org1 != org2){
					adjacent[*org1][*org2] += 1;
				}
			}
		}
		
		if(cnt % 10000 == 0){
			cout << cnt << endl;
		}
		cnt++;
	}
	in1.close();
	
	
	for(int i = 0; i < max_organizations; i++){
		for(auto it = adjacent[i].begin(); it != adjacent[i].end(); it++){
			// To prevent duplicated edges
			if(i <= it -> first){
				edges.push_back({i, it -> first, it -> second});
			}
		}
	}


	int number_of_edges = edges.size();
	ofstream out("data/graph.net");
	out << "*vertices " << number_of_nodes << endl;
	for(int i = 0; i < number_of_nodes; i++){
		out << i << " \"" << i << "\"" << "\n";
	}

	out << "*edges " << number_of_edges << endl;
	for(int i = 0; i < edges.size(); i++){
		out << edges[i][0] << " " << edges[i][1] << " " << edges[i][2] << "\n";
	}
	out.close();
	
	return 0;
}