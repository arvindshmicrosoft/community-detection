from sklearn.cluster import SpectralClustering
import matplotlib.pyplot as plt
import networkx as nx
import numpy as np

G = nx.read_pajek("data/graph.net")
print("data read")

S = nx.to_scipy_sparse_matrix(G)
print("adjacency matrix created")

clustering = SpectralClustering(n_clusters=50, assign_labels="discretize", affinity="precomputed").fit(S)
print(clustering.labels_)

f = open("data/spectral_clusters.txt", "w+")
for x in clustering.labels_:
    f.write(str(x) + " ")

f.close()