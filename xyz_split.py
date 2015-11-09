#!/bin/env python

import sys
if(len(sys.argv)!=2):
    print("useage: python xyz_split.py [xyzfile_name]")
xyz_file_name = sys.argv[1]
xyz_file = open(xyz_file_name)

import os
try:
    dirname = xyz_file_name+".splitted"
    if os.path.isdir(dirname) == False:
        os.mkdir(xyz_file_name+".splitted")
except:
    print("Can't mkdir %s.splitted"%xyz_file_name)
    sys.exit()

counter = 0

while(True):
    line = xyz_file.readline()
    if len(line)==0:
        break

    segments = line.split()
    if len(segments)==0:
        continue
    

    counter += 1
    outputfile = None
    try:
        outputfilename = "%s.splitted/%d.xyz"%(xyz_file_name,counter)
        outputfile = open(outputfilename,"w")
    except:
        print("Can't write to file "+outputfilename)
        sys.exit()
        
        
    atom_count = int(segments[0])
        
    outputfile.write(line)
    line = xyz_file.readline()
    outputfile.write(line)
    
    for i in range(1, atom_count+1):
        line = xyz_file.readline()
        outputfile.write(line)
    

