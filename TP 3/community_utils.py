# -*- coding: utf-8 -*-
"""
Created on Sun Apr 07 17:09:23 2019

@author: Victor
"""

import networkx as nx
import random as rd
import time


import community_louvain
def louvain(G):
    communities = community_louvain.best_partition(G)
    for node,label in communities.iteritems():
        G.node[node]['lbl'] = label
    
    partition = {}
    for node in G.nodes:
        lbl = communities[node]
        if partition.has_key(lbl):
            partition[lbl] += [node]
        else:
            partition[lbl] = [node]
    
    return partition.values()    
    
    return partition


labelsColor = ['red','blue','green','yellow'] + [(rd.random(),rd.random(),rd.random()) for k in range(10000)]

def plotCommunities(G,partition):
    pos=nx.spring_layout(G) # positions for all nodes
    for k in range(len(partition)):
    	# nodes
    	nx.draw_networkx_nodes(G,pos,
    	                       nodelist=partition[k],
    	                       node_color=labelsColor[k],
    	                       node_size=50,
    	                   alpha=1)

    nx.draw_networkx_edges(G,pos,width=1.0,alpha=0.5)

def qualityFunction(G,partition):
    f = 0
    for community in partition:
        I1 = 0
        I2 = 0
        for node in community:
            for nbr in G.neighbors(node):
                if (nbr > node):
                    if (node,nbr) in G.edges() and nbr in community:
                        I2 += 1
                    else:
                        I1 += 1
        
        f += I2/float(I1+I2)
    
    return f/len(partition)
    
def modularity(G,partition):
    Q = 0
    for k in range(len(partition)):
        d = 0
        for node in partition[k]:
            d += G.degree(node)
            
        m = 0
        for node in partition[k]:
            for nbr in G.neighbors(node):
                if (nbr > node):
                    if (node,nbr) in G.edges() and nbr in partition[k]:
                        m += 1
        
        Q += (m - d*d/(4.*G.number_of_edges()))/float(G.number_of_edges())
    
    return Q
            
def splitJoinDistance(G,partition,realPartition):
    partitionSet = []
    for community in partition:
        S = set()
        for u in community:
            S.add(u)
        partitionSet += [S]
    
    realPartitionSet = []
    for community in realPartition:
        S = set()
        for u in community:
            S.add(u)
        realPartitionSet += [S]
    
    totalOverlap = 0
    for community in partitionSet:
        maxOverlap = 0
        for realCommunity in realPartitionSet:
            overlap = len(community.intersection(realCommunity))
            if overlap > maxOverlap:
                maxOverlap = overlap
        
        totalOverlap += maxOverlap
    
    for realCommunity in realPartitionSet:
        maxOverlap = 0
        for community in partitionSet:
            overlap = len(realCommunity.intersection(community))
            if overlap > maxOverlap:
                maxOverlap = overlap
    
        totalOverlap += maxOverlap
    
    return 1. - totalOverlap/(2.*G.number_of_nodes())
        
    
def measureCommunityDetectionAlgorithm(G,N,K,algorithm):
    t = time.clock()
    partition = algorithm(G)
    timeSpent = time.clock()-t
    
    realPartition = []
    for k in range(K):
        realPartition += [range(k*N,(k+1)*N)]
        
    return partition,timeSpent,modularity(G,partition),qualityFunction(G,partition),splitJoinDistance(G,partition,realPartition)
    
    
# generation d'un graphe, avec K clusters de N noeuds, et les probas p et q qu'il y ait une arete entre deux noeuds,
# respectivement d'un meme cluster et de deux clusters differents
def randomGraph(N,K,p,q):
	G = nx.Graph()
	G.add_nodes_from([i for i in range(N*K)])

	for k in range(K):
		for i in range(N*k,N*(k+1)):
			for j in range(i+1,N*(k+1)):
				if rd.random() <= p:
					G.add_edge(i,j)

	for k1 in range(K):
		for k2 in range(k1+1,K):
			for i in range(N*k1,N*(k1+1)):
				for j in range(N*k2,N*(k2+1)):
					if rd.random() <= q:
						G.add_edge(i,j)

	return G