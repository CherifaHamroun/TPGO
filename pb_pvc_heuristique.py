import numpy as np
from sys import maxsize 
from itertools import permutations
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
    graph = np.array([[0,1,2,100,200],
                    [1,0,50,10,2],
                    [2,50,0,3,30],
                    [100,10,3,0,1],
                    [100,2,30,1,0]
                    ])
    print(PVC(graph,0))