#!/usr/bin/python
import sys
if(len(sys.argv)!=5):
    print("centural_sym.py [.xyz] [around_x] [around_y] [around_z]");
    exit()

xyz_file = open(sys.argv[1],"r");
xyz_file_lines = xyz_file.readlines();
atom_count = int(xyz_file_lines[0].split()[0])
atom_types=[]
atom_xyzs=[]
for i in range(0,atom_count):
    words = xyz_file_lines[i+2].split();
    atom_types.append(words[0]);
    atom_xyzs.append([   float(words[1]), float(words[2]), float(words[3]) ])

origin = [ float(sys.argv[2]),float(sys.argv[3]),float(sys.argv[4])]

for i in range(0,atom_count):
    for j in range(0,3):
        atom_xyzs[i][j] = 2*origin[j] - atom_xyzs[i][j]

print(atom_count)
print("Prepared by centural_sym.py")
for i in range(0,atom_count):
    sys.stdout.write(atom_types[i]);
    for j in range(0,3):
        sys.stdout.write("  "+str(atom_xyzs[i][j]));
    sys.stdout.write("\n")

