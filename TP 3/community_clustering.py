# -*- coding: utf-8 -*-
"""
Created on Sun Apr 07 16:41:55 2019

@author: Victor
"""

import networkx as nx
from math import *
import numpy as np

class Cluster:
    def __init__(self,nodes,out_edges,in_edges_count, neighbors):
        self.nodes = nodes
        self.out_edges = out_edges
        self.in_edges_count = in_edges_count
        self.neighbors = neighbors
        
    # Cette fonction renvoie une mesure de similarite de deux clusters.
    # S'ils sont tous deux des sommets isoles, cela renvoie 0 s'ils ne sont pas voisins, et le rapport du
    # nombre de voisins communs aux sommets sur le nombre total de voisins sinon.
    # Si un des deux clusters n'est pas un sommet, alors ce la renvoie le nombre d'aretes sortant des clusters communes
    # divise par le produit du nombre total d'aretes sortantes et du nombre total d'aretes internes aux clusters
    def similarity(self,cluster):
        inter = self.out_edges.intersection(cluster.out_edges)
        union = self.out_edges.union(cluster.out_edges)
        if len(cluster.nodes) == 1 and len(self.nodes) == 1:
            commonNeighbors = self.neighbors.intersection(cluster.neighbors)
            totalNeighbors = self.neighbors.union(cluster.neighbors)
            if (len(inter) >= 1) :
                return (1. + len(commonNeighbors))/(len(totalNeighbors))
            else :
                return 0.
        return (1.0*len(inter))/(1.0*len(union)*(1 + self.in_edges_count + cluster.in_edges_count))
        
    # fusion de deux clusters en un seul
    def merge(self,cluster):
        nodes = self.nodes.union(cluster.nodes)
        out_edges = self.out_edges.symmetric_difference(cluster.out_edges)
        in_edges_count = self.in_edges_count + cluster.in_edges_count + len(self.out_edges.intersection(cluster.out_edges))
        neighbors = (self.neighbors.union(cluster.neighbors)).difference(nodes)
        return Cluster(nodes, out_edges, in_edges_count, neighbors)

# Nous disposons d'un dictionnaire relationship qui contient les cluster voisins de chaque cluster
# (ie s'ils sont relies par au moins une arete)
# Nous disposons aussi d'une liste de clusters
# Lorsqu'on fusionne deuxclusters representes ici par leurs indices idx1 et idx2 dans la liste clusters
# la fonction mergeRelationships met a jour le dictionnaire relationship et la liste clusters
def mergeRelationships(relationship,clusters, idx1,idx2):
    cluster1 = clusters[idx1]
    cluster2 = clusters[idx2]
    mergedCluster = cluster1.merge(cluster2)
    relationship[mergedCluster] = {}
    if len(clusters) - 1 != idx2 :
        clusters[idx1] = clusters[-1]
    else :
        clusters[idx1] = clusters[-2]
    if len(clusters) - 2 != idx1 :
        clusters[idx2] = clusters[-2]
    clusters[len(clusters) - 2] = mergedCluster
    clusters.pop()
    
    for cluster in relationship[cluster1].keys():
        if (cluster != cluster2):
            del relationship[cluster][cluster1]
            sim = mergedCluster.similarity(cluster)
            relationship[cluster][mergedCluster] = sim
            relationship[mergedCluster][cluster] = sim
            
    for cluster in relationship[cluster2].keys():
        if (cluster != cluster1):
            del relationship[cluster][cluster2]
            if (not mergedCluster in relationship[cluster]):
                sim = mergedCluster.similarity(cluster)
                relationship[cluster][mergedCluster] = sim
                relationship[mergedCluster][cluster] = sim

    del relationship[cluster1]
    del relationship[cluster2]
    
    
# Trois fonctions utilisees pour detecter automatiquement le nombre de clusters
def gaussian_smooth(l, n = 10, sigma = 1):
    gaussian = [exp(-(1.0*i/sigma)**2) for i in range(-n,n+1)];
    lSmooth = []
    lengthL = len(l)
    for i in range(lengthL):
        val = 0.
        partialNorm = 0.
        for j in range(2*n+1):
            if 0 <= (i-j+n) and (i-j+n) < lengthL:
                val += gaussian[j]* l[i-j+n]
                partialNorm += gaussian[j]
        val /= partialNorm
        lSmooth += [val]
    return lSmooth

def second_derivative(l):
    lengthL = len(l)
    d = [0]
    for i in range(lengthL-2):
        d += [l[i+2] - 2* l[i+1] + l[i]]
    d += [0]
    return d

def rapport(l):
    lengthL = len(l)
    d = [0]
    for i in range(1,lengthL):
        d += [l[i-1]/l[i]]
    return d

# Notre algorithme
def communityClustering(G, maxNbClusters = 15):
    relationship = {}
    clusters = []
    #clusters initiaux pour chaque noeud
    for node in range(G.number_of_nodes()):
        out_edges = set()
        for nbr in G.neighbors(node):
            node1 = min(node,nbr)
            node2 = max(node,nbr)
            out_edges.add((node1,node2))
        cluster = Cluster(set([node]),out_edges,0,set(G.neighbors(node)))
        clusters += [cluster]
        relationship[cluster] = {}
        
    for node in range(G.number_of_nodes()):
        for nbr in G.neighbors(node):
            if (node < nbr):
                sim = clusters[node].similarity(clusters[nbr])
                relationship[clusters[node]][clusters[nbr]] = sim
                relationship[clusters[nbr]][clusters[node]] = sim
                
    mergingHistory = []
    simHistory = []
    # tant qu'il y a plusieurs clusters: recherche des deux clusters les plus proches,
    # puis fusion de ces clusters.
    while len(clusters) > 1:
        simMax = -1.
        idxMax1 = None
        idxMax2 = None
        length = len(clusters)
        for idx1 in range(length-1):
            c1 = clusters[idx1]
            for idx2 in range(idx1+1,length):
                c2 = clusters[idx2]
                if (c2 in relationship[c1]):
                    sim = relationship[c1][c2]
                    if (sim > simMax):
                        simMax = sim
                        idxMax1 = idx1
                        idxMax2 = idx2
        mergeRelationships(relationship,clusters, idxMax1,idxMax2)
        mergingHistory += [(idxMax1,idxMax2)]
        simHistory += [simMax]
        
    # detection du nombre de cluster comme le moment ou la valeur de la similitude varie le plus 
    measure = rapport(gaussian_smooth(simHistory))
    measure = [measure[len(measure) - 1 - i] for i in range(maxNbClusters)]
    nbClusters = np.argmax(measure) + 2
    
    #reconstitution des clusters a partir de l'historique
    clusters = [[i] for i in range(G.number_of_nodes())]
    for i in range(G.number_of_nodes() - nbClusters):
        (idx1,idx2) = mergingHistory[i]
        mergedCluster = clusters[idx1] + clusters[idx2]
        
        if len(clusters) - 1 != idx2 :
            clusters[idx1] = clusters[-1]
        else :
            clusters[idx1] = clusters[-2]
        if len(clusters) - 2 != idx1 :
            clusters[idx2] = clusters[-2]
        clusters[len(clusters) - 2] = mergedCluster
        clusters.pop()
        
        for idx in range(len(clusters)):
            for node in clusters[idx]:
                G.node[node]['lbl'] = idx
        
    return clusters