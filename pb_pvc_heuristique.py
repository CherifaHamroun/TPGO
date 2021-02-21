import numpy as np
from sys import maxsize 
from itertools import permutations
import time
def PVC(graph,racine):
    aretes = []
    element = { 
        "cout" : 0,
        "i" : 0,
        "j" : 0,
    }
    for i in range(graph.shape[0]):
       for j in range(i+1,graph.shape[1]):
            element = { 
                    "cout" : graph[i,j],
                    "i" : i,
                    "j" : j,
                }
            aretes.append(element)
    aretes = sorted(aretes, key = lambda i: i['cout'])
    degre = [0]*graph.shape[1]
    final = []
    k = 0
    l = 0
    stop = False
    while k < len(aretes) and not stop:
        if degre[aretes[k]["i"]]<2 and degre[aretes[k]["j"]]<2:
            degre[aretes[k]["i"]] = degre[aretes[k]["i"]] + 1
            degre[aretes[k]["j"]] = degre[aretes[k]["j"]] + 1
            final.append(aretes[k])
            l=l+1
            if l == graph.shape[1]:
                stop = True
        k = k+1
    return(final)

if __name__ == "__main__": 
    graph = np.array([
                    [0,2,3,5,7,8,9,10,10,10,10],
                    [2,0,8,11,13,1,12,22,22,22,22],
                    [3,8,0,20,13,100,10,22,22,22,22],
                    [5,11,20,0,10,1,16,11,11,11,11],
                    [7,13,13,10,0,6,6,6,6,6,6],
                    [8,1,100,1,6,0,2,1,1,1,1],
                    [9,12,10,16,6,2,0,3,3,3,3],
                    [10,22,22,11,6,1,3,0,11,11,11],
                    [10,22,22,11,6,1,3,11,0,4,4],
                    [10,22,22,11,6,1,3,11,4,0,5],
                    [10,22,22,11,6,1,3,11,4,5,0],
                    ])
    debut = time.time()
    print("DEBUT",debut)
    print(PVC(graph,0))
    fin = time.time()-debut
    print("FIN",fin)