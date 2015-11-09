#!/usr/bin/env python
import sys;
from  Gibbs import *;

halfH = -3.449022

def GetDipole(outcar):
    try:
        dipole_values = []
#        print(outcar)
        f = open(outcar,'r')
        lines = f.readlines();
#        print("opened")
        for line in lines:
            if "dipolmoment" in line:
#                print(line)
                dipole_values.append(line.split()[3])
#                print(dipole_values[-1])
        return float(dipole_values[-1]);
    except:
        print("Can't open OUTCAR file or can't find dipolmoment flag in it: "+outcar);

class U0:
    def __init__(self,path,temperature=300):
# path is a string array. path[0] is reaction name,
# path[1], path[2], path[3], path[4] are folders of IS, TS, FS, and IS.noH respectively
        self.name = path[0]
        self.g = []
        self.temperature = temperature
        for i in range(4):
            E0,vibs,negModes = ReadFiles(path[i+1]+"/OSZICAR-",path[i+1]+"/vib/OUTCAR-");
            if(i==1):
                negModes = 1
            else:
                negModes = 0

            self.g.append(CalculateGibbs(E0,vibs,negModes,temperature,noWarning=True))
#            print(self.g[i].Show())

        self.ISdipole = GetDipole(path[1]+"/OUTCAR-")
        self.TSdipole = GetDipole(path[2]+"/OUTCAR-")

    def Show(self,verbose=True):
        IS = self.g[0]
        TS = self.g[1]
        FS = self.g[2]
        IS_noH = self.g[3]
        Eact = TS.E0 - IS.E0
        Eact0 = TS.ZPE - IS.ZPE + Eact
        U0 = halfH + IS_noH.G - IS.G
        beta = (self.TSdipole - self.ISdipole)/3 + 0.5
        dG = FS.G - IS_noH.G - halfH
        dE = FS.E0 - IS.E0
        dGpro = FS.G - IS.G
        print("Reaction: "+self.name)
        print("                vib     E           G          Eact     Eact0    U0       Beta     dG      dE        dGpro    Eact(0VRHE)");
        print("Reactant\t0imag\t{0:8.5f} {1:8.5f} {2:8.5f} {3:8.5f} {4:8.5f} {5:8.5f} {6:8.5f} {7:8.5f} {8:8.5f} {9:8.5f}"
                  .format(IS.E0,IS.G,Eact,Eact0,U0,beta,dG,dE,dGpro,Eact0+beta*(0-U0)))
        if (verbose):
            print("Product\t0imag\t{0:8.5f} {1:8.5f}".format(FS.E0,FS.G))
            print("TS\t1imag\t{0:8.5f} {1:8.5f}".format(TS.E0, TS.G))
            print("Product\t0imag\t{0:8.5f} {1:8.5f}".format(FS.E0,FS.G))
            print("Reaction_wo_H\t0imag\t{0:8.5f} {1:8.5f}".format(IS_noH.E0,IS_noH.G))
            print("TS_ZPVE\t{0:8.5f}".format(TS.ZPE))
            print("Reactant_ZPVE\t{0:8.5f}".format(IS.ZPE))
            print("TS_dipole\t{0:8.5f}".format(self.TSdipole))
            print("Reactant_dipole\t{0:8.5f}".format(self.ISdipole))

    
