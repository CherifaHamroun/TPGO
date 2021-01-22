import numpy as np
import tkinter as tk
import cv2 
n = 0
w = 0
F=np.zeros((n+1,w+1))
def knapsack_bottom_up(n,w,items):
  for i in range(1,n+1):
    for j in range(1,w+1):
      if items[i-1]['weight'] > j:
        F[i,j]=F[i-1,j]
      else:
          F[i,j] = max(F[i-1,j-items[i-1]['weight']]+items[i-1]['value'],F[i-1,j])
  return F[n,w]

def items_in_optimal(n, w, items):
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
      
if __name__ == "__main__":
  l = [
    {
      "weight":0,
      "value":1,
    },
    {
      "weight":0,
      "value":1,
    },
    {
      "weight":0,
      "value":9,
    },
    {
      "weight":0,
      "value":2,
    },
    {
      "weight":0,
      "value":0,
    },
    {
      "weight":0,
      "value":1,
    }
  ]
  F=np.zeros((7,5))
  valuemax = knapsack_bottom_up(6,4,l)
  opt = items_in_optimal(6,4,l)
  poid = 0
  n = len(l)      #this is the length of the list l
  lngt = 600 // n #this is the dimension of the squares that I want
  fen = tk.Tk()
  fen.title("Probleme du sac a dos")

  fen.geometry("900x600")
  #I would like to create a table of 4 rows on canvas
  #each row should contain 4 squares
  can = tk.Canvas(fen, width=900, height=600, bg="lightblue")
  can.pack(side=tk.LEFT)
  can.create_text((250, 30), text="Votre jeu d'essai donne le resultat suivant ( les objets pris sont en vert ) :")
  for i in range(3):
      y = i * 50 + 50
      for j in range(n+1):
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
  can.create_text((100, y+90), text= txt.format(str = "Poid Maximal",val = 4))
  can.create_text((400, y+90), text= txt.format(str = "Valeur Maximale", val = valuemax))
  f = tk.Frame(fen, width=900, height=600, bg="lightblue")
  f.pack(side=tk.LEFT)
  fen.mainloop()