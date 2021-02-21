import numpy as np
from sys import maxsize 
from itertools import permutations
import time
import networkx as nx
import matplotlib.pyplot as plt
import tkinter as tk
from tkinter import * 
from tkinter.messagebox import showinfo
from tkinter import ttk
def PVC_naive(graph,racine):
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
    node1 = 0
    noeuds = []
    for node in ch:
        noeuds.append((node1,node))
        node1 = node
    noeuds.append((node1,0))
    return [min_path,noeuds]

def PVC_kruskal(graph,racine):
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
    cout = 0
    aretes = []
    for elt in final :
        cout = cout + elt['cout']
        aretes.append((elt['i'],elt['j']))
    return([cout,aretes])
def dessin(res1,couts,methode,n,temps):
    G = nx.Graph()
    color_map = []
    plt.figure(n)
    for i in range(couts.shape[0]):
        for j in range(couts.shape[1]):
            if i != j:
                if ((i,j) in res1[1]) or ((j,i) in res1[1]):
                    G.add_edge(str(i),str(j),color='r',weight=couts[i][j])
                else:
                    G.add_edge(str(i),str(j),color='b',weight=couts[i][j])
    edges = G.edges()
    colors = [G[u][v]['color'] for u,v in edges]
    pos=nx.spring_layout(G)
    labels = nx.get_edge_attributes(G,'weight')
    nx.draw_networkx_edge_labels(G,pos,edge_labels=labels)
    ax = plt.gca()
    txt = "Methode : {str} , Cout : {val} \n Temps d'execution : {temps} secondes "
    ax.set_title(txt.format(str =methode ,val=res1[0], temps = temps))
    nx.draw(G, with_labels=True,edge_color=colors,ax=ax)
    _ = ax.axis('off')

def structuriser(entrees,fenetre2):
    couts = np.zeros(shape=(len(entrees),len(entrees)))
    np.fill_diagonal(couts, 0)
    for i in range(0,len(entrees)):
        c = entrees[i].get().split()
        for j in range(0,len(c)):
            couts[i][j] = int(c[j])
            couts[j][i] = int(c[j])
    debut = time.time()
    res1 = PVC_naive(couts,0)
    temps_naive = time.time() - debut
    debut = time.time()
    res2 = PVC_kruskal(couts,0)
    temps_kruskal = time.time() - debut
    dessin(res1,couts,"naive",1,temps_naive)
    dessin(res2,couts,"heuristique",2,temps_kruskal)
    plt.show()
    fenetre2.destroy()
def recupere(entree,fenetre):
    fenetre.destroy()
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
    entrees = [0]*entree
    for i in range(0,entree):
        l = LabelFrame(scrollable_frame, text=txt.format(str = "Veuillez entrer la liste des couts des villes adjacentes a la ville ",val = i ,warning="[!] veuillez laisser un espace entre les noms de vos sommets [!] :" ), padx=10, pady=10)
        l.pack(fill="both", expand="yes")
        entrees[i] = tk.Entry(l)
        entrees[i].focus()
        entrees[i].pack()
    bouton2=Button(scrollable_frame, text="Valider",command = lambda:structuriser(entrees,fenetre2),bg="#EC6C44",fg="black")
    bouton2.pack()
    container.pack()
    canvas.pack(side="left", fill="both", expand=True)
    scrollbar.pack(side="right", fill="y")
    fenetre2.mainloop()
def start(openfen):
    openfen.destroy()
    fenetre = Tk()
    fenetre.title("Probleme du voyageur de commerce")
    fenetre.geometry("900x700")
    fenetre.configure(bg = "#EBF4F5")
    label = Label(fenetre, text="    ",bg="#EBF4F5")
    label.pack()
    l = LabelFrame(fenetre, text="Quel est le nombre de vos sommets", padx=10, pady=10,bg="#EBF4F5",fg="#0B6B88")
    l.pack(fill="both", expand="yes")
    value = IntVar() 
    value.set("0")
    e1 = Spinbox(l, from_=0, to=100)
    e1.pack()
    bouton = Button(fenetre, text="Valider", command=lambda:recupere(int(e1.get()),fenetre),bg="#EC6C44")
    bouton.pack()
    fenetre.mainloop()
if __name__ == "__main__": 
    j = -1
    openfen = Tk()
    openfen.title("Probleme du voyageur de commerce")
    openfen.geometry("900x700")
    openfen.configure(bg = "white")
    background_image=tk.PhotoImage(file="particul.gif",format="gif -index 0")
    background_label = tk.Label(openfen, image=background_image)
    background_label.pack()
    bouton_start = Button(openfen, text="Commencer",command=lambda:start(openfen),bg="#EC6C44")
    bouton_start.pack()
    openfen.mainloop()