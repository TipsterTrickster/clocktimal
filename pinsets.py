import itertools
import numpy as np
from numpy.linalg import inv
import struct


# This is a matrix for every clock move. The columns are:
# UR DR DL UL U R D L ur dr dl ul / \ ALL (then a y2 and the same order on the back))
# The 14 rows are for the 14 dials on the clock. So each column represents what dials are affected by that move.
A = [
    [0,1,0,1,0,1,1,0,1,0,0,1,0,1,1,0,1,0,1,0,1,0,0,1,0,1,1,0,1,1],
    [1,0,0,0,0,0,1,0,1,0,1,0,0,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0],
    [1,0,0,1,0,1,0,1,1,0,1,0,0,1,0,1,0,1,1,0,1,0,1,0,1,0,0,1,1,1],
    [0,0,0,0,1,0,1,0,1,0,0,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0],
    [1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0],
    [1,0,1,0,0,0,0,0,1,0,1,0,1,0,0,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0],
    [0,1,0,1,1,0,0,1,0,1,0,1,1,0,1,0,1,0,1,0,0,1,1,0,1,0,0,1,1,1],
    [0,0,1,0,1,0,0,0,0,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0],
    [0,1,1,0,0,1,0,1,0,1,1,0,1,0,0,1,1,0,0,1,1,0,1,0,0,1,1,0,1,1],
    [0,-1,0,-1,0,-1,0,-1,0,0,0,-1,0,-1,0,-1,0,-1,0,0,0,0,0,-1,0,-1,0,-1,0,-1],
    [0,-1,0,-1,0,-1,0,-1,0,-1,0,0,0,-1,0,-1,0,-1,0,-1,0,0,0,0,0,-1,0,-1,0,-1],
    [0,-1,0,-1,0,-1,0,-1,0,-1,0,-1,0,-1,0,-1,0,-1,0,-1,0,-1,0,-1,0,-1,0,-1,0,-1],
    [0,-1,0,-1,0,-1,0,-1,0,-1,0,-1,0,-1,0,0,0,0,0,0,0,-1,0,-1,0,-1,0,-1,0,-1],
    [0,-1,0,-1,0,-1,0,-1,0,-1,0,-1,0,0,0,-1,0,0,0,-1,0,-1,0,0,0,-1,0,-1,0,-1]
]

moveNames=["URf","URb","DRf","DRb","DLf","DLb","ULf","ULb","Uf","Ub","Rf","Rb","Df","Db","Lf","Lb",
           "urf","urb","drf","drb","dlf","dlb","ulf","ulb","/f","/b","\\f","\\b","ALLf","ALLb"]


# Checks if the matrix is invertible. If so the set can be used to solve the clock.
def is_invertible(matrix):
    global inverse
    if np.linalg.det(matrix) != 0:
        inverse = inv(matrix)


        if np.any(np.abs(inverse - np.round(inverse)) > 1e-8) or np.any(inverse > 10000):
            return False
        
        inverse = np.around(inverse, 0)
        inverse = inverse.astype(int)
        return True
    else:
        return False


combinations = itertools.combinations(list(range(30)), 14)
counter=0
invertibleCounter=0

all_rows = []
all_pinsets = []

with open("pinsets.bin", "w") as f:
    pass
with open("rows.bin", "w") as f2:
    pass

# for c in combinations:
for c in combinations:
    
    counter += 1
    # makes a 14x14 matrix containing the columns of the big matrix above whose indices are in the list i.
    B = [[row[x] for x in c] for row in A]
    B = np.array(B)

    if is_invertible(B):
        invertibleCounter+=1

        all_pinsets.append(c)

        for row in inverse:
            all_rows.append(row.tolist())

    if counter % 1000000 == 0:
        print(counter,invertibleCounter)

        with open("pinsets.bin", "ab") as f:
            for pinset in all_pinsets:
                f.write(struct.pack("14i", *pinset))  # Packing each 14-element tuple


        with open("rows.bin", "ab") as f2:
            for row in all_rows:
                f2.write(struct.pack("14i", *row))  # Packing each 14-element list


        all_rows = []
        all_pinsets = []



with open("pinsets.bin", "ab") as f:
    for pinset in all_pinsets:
        f.write(struct.pack("14i", *pinset))  # Packing each 14-element tuple

with open("rows.bin", "ab") as f2:
    for row in all_rows:
        f2.write(struct.pack("14i", *row))  # Packing each 14-element list


print(counter, invertibleCounter)
