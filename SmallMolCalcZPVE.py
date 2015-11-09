from math import *;
#constants# :
PLANCK_CONSTANT = 6.6260693e-34 #J s
NA = 6.0221415e+23 # J/ mol K
SPEED_OF_LIGHT = 2.99792458e10 # cm/s
BOLTZMAN_CONSTANT = 1.3806505e-23 # J/K
IDEAL_GAS_CONSTANT = NA*BOLTZMAN_CONSTANT
EV_TO_KJ = 96.48


def ReadFiles(oszicarName,outcarName):
    E0 = 0.0;
    vibModes = [];
    countNegativeModes = 0;

    try:
        oszicar = open(oszicarName);
        lastLine = oszicar.readlines()[-1];
        E0 = (float)(lastLine.split()[4])
#        print(E0);
        oszicar.close();
    except IOError:
        print("Can't open OSZICAR file "+sys.argv[1])
        exit()

    try:
        outcar = open(outcarName);
        for line in outcar.readlines():
            if("2PiTHz" in line.split()):
#                print(line.strip())
                index = line.split().index("2PiTHz");
                vibModes.append(float(line.split()[index+1]))
                if(index == 5):
                    countNegativeModes = countNegativeModes + 1
                outcar.close();
    except IOError:
        vibModes = []
        countNegativeModes = 0;

    return E0,vibModes,countNegativeModes



class SmlMol:
     E0 = 0.0;
     vibModes = None;
     countNegativeModes = 0;
     ZPE = 0.0;
     def Show(self):
         buf = "E0= {0:.6f}  ZPE= {1:.6f}  ZPC= {2:.6f}".\
             format(self.E0,self.ZPE,self.E0+self.ZPE);
         return buf


def SmallMolCalcZPVE(E0,modes,Natom,Linear):
    modes_to_consider = 0;
    sml = SmlMol();
    sml.E0 = E0
    sml.vibModes = modes

    if Linear=="T":
        modes_to_consider = 3*Natom - 5;
    else:
        modes_to_consider = 3*Natom - 6;
    if(modes_to_consider <= 0):
        modes_to_consider = 0;

    sml.E0 = E0;
    sml.ZPE = sum(sml.vibModes[0:modes_to_consider])* 0.5 * PLANCK_CONSTANT * NA * SPEED_OF_LIGHT / 1000.0 / EV_TO_KJ;

    return sml


import sys;
#print(sys.argv)
if(len(sys.argv) != 5):
    print("Usage: python SmallMolCalcZPVE.py OSZICAR-(from relaxation) OUTCAR-(from vibration)) #ofAtoms Mol_is_Liner?(T/F)")
    exit()

E0,modes,negModeCount = ReadFiles(sys.argv[1],sys.argv[2]);
g = SmallMolCalcZPVE(E0,modes,int(sys.argv[3]),sys.argv[4])
print(g.Show())
