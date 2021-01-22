import numpy as np
import math
import tkinter 
def DFS(adjacent, visited, racine, vertex, V,cout):
    for i in range(0,V):
        if adjacent.item((vertex,i)) == 1:
            if adjacent[i] != racine:
                DFS(adjacent, visited, racine, i, V,cout)
            else:
                
if __name__ == "__main__":
