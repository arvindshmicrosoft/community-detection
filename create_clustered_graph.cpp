#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <set>
#include <map>

using namespace std;

const int max_organizations = 27000;
const int min_weight = 0;

vector<vector<long long>> edges;
map<long long, map<long long, long long>> merged_graph, cluster_graph;
set<long long> merged_graph_nodes, cluster_graph_nodes;

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
		edges.push_back({w, a, b});
		
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


vector<vector<long long>> create_edge_list(map<long long, map<long long, long long>> edges){
	vector<vector<long long>> edge_list;
	for(auto it1 = edges.begin(); it1 != edges.end(); it1++){
		for(auto it2 = it1 -> second.begin(); it2 != it1 -> second.end(); it2++){
			edge_list.push_back({it1 -> first, it2 -> first, it2 -> second});
		}
	}

	return edge_list;
}


void output_data(string output_file, set<long long> nodes, vector<vector<long long>> edges){
	int number_of_nodes = nodes.size();
	int number_of_edges = edges.size();
	ofstream out(output_file);

	out << "*vertices " << number_of_nodes << endl;
	for(auto it = nodes.begin(); it != nodes.end(); it++){
		out << *it << " \"" << *it << "\"" << "\n";
	}

	out << "*edges " << number_of_edges << endl;
	for(int i = 0; i < edges.size(); i++){
		if(edges[i][2] > min_weight){
			out << edges[i][0] << " " << edges[i][1] << " " << edges[i][2] << endl; 
		}
	}
	out.close();
}


int main(){
	string folder = "k-spanning-tree-clustering/50";
	string name = "k_spanning_tree_50.txt";

	n = read_data("data/graph.net");
	k = read_clusters(folder + "/" + name);	

	cout << n << endl;

	for(int i = 0; i < n; i++){
		cluster_graph_nodes.insert(i);
	}

	for(int i = 0; i < n; i++){
		merged_graph_nodes.insert(cluster_ids[i]);
	}

	for(int i = 0; i < edges.size(); i++){
		long long w = edges[i][0];
		long long a = edges[i][1];
		long long b = edges[i][2];

		if(a > b){
			swap(a, b);
		}

		if(cluster_ids[a] == cluster_ids[b]){
			cluster_graph[a][b] += w;
		}

		if(cluster_ids[a] > cluster_ids[b]){
			swap(a, b);
		}

		if(cluster_ids[a] != cluster_ids[b]){
			merged_graph[cluster_ids[a]][cluster_ids[b]] += w;
		}
	}

	vector<vector<long long>> merged_graph_edges = create_edge_list(merged_graph);
	vector<vector<long long>> cluster_graph_edges = create_edge_list(cluster_graph);

	output_data(folder + "/cluster_graph_50.net", cluster_graph_nodes, cluster_graph_edges);
	output_data(folder + "/merged_graph_50.net", merged_graph_nodes, merged_graph_edges);

	return 0;
}