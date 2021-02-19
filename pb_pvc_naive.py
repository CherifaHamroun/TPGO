import numpy as np
from sys import maxsize 
from itertools import permutations
import time
def PVC(graph,racine):
    chemin = []
    for i in range(graph.shape[0]):
        if i != racine:
            chemin.append(i)
    min_path = maxsize
    ch = []
    for i in permutations(chemin):
        cout = 0
        k = racine
        for j in i: 
            cout += graph[k,j] 
            k = j 
        cout += graph[k,racine] 
        if min_path > cout:
            min_path = cout
            ch = i
    return [min_path,ch]
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