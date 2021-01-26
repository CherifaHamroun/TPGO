import numpy as np
import math
import networkx as nx
import matplotlib.pyplot as plt
def appelant(adjacent, t_decouv, t_arriere, visited, parent, pa, vertex, V,time):
    for i in range(0,V):
        if visited[i] == False:
            DFS(adjacent, t_decouv, t_arriere, visited, parent, pa, vertex, V,time)
def DFS(adjacent, t_decouv, t_arriere, visited, parent, pa, vertex, V,time):
    visited[vertex] = True
    t_decouv[vertex] = time+1
    t_arriere[vertex] = time +1
    child = 0
    for i in range(0,V):
        if adjacent.item((vertex,i)) == 1:
            if visited[i] == False:
                child = child + 1
                parent[i] = vertex
                DFS(adjacent, t_decouv, t_arriere, visited, parent, pa, i, V, time+1)
                t_arriere[vertex] = min(t_arriere[vertex], t_arriere[i])
                if parent[vertex] == None and child > 1:
                    pa[vertex] = True
                if parent[vertex] != None and t_arriere[i] >= t_decouv[vertex]:
                    pa[vertex] = True
            else:
                if parent[vertex] != i:
                    t_arriere[vertex] = min(t_arriere[vertex], t_decouv[i])
class SommetInexistantException(Exception):
    def __init__(self, adj, message="Sommets Inexistants dans le graphe"):
        self.adj = adj
        self.message = message
        super().__init__(self.message)

    def __str__(self):
        return f'{self.adj} -> {self.message}'

if __name__ == "__main__":
    j = -1
    print('Veuillez entrer la liste des sommets [!] veuillez laisser un espace entre les noms de vos sommets [!] : ')
    sommets = input().split()
    liste_sans = set(sommets)
    sommets = list(liste_sans)
    sommets.sort()
    adjacent = np.zeros(shape=(len(sommets),len(sommets)))
    np.fill_diagonal(adjacent, 1)
    visited = np.array([False]*len(sommets))
    parent = np.array([None]*len(sommets))
    t_decouv = np.array([0]*len(sommets))
    t_arriere = np.array([math.inf]*len(sommets))
    p_articul = np.array([False]*len(sommets))
    for i in range(0,len(sommets)):
        print('Veuillez entrer la liste des sommets adjacents a',sommets[i],'[!] veuillez laisser un espace entre les noms de vos sommets [!] : ')
        adj = input().split()
        print(adj)
        liste_sans = set(adj)
        adj = list(liste_sans)
        adj.sort()
        print(adj)
        if False in np.in1d(adj,sommets):
            raise SommetInexistantException(adj)
        for a in adj:
            j = sommets.index(a)
            adjacent[i][j] = 1
    G = nx.Graph()
    pnt = []
    color_map = []
    appelant(adjacent,t_decouv,t_arriere,visited,parent,p_articul,0,len(sommets),0)
    for k in range(0,len(p_articul)):
        if p_articul[k]:
            pnt.append(sommets[k])
    for i in range(adjacent.shape[0]):
        for j in range(adjacent.shape[1]):
            if adjacent[i][j] == 1:
                G.add_edge(sommets[i],sommets[j])
    for node in G:
        if node in pnt:
            color_map.append("green")
        else:
            color_map.append("blue")
    nx.draw(G,node_color=color_map, with_labels=True)
    plt.show()
