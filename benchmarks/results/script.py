#!/usr/bin/python
# coding: utf-8

import numpy as np
import matplotlib.pyplot as plt
import copy
import sys

lis = [
    "bst_double_double_b.txt",
    "umap_double_double.txt",
    "bst_double_double_u.txt",
    "map_double_double.txt",
    "bst_int_int_b.txt",
    "map_int_int.txt",
    "bst_int_int_u.txt",
    "umap_int_int.txt",
    "bst_char_char_b.txt",
    "map_char_char.txt",
    "bst_char_char_u.txt",
    "umap_char_char.txt",
]

data = np.zeros((50, 12))
for i, item in enumerate(lis):
    data[:, i] = np.loadtxt(item)
    # print(item+".txt")

data = np.array(data)
x = np.arange(1, 51)

x = 100000 * x

for i, item in enumerate(lis[:4]):
    plt.plot(x, (data[:, i]), label=item)

plt.xlabel("pairs(double, double)")
plt.ylabel("seconds")
plt.legend(loc="upper left")
plt.grid()
plt.savefig("./img/double_double.png", dpi=600, bbox_inches="tight", pad_inches=0.0)
plt.clf()

for i, item in enumerate(lis[4:8]):
    plt.plot(x, (data[:, i + 4]), label=item)

plt.xlabel("pairs(int, int)")
plt.ylabel("seconds")
plt.legend(loc="upper left")
plt.grid()
plt.savefig("./img/int_int.png", dpi=600, bbox_inches="tight", pad_inches=0.0)
plt.clf()

for i, item in enumerate(lis[8:12]):
    plt.plot(x, (data[:, i + 8]), label=item)

plt.xlabel("pairs(char, char)")
plt.ylabel("seconds")
plt.legend(loc="upper left")
plt.grid()
plt.savefig("./img/char_char.png", dpi=600, bbox_inches="tight", pad_inches=0.0)
