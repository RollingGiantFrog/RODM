# -*- coding: utf-8 -*-
"""
Created on Sun Apr 07 16:41:55 2019

@author: Victor
"""

import networkx as nx
import random as rd

def edgesPathWeight(G,remainingEdges=None,R=50):

    maxNode = list(G.nodes)[-1]
    for e in G.edges:
        G.edges[e]["weight"] = 0
    
#    Version aléatoire
#    nodes = list(G.nodes)
#    for i in range(R):
#        node = nodes[rd.randint(0,G.number_of_nodes()-1)]
       
#   Version non aléatoire
    for node in G.nodes:
        cursor = 0
        marked = [False for n in range(maxNode+1)]
        marked[node] = True
        fifo = [node]
        while cursor < len(fifo):
            u = fifo[cursor]
            cursor += 1
            neighbors = list(G.neighbors(u))
            rd.shuffle(neighbors)
            for v in neighbors:
                if not marked[v] and (remainingEdges == None or (u,v) in remainingEdges):
                    fifo += [v]
                    marked[v] = True
                    if G.edges[(u,v)].has_key("weight"):
                        G.edges[(u,v)]["weight"] += 1
                        G.edges[(v,u)]["weight"] += 1
                    else:
                        G.edges[(u,v)]["weight"] = 1
                        G.edges[(v,u)]["weight"] = 1
                    
    edges = []
    for node in G.nodes:
        edges += [(G.edges[(e[0],e[1])]["weight"],e) for e in G.edges(node) if e[1] > e[0] and (remainingEdges == None or e in remainingEdges)]
        
    edges.sort()
    
    return edges
    
def communityConnectance(G,alpha=0.5,K=2,R=50):
    remainingEdges = None
    
    for k in range(K):
        E = edgesPathWeight(G,remainingEdges,R)
        E = E[:int(len(E)*alpha)]
        
        remainingEdges = set()
        for w,e in E:
            remainingEdges.add(e)
            remainingEdges.add((e[1],e[0]))
            
    edges = []
    for e in remainingEdges:
        edges += [e]
    H = G.edge_subgraph(edges).copy()
    components = list(nx.connected_components(H))

    for k in range(len(components)):
        for node in components[k]:
            G.node[node]['lbl'] = k    
    
    return components