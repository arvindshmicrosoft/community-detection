#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <set>
#include <map>

using namespace std;

const int max_organizations = 27000;

vector<vector<long long>> edges;
int cluster[max_organizations], adjacent_edges[max_organizations];
int n, k;

int root(int x){
	if(cluster[x] == x){
		return x;
	}

	return cluster[x] = root(cluster[x]);
}


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
		edges.push_back({w, a, b});
		
		if(cnt % 10000 == 0){
			cout << cnt << endl;
		}
		cnt++;		
	}
	in.close();

	return number_of_nodes;
}

void output_clusters(string output_file, int number_of_clusters, vector<long long> cluster_ids){
	ofstream out(output_file);
	out << number_of_clusters << endl;
	out << cluster_ids[0];
	for(int i = 1; i < cluster_ids.size(); i++){
		out << " " << cluster_ids[i];
	}
	out << endl;
	out.close();
}


int main(){
	srand(time(NULL));
	n = read_data("data/graph.net");
	k = 10;
	
	sort(edges.begin(), edges.end());
	reverse(edges.begin(), edges.end());

	for(int i = 0; i < edges.size(); i++){
		long long a = edges[i][1];
		long long b = edges[i][2];

		adjacent_edges[a]++;
		adjacent_edges[b]++;
	}

	int number_of_clusters = 0;
	for(int i = 0; i < max_organizations; i++){
		cluster[i] = i;
		if(adjacent_edges[i] > 0){
			number_of_clusters++;
		}
	}

	cout << number_of_clusters << endl;
	
	for(int i = 0; i < edges.size() && number_of_clusters > k; i++){
		long long w = edges[i][0];
		long long a = edges[i][1];
		long long b = edges[i][2];

		if(root(a) != root(b)){
			cluster[root(a)] = root(b);
			number_of_clusters--;
		}
	}

	cout << number_of_clusters << endl;

	// Add remaining edges in order to get desired number of clusters	
	vector<long long> isolated_nodes, normal_clusters;
	for(int i = 0; i < max_organizations; i++){
		if(adjacent_edges[i] == 0){
			isolated_nodes.push_back(i);
		}

		if(adjacent_edges[i] > 0){
			normal_clusters.push_back(i);
		}
	}

	for(long long a : isolated_nodes){
		int ind_b = rand() % normal_clusters.size();
		long long b = normal_clusters[ind_b];
		if(root(a) != root(b)){
			cluster[root(a)] = root(b);
		}
	}
		

	vector<long long> cluster_ids;
	for(int i = 0; i < n; i++){
		cluster_ids.push_back(root(i));
	}

	output_clusters("k-spanning-tree-clustering/10/k_spanning_tree_10.txt", k, cluster_ids);

	return 0;
}