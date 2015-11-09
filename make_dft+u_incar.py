#!/usr/bin/env python
#LDAU = .TRUE.
#LDAUTYPE = 2  ! Dudarev's Approach
#LDAUL = 2 -1
#LDAUU = 4.0 0
#LDAUJ = 1 0
import sys;
if len(sys.argv) < 4 or len(sys.argv)%2 != 0:
    print("Usage: python make_dft+u_incar.py POSCAR ele1 U_for_ele_1 [ele2 U_for_ele_2,...]");
    exit()

element_wU = [];
element_Us = [];

for i in range(2,len(sys.argv)):
    if(i%2==0):
        element_wU.append(sys.argv[i]);
        element_Us.append(sys.argv[i+1]);

LDAULline = "LDAUL = ";
LDAUUline = "LDAUU = ";
LDAUJline = "LDAUJ = ";

try:
    poscar = open(sys.argv[1],'r');
    elements = poscar.readline().strip().split();
    for i in elements:
        if i in element_wU:
            pos = element_wU.index(i);
            LDAULline += str("2 ");
            LDAUUline += str(element_Us[pos]+" ");
        else:
            LDAULline += str("-1 ");
            LDAUUline += str("0.0 ");
        LDAUJline += "0.0 ";
except IOError:
    print("Can't open poscar file "+sys.argv[1]);
    exit();

print("LDAU = .TRUE.");
print("LDAUTYPE = 2  ! Dudarev's Approach");
print(LDAULline);
print(LDAUUline);
print(LDAUJline);
