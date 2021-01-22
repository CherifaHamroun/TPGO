import numpy as np
import tkinter as tk
from tkinter import * 
from tkinter.messagebox import showinfo
from tkinter import ttk

n = 0
w = 0

def knapsack_bottom_up(F,n,w,items):
  for i in range(1,n+1):
    for j in range(1,w+1):
      if items[i-1]['weight'] > j:
        print(F,n,w)
        F[i,j]=F[i-1,j]
      else:
        print(F,n,w)
        F[i,j] = max(F[i-1,j-items[i-1]['weight']]+items[i-1]['value'],F[i-1,j])

  return F[n,w]

def items_in_optimal(F,n, w, items):
  i = n
  j = w
  opt =[]
  while (i > 0 and j > 0):
    if(F[i,j] != F[i-1,j]):
      opt.append(i)
      j = j-items[i-1]['weight']
      i = i-1
    else:
      i = i-1
  return opt

def result(valuemax,opt,l,poid):
    n = len(l)      #this is the length of the list l
    if n !=0:
        lngt = 600 // n #this is the dimension of the squares that I want
    else:
        lngt = 0
    fen = tk.Tk()
    fen.title("Probleme du sac a dos")

    fen.geometry("900x700")
    #I would like to create a table of 4 rows on canvas
    #each row should contain 4 squares
    can = tk.Canvas(fen, width=900, height=600, bg="lightblue")
    can.pack(side=tk.LEFT)
    can.create_text((250, 30), text="Votre jeu d'essai donne le resultat suivant ( les objets pris sont en vert ) :")
    for i in range(3):
        y = i * 50 + 50
        for j in range(0,n+1):
            x = j * lngt
            if j in opt:
                can.create_rectangle(x, y, x+lngt, y+50, fill="green",outline ="lightblue")
            else:
                can.create_rectangle(x, y, x+lngt, y+50, fill="lightcoral",outline ="lightblue")
            if j==0 and i == 0:
                can.create_text((x+50, y+20), text="Objet")
            elif j == 0 and i == 1:
                can.create_text((x+50, y+20), text="Valeur")
            elif j == 0 and i == 2:
                can.create_text((x+50, y+20), text="Poid")
            elif i == 0 :
                can.create_text((x+50, y+20), text= j)
            elif i == 1:
                can.create_text((x+50, y+20), text= l[j-1]["value"])
            else:
                can.create_text((x+50, y+20), text=l[j-1]["weight"])
    can.create_rectangle(0,y+60, 300, y+110, fill="green",outline ="lightblue")
    can.create_rectangle(300,y+60,600,y+110, fill="green",outline ="lightblue")
    txt = "{str} : {val:.2f}"
    can.create_text((100, y+90), text= txt.format(str = "Poid Maximal",val = poid))
    can.create_text((400, y+90), text= txt.format(str = "Valeur Maximale", val = valuemax))
    f = tk.Frame(fen, width=900, height=600, bg="lightblue")
    f.pack(side=tk.LEFT)
    fen.mainloop()

def structuriser(poids,valeur,n,p):
    items = []
    for j in range(1,n+1):
        item = {
        "weight":0,
        "value":0,
        }
        item["weight"] = int(poids[j-1].get())
        item["value"] = int(valeur[j-1].get())
        items.append(item)
    F = np.zeros((n+1,p+1))
    valuemax = knapsack_bottom_up(F,n,p,items)
    opt = items_in_optimal(F,n,p,items)
    result(valuemax,opt,items,p)
def recupere(n,p):
    poids=[0]*n
    valeur=[0]*n
    fenetre2 = Tk()
    fenetre2.title("Probleme du sac a dos")
    fenetre2.geometry("900x700")
    fenetre2.configure(bg = "lightblue")
    container = ttk.Frame(fenetre2)
    canvas = tk.Canvas(container,width=700, height=600)
    scrollbar = ttk.Scrollbar(container, orient="vertical", command=canvas.yview)
    scrollable_frame = ttk.Frame(canvas)
    scrollable_frame.bind(
        "<Configure>",
        lambda e: canvas.configure(
            scrollregion=canvas.bbox("all")
        )
    )
    canvas.create_window((0, 0), window=scrollable_frame, anchor="nw")

    canvas.configure(yscrollcommand=scrollbar.set)
    label = Label(scrollable_frame, text="Problème du sac a dos")
    label.pack()
    for i in range(0,n):
        l = LabelFrame(scrollable_frame, text="objet", padx=10, pady=10)
        l.pack(fill="both", expand="yes")
        label = Label(l, text="Veuillez entrer le poids de votre objet: ")
        label.pack()
        value = IntVar() 
        value.set("0")
        poids[i] = Spinbox(l, from_=0, to=100)
        poids[i].pack()
        label = Label(l, text="Veuillez entrer la valeur de votre objet: ")
        label.pack()
        value = IntVar() 
        value.set("0")
        valeur[i] = Spinbox(l, from_=0, to=100)
        valeur[i].pack()
    bouton2=Button(scrollable_frame, text="Envoyer", command= lambda: structuriser(poids,valeur,n,p))
    bouton2.pack()
    container.pack()
    canvas.pack(side="left", fill="both", expand=True)
    scrollbar.pack(side="right", fill="y")
    fenetre2.mainloop()
def start():
    fenetre = Tk()
    fenetre.title("Probleme du sac a dos")
    fenetre.geometry("900x700")
    fenetre.configure(bg = "lightblue")
    label = Label(fenetre, text="Problème du sac a dos",bg="lightblue")
    label.pack()
    l = LabelFrame(fenetre, text="Quel est le nombre de vos objets", padx=10, pady=10,bg="lightyellow")
    l.pack(fill="both", expand="yes")
    value = IntVar() 
    value.set("0")
    entree = Spinbox(l, from_=0, to=100)
    entree.pack()
    l2 = LabelFrame(fenetre, text="Quel est le poids maximal de votre sac à dos", padx=10, pady=10,bg="lightyellow")
    l2.pack(fill="both", expand="yes")
    value = IntVar() 
    value.set("0")
    entree2 = Spinbox(l2, from_=0, to=100)
    entree2.pack()
    bouton = Button(fenetre, text="Valider", command=lambda:recupere(int(entree.get()),int(entree2.get())),bg="lightyellow")
    bouton.pack()
    fenetre.mainloop()

if __name__ == '__main__':
    openfen = Tk()
    openfen.title("Probleme du sac a dos")
    openfen.geometry("900x700")
    openfen.configure(bg = "lightyellow")
    background_image=tk.PhotoImage(file="giphy.gif",format="gif -index 0")
    background_label = tk.Label(openfen, image=background_image)
    background_label.pack()
    bouton_start = Button(openfen, text="Commencer", command=lambda:[start(),openfen.destroy])
    bouton_start.pack()
    openfen.mainloop()
