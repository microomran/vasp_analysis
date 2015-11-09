#!/usr/bin/env python
import sys
import os
if len(sys.argv)!=3:
   print("Usage: ExtractPositionFromOUTCAR.py OUTCAR- RefPOSCAR")

os.system("vasp2xyz1 %s > EPFO_tmp.xyz"%sys.argv[2] )
os.system("more 00/POSCAR |grep -e \"[T|F]\" | awk '{print $4}' > EPFO_TF")
tmpXYZ=open("EPFO_tmp.xyz","r")
natom=int(tmpXYZ.readline().strip())
atoms= [ ['Name', 0.0, 0.0, 0.0] for i in range(natom) ]

tmpXYZ.readline()

for i in range(natom):
    line = tmpXYZ.readline()
    words = line.split()
    atoms[i][0] = words[0]
    for j in range(3):
        atoms[i][j+1] = float(words[j+1])
tmpXYZ.close()




command="cat %s |grep \"POSITION\" -C %d|tail -%d|awk '{print $1,$2,$3}' > EPFO_pos"%(sys.argv[1],natom+1,natom)
os.system(command)
pos = open("EPFO_pos")
for i in range(natom):
   words = pos.readline().split()
   for j in range(3):
      atoms[i][j+1] = float(words[j])



tmpXYZ = open("EPFO_tmp.xyz","w")
tmpXYZ.write("%d\n"%natom)
tmpXYZ.write("\n")
for i in range(natom):
    tmpXYZ.write("%s %f %f %f\n"%(atoms[i][0],
                                atoms[i][1],
                                atoms[i][2],
                                atoms[i][3]))
tmpXYZ.close()


os.system("xyz2vasp EPFO_tmp.xyz %s > prePOSCAR"%sys.argv[2])
os.system("adjustTF prePOSCAR EPFO_TF > realPOSCAR")
os.system("poscar2contcar.sh realPOSCAR")
os.system("rm EPFO_tmp.xyz EPFO_TF EPFO_pos prePOSCAR realPOSCAR")

