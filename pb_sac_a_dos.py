import numpy as np
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
  while (i > 0 and j > 0):
    if(F[i,j] != F[i-1,j]):
      print("Objet :",i)
      j = j-items[i-1]['weight']
      i = i-1
    else:
      i = i-1
      
if __name__ == "__main__":
  print('Veuillez entrer le nombre d objets que vous souhaiter ajouter : ')
  n = int(input())
  print('Veuillez entrer le poids de votre sac a dos : ')
  w = int(input())
  items = []
  for i in range(1,n+1):
    item = {
      "weight":0,
      "value":0,
    }
    print('Veuillez entrer le poids de votre objet : ')
    item["weight"] = int(input())
    print('Veuillez entrer la valeur de votre objet : ')
    item["value"] = int(input())
    items.append(item)
F=np.zeros((n+1,w+1))
print("Valeur maximale :",knapsack_bottom_up(n,w,items))
print("La matrice des couts : ")
print(F)
print("Les objets pris")
items_in_optimal(n,w,items)