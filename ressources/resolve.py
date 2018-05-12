import numpy as py
from copy import deepcopy

def build_a(n):
    tab = [[0 for j in range(n)] for i in range(n)]
    return py.matrix(tab)

def build_b(n, rom, k):
    tab = [rom / (-k) for i in range(n)]
    return py.array(tab)

def resolve(tab, wall, rom, dist, k):
  py.set_printoptions(threshold='nan')
  tab_voisin = deepcopy(tab)
  n = 0
  for i in range(len(tab)):
    for j in range(len(tab[0])):
      if tab[i][j] == rom:
        tab_voisin[i][j] = n
        n += 1
  mat_a = build_a(n)
  mat_b = build_b(n, rom, k)
  for i in range(len(tab)):
    for j in range(len(tab[0])):
      if tab[i][j] == rom:
        cur = tab_voisin[i][j]
        mat_a[cur, cur] -= (-2 / (distx1 * distx2)) + (-2 / (disty1 * disty2))

        if tab[i - 1][j] == rom:
          mat_a[cur, tab_voisin[i-1][j]] += (disty1 * 2) / ((disty1 * disty2) * (disty1 + disty2))
        else:
          mat_b[cur] -= (2 * rom / disty2 * (disty1 + disty2))

        if tab[i + 1][j] == rom:
          mat_a[cur, tab_voisin[i+1][j]] += (disty1 * 2) / ((disty1 * disty2) * (disty1 + disty2))
        else:
          mat_b[cur] -= (2 * rom / disty2 * (disty1 + disty2))

        if tab[i][j - 1] == rom:
          mat_a[cur, tab_voisin[i][j-1]] += (distx1 * 2) / ((distx1 * distx2) * (distx1 + distx2))
        else:
          mat_b[cur] -= (2 * rom / distx2 * (distx1 + distx2))

        if tab[i][j + 1] == rom:
          mat_a[cur, tab_voisin[i][j+1]] += (distx1 * 2) / ((distx1 * distx2) * (distx1 + distx2))
        else:
          mat_b[cur] -= (2 * rom / distx2 * (distx1 + distx2))

  tmp = py.linalg.tensorsolve(mat_a, mat_b)
  for i in range(len(tab)):
    for j in range(len(tab[0])):
      if tab[i][j] == rom:
        tab[i][j] = tmp[0, tab_voisin[i][j]]
