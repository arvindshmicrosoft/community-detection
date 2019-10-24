#include <iostream>
#include <algorithm>
#include <fstream>
#include <cmath>
#include <cstring>
#include <vector>
#include <set>
#include <map>

using namespace std;

const int max_organizations = 27000;

map<int, int> m;
map<int, string> institutions;
vector<pair<long long, long long>> adjacent[max_organizations];
set<vector<long long>> edges;

long double clustering_coefficient[max_organizations];

long long weighted_degree[max_organizations];
int cluster_ids[max_organizations], cluster_size[max_organizations];
int n, k;

int read_data(string input_file){
	ifstream in(input_file);
	int number_of_edges, number_of_nodes;
	int cnt = 0;
	string tag;

	in >> tag >> number_of_nodes;
	for(int i = 0; i < number_of_nodes; i++){
		long long x;
		string name;
		in >> x >> name;
	}

	in >> tag >> number_of_edges;
	for(int i = 0; i < number_of_edges; i++){
		long long a, b, w;
		in >> a >> b >> w;
		adjacent[a].push_back({b, w});
		adjacent[b].push_back({a, w});
		edges.insert({min(a, b), max(a, b), w});

		if(cnt % 10000 == 0){
			cout << cnt << endl;
		}
		cnt++;		
	}
	in.close();

	return number_of_nodes;
}

int read_clusters(string input_file){
	int number_of_clusters;

	ifstream in(input_file);
	in >> number_of_clusters;
	for(int i = 0; i < n; i++){
		in >> cluster_ids[i];
		cluster_size[cluster_ids[i]]++;
	}
	in.close();

	return number_of_clusters;
}

void read_mapping(string number_file, string string_file){
	ifstream in1(number_file);
	long long id, value;

	while(in1 >> id >> value){
		m[value] = id;
	}
	in1.close();

	ifstream in2(string_file);
	string str;

	while(in2 >> value){
		getline(in2, str);
		str = str.substr(1, str.length() - 1);
		institutions[m[value]] = str;
	}
	in2.close();
}

void most_important(string output_file, int number_of_important){
	memset(weighted_degree, 0, sizeof(weighted_degree));
	vector<pair<long long, int>> ordered_institutions; 
	for(int i = 0; i < n; i++){
		for(auto x : adjacent[i]){
			weighted_degree[i] += x.second;
		}
		ordered_institutions.push_back({weighted_degree[i], i});
	}

	sort(ordered_institutions.begin(), ordered_institutions.end());
	reverse(ordered_institutions.begin(), ordered_institutions.end());	

	ofstream out(output_file);
	for(int i = 0; i < ordered_institutions.size() && i < number_of_important; i++){
		int index = ordered_institutions[i].second;
		out << institutions[index] << " " << ordered_institutions[i].first << "\n";
	}	
	out.close();
}

void most_important_in_clusters(string output_file, int number_of_important){
	memset(weighted_degree, 0, sizeof(weighted_degree));
	vector<pair<long long, int>> ordered_institutions[max_organizations]; 
	for(int i = 0; i < n; i++){
		for(auto x : adjacent[i]){
			weighted_degree[i] += x.second;
		}
		ordered_institutions[cluster_ids[i]].push_back({weighted_degree[i], i});
	}

	ofstream out(output_file);
	for(int i = 0; i < max_organizations; i++){
		sort(ordered_institutions[i].begin(), ordered_institutions[i].end());
		reverse(ordered_institutions[i].begin(), ordered_institutions[i].end());	

		if(ordered_institutions[i].size() > 0 && number_of_important > 0){
			out << "Cluster " << i << " (size " << cluster_size[i] << "):\n"; 
		}

		for(int j = 0; j < ordered_institutions[i].size() && j < number_of_important; j++){
			int index = ordered_institutions[i][j].second;
			out << institutions[index] << " " << ordered_institutions[i][j].first << "\n";
		}	

		if(ordered_institutions[i].size() > 0 && number_of_important > 0){
			out << "\n"; 
		}

	}
	out.close();
}



long long find(long long x, long long y){
	auto it = edges.lower_bound({min(x, y), max(x, y), -1});
	if(it != edges.end() && (*it)[0] == min(x, y) && (*it)[1] == max(x, y)){
		return (*it)[2];
	}
	return -1LL;
}

// Useless because graph is too big
void calculate_clustering_coefficient(){
	long double average = 0.0;
	for(int i = 0; i < n; i++){
		cout << i << " " << adjacent[i].size() << endl;

		long long cnt = 0;
		for(auto x : adjacent[i]){
			for(auto y : adjacent[i]){
				if(x != y){
					long long w = find(x.first, y.first);
					if(w > 0){
						cnt += cbrt(x.second * y.second * w);
					}
				}
			}
		}
		clustering_coefficient[i] = 1.0 * cnt / (1.0 * adjacent[i].size() * (adjacent[i].size() - 1));
		average += clustering_coefficient[i];
	}
	cout << 1.0 * average / n << endl;
}

int main(){
	string folder = "k-spanning-tree-clustering/50";
	string graph_name = "cluster_graph_50.net";
	string cluster_name = "k_spanning_tree_50.txt";


//	n = read_data("data/graph.net");
	n = read_data(folder + "/" + graph_name);
	read_mapping("data/remaped_affiliations_info.tsv", "data/affiliations_info.tsv");

//	most_important("data/most_important_institutions.txt", 100);

	k = read_clusters(folder + "/" + cluster_name);
	most_important_in_clusters(folder + "/most_important_institutions_in_clusters_50.txt", 5);

	return 0;
}