import numpy as np
import math
import networkx as nx
import matplotlib.pyplot as plt
import tkinter as tk
from tkinter import * 
from tkinter.messagebox import showinfo
from tkinter import ttk
def appelant(adjacent, t_decouv, t_arriere, visited, parent, pa, vertex, V,time):
    for i in range(0,V):
        if visited[i] == False:
            DFS(adjacent, t_decouv, t_arriere, visited, parent, pa, i, V,time)
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
def structuriser(sommets,entrees,fenetre2):
    adjacent = np.zeros(shape=(len(sommets),len(sommets)))
    np.fill_diagonal(adjacent, 1)
    visited = np.array([False]*len(sommets))
    parent = np.array([None]*len(sommets))
    t_decouv = np.array([0]*len(sommets))
    t_arriere = np.array([math.inf]*len(sommets))
    p_articul = np.array([False]*len(sommets))
    for i in range(0,len(sommets)):
        adj = entrees[i].get().split()
        liste_sans = set(adj)
        adj = list(liste_sans)
        adj.sort()
        if False in np.in1d(adj,sommets):
            raise SommetInexistantException(adj)
        for a in adj:
            j = sommets.index(a)
            adjacent[i][j] = 1
            adjacent[j][i] = 1
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
        if (node in pnt) or (node == 'i') or (node == 'j') or (node == 'k'):
            color_map.append("#EC6C44")
        else:
            color_map.append("#75E6DA")
    print(visited)
    nx.draw(G,node_color=color_map, with_labels=True,edge_color="#0B6B88")
    print(adjacent)
    fenetre2.destroy()
    plt.show()
    
def recupere(entree,fenetre):
    fenetre.destroy()
    sommets = entree.split()
    liste_sans = set(sommets)
    sommets = list(liste_sans)
    sommets.sort()
    fenetre2 = tk.Tk()
    fenetre2.geometry("900x700")
    fenetre2.configure(bg = "#0B6B88")
    txt = "{str} {val} {warning}"
    container = ttk.Frame(fenetre2)
    canvas = tk.Canvas(container,width=770, height=690)
    scrollbar = ttk.Scrollbar(container, orient="vertical", command=canvas.yview)
    scrollable_frame = ttk.Frame(canvas,height = 900,width=600)
    scrollable_frame.bind(
        "<Configure>",
        lambda e: canvas.configure(
            scrollregion=canvas.bbox("all")
        )
    )
    canvas.create_window((0, 0), window=scrollable_frame, anchor="nw")
    canvas.configure(yscrollcommand=scrollbar.set)
    entrees = [0]*len(sommets)
    for i in range(0,len(sommets)):
        l = LabelFrame(scrollable_frame, text=txt.format(str = "Veuillez entrer la liste des sommets adjacents a",val = sommets[i],warning="[!] veuillez laisser un espace entre les noms de vos sommets [!] :" ), padx=10, pady=10)
        l.pack(fill="both", expand="yes")
        entrees[i] = tk.Entry(l)
        entrees[i].focus()
        entrees[i].pack()
    bouton2=Button(scrollable_frame, text="Valider",command = lambda:structuriser(sommets,entrees,fenetre2),bg="#EC6C44",fg="black")
    bouton2.pack()
    container.pack()
    canvas.pack(side="left", fill="both", expand=True)
    scrollbar.pack(side="right", fill="y")
    fenetre2.mainloop()
def start(openfen):
    openfen.destroy()
    fenetre = Tk()
    fenetre.title("Probleme des points d'articulation")
    fenetre.geometry("900x700")
    fenetre.configure(bg = "#EBF4F5")
    label = Label(fenetre, text="    ",bg="#EBF4F5")
    label.pack()
    l = LabelFrame(fenetre, text="Quels sont vos sommets ? [!] veuillez laisser un espace entre les noms de vos sommets [!]", padx=10, pady=10,bg="#EBF4F5",fg="#0B6B88")
    l.pack(fill="both", expand="yes")
    e1 = tk.Entry(l)
    e1.focus()
    e1.pack()
    bouton = Button(fenetre, text="Valider", command=lambda:recupere(e1.get(),fenetre),bg="#EC6C44")
    bouton.pack()
    fenetre.mainloop()
if __name__ == "__main__":
    j = -1
    openfen = Tk()
    openfen.title("Probleme des points d'articulation")
    openfen.geometry("900x700")
    openfen.configure(bg = "white")
    background_image=tk.PhotoImage(file="particul.gif",format="gif -index 0")
    background_label = tk.Label(openfen, image=background_image)
    background_label.pack()
    bouton_start = Button(openfen, text="Commencer",command=lambda:start(openfen),bg="#EC6C44")
    bouton_start.pack()
    openfen.mainloop()
    
