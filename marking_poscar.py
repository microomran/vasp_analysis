#!/usr/bin/env python
import sys
if len(sys.argv)!= 2:
    print "Usage: marking_poscar.py [POSCAR]"
    exit(0)

poscar = None
try:
    poscar = open(sys.argv[1],"r")
except:
    print "Can't open %s"%sys.argv[1]
    exit(0)

line = poscar.readline()
atom_types = line.split()
print line.rstrip()
#print atom_types
for i in range(5):
    line = poscar.readline()
    print line.rstrip()

word = line.split()[0]
if word.isalpha():
    line = poscar.readline()
    print line.rstrip()

atom_counts = line.split()
for i,w in enumerate(atom_counts):
    atom_counts[i] = int(w)

#print atom_counts
elements = []
for i,ele in enumerate(atom_types):
    for j in range(atom_counts[i]):
        elements.append(ele)

for i in range(2):
    print poscar.readline().rstrip()

for i in range(len(elements)):
    line = poscar.readline().rstrip()
    print "%s   %d   %s"%(line,i+1,elements[i])

