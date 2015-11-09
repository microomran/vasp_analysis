from math import *;
#constants:
PLANCK_CONSTANT = 6.6260693e-34 #J s
NA = 6.0221415e+23 # J/ mol K
SPEED_OF_LIGHT = 2.99792458e10 # cm/s
BOLTZMAN_CONSTANT = 1.3806505e-23 # J/K
IDEAL_GAS_CONSTANT = NA*BOLTZMAN_CONSTANT
EV_TO_KJ = 96.48



class Gibbs:
    E0 = 0.0;
    vibModes = None;
    ZPE = 0.0;
    Svib = 0.0;
    Srot = 0.0;
    STtotal = 0.0;
    Evib = 0.0
    Etrans = 0.0
    Erot = 0.0
    Etotal = 0.0
    G = 0.0
    temperature = 0.0
    def Show(self):
        buf = "E0= {0:.6f}  ZPE= {1:.6f}  -ST= {2:.6f}  E= {3:.6f}  G= {4:.6f}".\
            format(self.E0,self.ZPE,-self.STtotal,self.Etotal,self.G)
        return buf

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
        print("Can't open OSZICAR file "+oszicarName)
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



def CalculateGibbs(E0,vibModes,modes_to_skip,temperature,noWarning=False):

    h = PLANCK_CONSTANT
    c = SPEED_OF_LIGHT
    k = BOLTZMAN_CONSTANT
    R = IDEAL_GAS_CONSTANT

    g = Gibbs();
    g.E0 = E0;

    if(modes_to_skip > 0 and noWarning == False):
        print("Note: The last " + str(modes_to_skip) + " mode(s) are not included in ZPE and G");

    modes_included = len(vibModes) - modes_to_skip;

    g.vibModes = [];
    for i in range(0,modes_included):
        g.vibModes.append(abs(vibModes[i]))


        
#    print(g.vibModes)
                     

# ZPVE
    g.ZPE = sum(g.vibModes)*0.5*h*NA*c/1000.0/EV_TO_KJ;

#Svib and Evib
    theta = [i*h*c/k for i in g.vibModes]
    theta_over_T = [i/temperature for i in theta]
    Svib = [( -R*log(1-exp(-i)) + R*i/(exp(i)-1) )/1000.0 for i in theta_over_T]
    Evib = [ R*t / (1000.0 * (exp(tot)-1.0) ) for t,tot in zip(theta,theta_over_T) ]

    g.Svib = sum(Svib)/EV_TO_KJ
    g.Evib = sum(Evib)/EV_TO_KJ

#S rot
    g.Srot = 0.0
#E trans
    g.Etrans = 0.0
#E rot
    g.Erot = 0.0


#Summary
    g.STtotal = (g.Svib + g.Srot)*temperature
    g.Etotal = g.Evib + g.Etrans + g.Erot
    g.G = g.E0 + g.ZPE - g.STtotal + g.Etotal

    return g;
#
    
