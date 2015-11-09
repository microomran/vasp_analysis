#!/usr/bin/env python
import sys;
from  Gibbs import *;
#print(sys.argv)
if(len(sys.argv)!=4 and len(sys.argv)!=5 ):
    print("Usage: python GibbsFreeEnergy.py OSZICAR-(from relaxation) OUTCAR-(from vibration)) T(in Kelvin)")
    print("Number_Of_Modes_not_included in ZPVE(0,1,or 2.., if this argument is not provided, skip all negative modes)")
    exit()

E0,modes,negModeCount = ReadFiles(sys.argv[1],sys.argv[2]);


modes_to_skip = 0
if(len(sys.argv) == 5):
    modes_to_skip = abs(int(sys.argv[4]))
else:
    modes_to_skip = negModeCount

g = CalculateGibbs(E0, modes, modes_to_skip, float(sys.argv[3]))
print(g.Show())
