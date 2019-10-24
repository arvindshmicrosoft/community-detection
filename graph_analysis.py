import collections
import matplotlib.pyplot as plt
import networkx as nx
from math import *

def degree_distribution(G, file_name):
	degree_sequence = sorted([d for n, d in G.degree(weight='weight')], reverse=True)  # degree sequence
	degree_count = collections.Counter(degree_sequence)
	deg, cnt = zip(*degree_count.items())

	fig, ax = plt.subplots()
	plt.plot(deg, cnt, "bo")

	plt.title("Weighted degree distribution")
	plt.ylabel("Count")
	plt.xlabel("Degree")
	plt.savefig(file_name)
	plt.show()


def edge_weight_distribution(G, file_name):
	weight_sequence = sorted([G[e[0]][e[1]][0]["weight"] for e in G.edges()], reverse=True)  # weight sequence
	if(len(weight_sequence) == 0):
		return

	weight_count = collections.Counter(weight_sequence)
	weight, cnt = zip(*weight_count.items())
	points = [(weight[i], cnt[i]) for i in range(len(weight))]
	points.reverse()

	fig, ax = plt.subplots()
	plt.plot(weight, cnt, "bo")

	plt.title("Edge Weight Distribution")
	plt.ylabel("Count")
	plt.xlabel("Weight")
	plt.savefig(file_name)
	plt.show()


def graph_visualization(G, file_name, cluster_file):
	f = open(cluster_file, "r")
	number_of_clusters = int(f.readline())
	cluster_ids = list(map(int, f.readline().split(" ")))
	f.close()

	cluster_size = dict()
	cluster_set = set()
	max_node = 0
	for x in cluster_ids:
		cluster_set.add(x)
		if(not x in cluster_size):
			cluster_size[x] = 0

		cluster_size[x] = cluster_size[x] + 1
		max_node = max(max_node, cluster_size[x])

	cluster_set = sorted(list(cluster_set))

	max_weight = 2
	if(nx.number_of_edges(G) != 0):
		max_weight = max([G[e[0]][e[1]][0]["weight"] for e in G.edges()])

	node_sizes = [(log(cluster_size[x]) + 1) / log(max_node) * 1000 for x in cluster_set]
	edge_sizes = [(log(G[e[0]][e[1]][0]["weight"]) + 1) / log(max_weight) * 5 for e in G.edges()]

	cluster_size_str = dict()
	for x in cluster_size.keys():
		cluster_size_str[str(x)] = cluster_size[x]

	nx.draw(G, pos = nx.circular_layout(G), node_size=node_sizes, width=edge_sizes, labels=cluster_size_str)
	plt.savefig(file_name)
	plt.show()


def general_info(G, file_name, cluster_file):
	f = open(file_name, "w")
	f.write("Nodes: " + str(nx.number_of_nodes(G)) + "\n")
	f.write("Edges: " + str(nx.number_of_edges(G)) + "\n")
	f.write("\n")

	weighted_degrees = [d for _, d in G.degree(weight="weight")]
	average_weighted_degree = sum(weighted_degrees) / len(weighted_degrees)
	min_weighted_degree = min(weighted_degrees)
	max_weighted_degree = max(weighted_degrees)
	f.write("Average weighted degree: " + str(average_weighted_degree) + "\n")
	f.write("Minimum weighted degree: " + str(min_weighted_degree) + "\n")
	f.write("Maximum weighted degree: " + str(max_weighted_degree) + "\n")
	f.write("\n")

	degrees = [d for _, d in G.degree()]
	average_degree = sum(degrees) / len(degrees)
	min_degree = min(degrees)
	max_degree = max(degrees)
	f.write("Average degree: " + str(average_degree) + "\n")
	f.write("Minimum degree: " + str(min_degree) + "\n")
	f.write("Maximum degree: " + str(max_degree) + "\n")
	f.write("\n")

	if(cluster_file != None):
		f_read = open(cluster_file, "r")
		number_of_clusters = int(f_read.readline())
		cluster_ids = list(map(int, f_read.readline().split(" ")))
		f_read.close()

		cluster_size = dict()
		for x in cluster_ids:
			if (not x in cluster_size):
				cluster_size[x] = 0

			cluster_size[x] = cluster_size[x] + 1

		cluster_sizes = [cluster_size[x] for x in cluster_size.keys()]
		average_cluster_size = sum(cluster_sizes) / len(cluster_sizes)
		min_cluster_size = min(cluster_sizes)
		max_cluster_size = max(cluster_sizes)
		f.write("Average cluster size: " + str(average_cluster_size) + "\n")
		f.write("Minimum cluster size: " + str(min_cluster_size) + "\n")
		f.write("Maximum cluster size: " + str(max_cluster_size) + "\n")




folder = "spectral-clustering/10"
name = "merged_graph_10.net"
cluster_file = "spectral_clusters_10.txt"
output_file = name[:-4] + "_analysis.txt"
weight_distribution_file = name[:-4] + "_edge_weights.png"
degree_distribution_file = name[:-4] + "_degrees.png"
graph_visualization_file = name[:-4] + "_visualization.png"


G = nx.Graph()
G = nx.read_pajek(folder + "/" + name)

general_info(G, folder + "/" + output_file, folder + "/" + cluster_file)
degree_distribution(G, folder + "/" + degree_distribution_file)
edge_weight_distribution(G, folder + "/" + weight_distribution_file)
graph_visualization(G, folder + "/" + graph_visualization_file, folder + "/" + cluster_file)