#!/bin/env python
import sys
if len(sys.argv)!=2:
    print "usage: python pods_spin_combine.py DOS#"
    exit(-1)

pdos_file = None
try:
    print(sys.argv[1])
    pdos_file = open(sys.argv[1],"r")
except:
    print "Can't open %s"%sys.argv[1]
    exit(-1)
    

energies=[]
dos=[]

for i in pdos_file:
    segments = None
    try:
        segments = [float(n) for n in i.split()]
        energies.append(segments[0])
        dos.append(segments[1:])
    except:
        pass

combined_dos=[]
for i in range(len(energies)):
    total = []
    for j in range(0,len(dos[i]),2):
        total.append(abs(dos[i][j])+abs(dos[i][j+1]))
    combined_dos.append(total)

for i in range(len(energies)):
    sys.stdout.write("%f  "%energies[i])
    for j in range(len(combined_dos[i])):
        sys.stdout.write("%f  "%combined_dos[i][j])
    sys.stdout.write("\n")



