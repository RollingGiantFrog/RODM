# using utf-8

import networkx as nx
import matplotlib.pyplot as plt

from community_propagation import *
from community_clustering import *
from community_connectance import *
from community_utils import *
  
K = 4
N = 400
p = 0.5
q = 0.01
G = randomGraph(N,K,p,q)


for k in range(1,5):
    plt.figure(k)
    plt.close()
    
#### Louvain ####
plt.figure(1)
partition,timeSpent,modularity,qualityFunction,splitJoinDistance = measureCommunityDetectionAlgorithm(G,N,K,louvain)
print("Louvain : " + str(timeSpent))
print("Modularity : " + str(modularity))
print("Quality function : " + str(qualityFunction))
print("Split join distance : " + str(splitJoinDistance))
print("")
plotCommunities(G,partition)
plt.show()

#### Clustering ####
plt.figure(2)
partition,timeSpent,modularity,qualityFunction,splitJoinDistance = measureCommunityDetectionAlgorithm(G,N,K,communityClustering)
print("Clustering : " + str(timeSpent))
print("Modularity : " + str(modularity))
print("Quality function : " + str(qualityFunction))
print("Split join distance : " + str(splitJoinDistance))
print("")
plotCommunities(G,partition)
plt.show()

#### Connectance ####
plt.figure(3)
partition,timeSpent,modularity,qualityFunction,splitJoinDistance = measureCommunityDetectionAlgorithm(G,N,K,communityConnectance)
print("Connectance : " + str(timeSpent))
print("Modularity : " + str(modularity))
print("Quality function : " + str(qualityFunction))
print("Split join distance : " + str(splitJoinDistance))
print("")
plotCommunities(G,partition)
plt.show()

#### Label propagation ####
plt.figure(4)
partition,timeSpent,modularity,qualityFunction,splitJoinDistance = measureCommunityDetectionAlgorithm(G,N,K,labelPropagation)
print("Label propagation : " + str(timeSpent))
print("Modularity : " + str(modularity))
print("Quality function : " + str(qualityFunction))
print("Split join distance : " + str(splitJoinDistance))
print("")
plotCommunities(G,partition)
plt.show()

