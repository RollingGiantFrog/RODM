# -*- coding: utf-8 -*-
"""
Created on Sun Apr 07 16:46:19 2019

@author: Victor
"""

import networkx as nx
import random as rd

def labelPropagation(G):
    nodes = range(G.number_of_nodes())
    for node in nodes:
        G.node[node]['lbl'] = node
        
    # rd.shuffle implemente le fischer-yates shuffle
    rd.shuffle(nodes)
    
    for node in nodes:
        maxLblOccur = -1
        maxLbl = -1
        
        labelsOccur = {}
        
        for nbr in G.neighbors(node):
            nbrLbl = G.node[nbr]['lbl']
            if nbrLbl in labelsOccur:
                labelsOccur[nbrLbl] += 1
            else:
                labelsOccur[nbrLbl] = 1

            # calcul du max au fur et a mesure : etant donne que l'on shuffle les noeuds au debut, 
            # on estime que la selection parmis les labels de meme occurence max est aleatoire
            if labelsOccur[nbrLbl] > maxLblOccur:
                maxLbl = nbrLbl
                maxLblOccur = labelsOccur[nbrLbl]
                
                
        G.node[node]['lbl'] = maxLbl

    changed = True
    while changed:
        changed = False
        
        # rd.shuffle implemente le fischer-yates shuffle
        rd.shuffle(nodes)
        
        for node in nodes:
            maxLblOccur = -1
            maxLbl = -1
            
            labelsOccur = {}
            
            for nbr in G.neighbors(node):
                nbrLbl = G.node[nbr]['lbl']
                if nbrLbl in labelsOccur:
                    labelsOccur[nbrLbl] += 1
                else:
                    labelsOccur[nbrLbl] = 1
                    
                # calcul du max au fur et a mesure : etant donne que l'on shuffle les noeuds au debut, 
                # on estime que la selection parmis les labels de meme occurence max est aleatoire
                if labelsOccur[nbrLbl] > maxLblOccur:
                    maxLbl = nbrLbl
                    maxLblOccur = labelsOccur[nbrLbl]
                    
            if (not G.node[node]['lbl'] in labelsOccur or labelsOccur[G.node[node]['lbl']] < maxLblOccur):
                changed = True
                G.node[node]['lbl'] = maxLbl
                
    partition = {}
    for node in nodes:
        lbl = G.node[node]['lbl']
        if partition.has_key(lbl):
            partition[lbl] += [node]
        else:
            partition[lbl] = [node]
    
    return partition.values()