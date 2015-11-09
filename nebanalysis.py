#!/usr/bin/env python

#Usage: Use stdin to input 5 parameters: name_of_neb_run, folder_of_neb, folder_of_neb_tsvib, folder_of_IS, folder_of_FS
#There should be a folder named 'vib' under folder_of_IS and folder_of_FS to calculate the modes of IS and FS


import sys
import os
import commands
import string
import math

def MyChdir(dest):
    try:
        os.chdir(dest);
    except OSError:
        print("Path: "+dest+" is invalid")
        exit(-1)

def ReadVibModes():
    modes = []
    status, output = commands.getstatusoutput("cat OUTCAR-|grep THz")
#   print(output)
    for line in output.split("\n"):
        parts = line.split('=')
        if(len(parts)!=2):
            print("Can't find any vibrational modes")
            return []
        mode = float((parts[1].split())[0])  # vibrational mode in THz
        if "i" in parts[0]:
            mode = -mode
        modes.append(mode)
            
#        for i in modes:
#            print(i * 100 / 2.99792, "cm-1")
    return modes

def ReadE0():
    status, output = commands.getstatusoutput("cat OUTCAR- | grep energy|grep without|grep entropy")
    line = (output.split("\n"))[-1]
#    print line
    E0 = float((line.split())[-1])
    return E0

class NEB:
    def __init__(self,name):
        self.name = name
        self.ImageCount = int(0)
        self.Energies = []
#        self.TSIndex = int(0);
#        self.ForwardBarrier = float(0.0);
#        self.BackwardBarrier = float(0.0);
        self.ModesOfIS = []
        self.ModesOfTS = []
        self.ModesOfFS = []
#       All modes are in unit of THz(1e12 Hz)

    def Read(self,NEBFolder,NEBVibFolder,ISFolder,FSFolder):
        pwd = os.getcwd();
        # 1. Read Images, and their associated energy
        MyChdir(NEBFolder)

        status, output = commands.getstatusoutput("nebef.pl")
        for line in output.split("\n"):
            e = line.split()[2]
            self.Energies.append(float(e));
        self.ImageCount = len(self.Energies);
        
                 
        #2. Read TS vibrational modes
        MyChdir(NEBVibFolder)
        self.ModesOfTS = ReadVibModes()
        if(len(self.ModesOfTS)==0):
            print("Error : TS found not vibrational mode")
            exit(-1)
#        print self.ModesOfTS
        if(self.ModesOfTS[-1] > 0):
            print("Warning : TS does not have a negative mode : "+str(self.ModesOfTS[-1])+" THz")
        if(self.ModesOfTS[-2] < 0):
            print("Warning : TS has more than one negative mode : "+str(self.ModesOfTS[-2])+" THz")

        #3. Read IS and FS vibrational modes
        for state, folder, imageIndex in zip(["IS","FS"], [ISFolder, FSFolder], [0, self.ImageCount - 1]):
#            print(folder, imageIndex)
            MyChdir(folder)
            E0 = ReadE0()
            if (abs(E0 - self.Energies[imageIndex]) > 1e-4 ):
                print("Warning : Energy of " + state + " = "+str(E0)+"eV, does not match energy of Image 0" +
                      str(imageIndex) + " : " + str(self.Energies[imageIndex])+"eV")
            MyChdir(folder+"/vib")
            modes = ReadVibModes()
            if(len(modes) != len(self.ModesOfTS)):
                print("Warning : " + state + " has different number of modes (" + str(len(modes)) + 
                      ") from TS (" + str(len(self.ModesOfTS)) + " modes)")
            if(modes[-1] < 0):
                print("Warning : " + state + " has at least one imag mode : "+str(modes[-1])+" THz")
            
            if(state == "IS"):
                self.ModesOfIS = modes
            else:
                self.ModesOfFS = modes


    def Calculate_DavidSholl(self,Temperature): #Temperature in Kelvin
        TSIndex = 0;
        for i,energy in enumerate(self.Energies):
            if( self.Energies[TSIndex] < self.Energies[i]  ):
                TSIndex = i
                
        FBarrier = self.Energies[TSIndex] - self.Energies[0];
        BBarrier = self.Energies[TSIndex] - self.Energies[-1];
            
        FPre = 0.0
        BPre = 0.0
        FRate = 0.0
        BRate = 0.0


        Kb = 1.3806488e-23
        eV = 1.60217657e-19
        Planck = 6.62606957e-34


        def f(mode, T):
            mode = abs(mode)
            x = Planck * mode * 1e12 / 2 / T / Kb
            return math.sinh(x)/x * mode * 1e12

        for direction in ["F","B"]:
            prefactor = 1.0;
            barrier = 0.0;
            modes = []
            if(direction == "F"):
                modes = self.ModesOfIS
                barrier = FBarrier
            else:
                modes = self.ModesOfFS
                barrier = BBarrier
                
            for i in range(len(self.ModesOfTS)-1):
                
                prefactor = prefactor * f(modes[i],Temperature) / f(self.ModesOfTS[i],Temperature)

            prefactor = prefactor * f(modes[-1],Temperature)

            exponential = math.exp( - barrier * eV / Kb / Temperature)
            
            rate = prefactor * exponential
                
            if(direction == "F"):
                FPre = prefactor
                FRate = rate
            else:
                BPre = prefactor
                BRate = rate
            

        return FBarrier, 0, FPre, FRate, BBarrier, 0, BPre, BRate

    def Calculate_Finite_T(self,Temperature): #Temperature in Kelvin
        TSIndex = 0;
        for i,energy in enumerate(self.Energies):
            if( self.Energies[TSIndex] < self.Energies[i]  ):
                TSIndex = i

        FBarrier = self.Energies[TSIndex] - self.Energies[0];
        BBarrier = self.Energies[TSIndex] - self.Energies[-1];

        Kb = 1.3806488e-23
        eV = 1.60217657e-19
        Planck = 6.62606957e-34

        FPre = 0.0
        BPre = 0.0
        FRate = 0.0
        BRate = 0.0

        for direction in ["F","B"]:
            barrier = 0.0;
            modes = []
            if(direction == "F"):
                modes = self.ModesOfIS
                barrier = FBarrier
            else:
                modes = self.ModesOfFS
                barrier = BBarrier

            ZPC = 0.0
            for i in range(len(self.ModesOfTS)-1):
                ZPC = ZPC + Planck * abs(self.ModesOfTS[i]) * 1e12 / 2.0 / eV
            for i in range(len(modes)):
                ZPC = ZPC - Planck * abs(modes[i]) * 1e12 / 2.0 / eV

            barrier = barrier + ZPC;


            exponential = math.exp( - barrier * eV / Kb / Temperature)
            
            prefactor = 1.0
            for i in range(len(modes)):
                q = 1 - math.exp(- Planck * abs(modes[i]) * 1e12 / Kb / Temperature);
                prefactor = prefactor * q;
            for i in range(len(self.ModesOfTS)-1):
                q = 1 - math.exp(- Planck * abs(self.ModesOfTS[i]) * 1e12 / Kb / Temperature);
                prefactor = prefactor / q;

 
            prefactor = prefactor * Kb * Temperature / Planck

            rate = prefactor * exponential

            if(direction == "F"):
                FPre = prefactor
                FRate = rate
                FBarrier_ZPC = FBarrier + ZPC
            else:
                BPre = prefactor
                BRate = rate
                BBarrier_ZPC = BBarrier + ZPC

        return FBarrier,FBarrier_ZPC, FPre, FRate, BBarrier,BBarrier_ZPC, BPre, BRate

    def Show3Energies(self): #Temperature in Kelvin
        TSIndex = 0;
        for i,energy in enumerate(self.Energies):
            if( self.Energies[TSIndex] < self.Energies[i]  ):
                TSIndex = i

        FBarrier = self.Energies[TSIndex] - self.Energies[0];
        BBarrier = self.Energies[TSIndex] - self.Energies[-1];

        eV = 1.60217657e-19
        Planck = 6.62606957e-34



        ZPC = 0.0
        for i in range(len(self.ModesOfIS)):
            ZPC = ZPC + Planck * abs(self.ModesOfIS[i]) * 1e12 / 2.0 / eV
        IS = self.Energies[0] + ZPC

        ZPC = 0.0
        for i in range(len(self.ModesOfTS)-1):
            ZPC = ZPC + Planck * abs(self.ModesOfTS[i]) * 1e12 / 2.0 / eV
        TS = self.Energies[TSIndex] + ZPC


        ZPC = 0.0
        for i in range(len(self.ModesOfIS)):
            ZPC = ZPC + Planck * abs(self.ModesOfFS[i]) * 1e12 / 2.0 / eV
        FS = self.Energies[-1] + ZPC


        sys.stdout.write("{0:s}:\t{1:f}  {2:f}  {3:f}".format(self.name,IS,TS,FS));
        print("")

    def Show(self,Temperature = 300,verbose = False):
        if(verbose):
            print("Reaction : "+self.name);
            print("Energy of Images (eV):");
            for i in self.Energies:
                sys.stdout.write("{0:f}".format(i)+"\t")
                print("");
            print("Mode of TS, IS and FS are (THz):");
            for i in range(len(self.ModesOfTS)):
                print(str(self.ModesOfTS[i])+"\t"+str(self.ModesOfIS[i])+"\t"+str(self.ModesOfFS[i]));
            print("Name:  DE(eV), DE_ZPC(eV), FB(eV), FB_ZPC(eV),  FPre(Hz), FRate(Hz), BB(eV), BB)ZPC(eV), BPre(Hz), BRate(Hz) are :" )


        FB,FB_ZPC, FPre, FRate, BB,BB_ZPC, BPre, BRate = self.Calculate_Finite_T(Temperature)
        sys.stdout.write("{0:s}:\t{1:f}  {2:f}  {3:f}  {4:f}  {5:e}  {6:e}  {7:f}  {8:f}  {9:e}  {10:e}".
                         format(self.name,FB-BB, FB_ZPC-BB_ZPC, FB,FB_ZPC,FPre,FRate,BB,BB_ZPC,BPre,BRate))
        print("")

#        FB,FB_ZPC, FPre, FRate, BB,BB_ZPC, BPre, BRate = self.Calculate_DavidSholl(Temperature)
#        sys.stdout.write("{0:s}:\t{1:f}  {2:f}  {3:f}  {4:f}  {5:e}  {6:e}  {7:f}  {8:f}  {9:e}  {10:e}".
#                         format(self.name,FB-BB, FB_ZPC-BB_ZPC, FB,FB_ZPC,FPre,FRate,BB,BB_ZPC,BPre,BRate))
#        print("")

                             
# Main function

#def Main():
#    args = [];
#    for i in range(5):
#        args.append(sys.stdin.readline().strip())

#    nebcase = NEB(args[0]);
#    nebcase.Read(args[1],args[2],args[3],args[4]);
#    nebcase.Show(550)

#Main();
