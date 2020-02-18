import numpy as np
import matplotlib.pyplot as plt
import copy
import sys

lis = ["bst_int_int","map_int_int","umap_int_int",
       "bst_double_double","map_double_double","umap_double_double",
       "bst_char_char","map_char_char","umap_char_char"]

#lis = ["bst_int_int"]
data = np.zeros((100,9))
for i, item in enumerate(lis):
       data[:,i] = np.loadtxt(item+".txt")
        #print(item+".txt")

data = np.array(data)
x = np.arange(1, 101)

x = 10000*x

for i, item in enumerate(lis[:3]):
    plt.plot(x, (data[:,i]), label=item)
    
plt.xlabel("pairs(int, int)")
plt.ylabel("seconds")
plt.legend(loc='upper left')
plt.grid()
plt.show()
plt.savefig('int_int.svg', dpi=300, bbox_inches='tight', pad_inches=0.0)
print("figure saved")

for i, item in enumerate(lis[3:6]):
    plt.plot(x, (data[:,i+3]), label=item)
    
plt.xlabel("pairs(int, int)")
plt.ylabel("seconds")
plt.legend(loc='upper left')
plt.grid()
plt.show()
plt.savefig('double_double.svg', dpi=300, bbox_inches='tight', pad_inches=0.0)
print("figure saved")

for i, item in enumerate(lis[6:9]):
    plt.plot(x, (data[:,i+6]), label=item)
    
plt.xlabel("pairs(int, int)")
plt.ylabel("seconds")
plt.legend(loc='upper left')
plt.grid()
plt.show()
plt.savefig('char_char.svg', dpi=300, bbox_inches='tight', pad_inches=0.0)
print("figure saved")
