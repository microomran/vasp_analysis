#!/usr/bin/env python

#Check whether the magnetic moments produced in OUTCAR reaches the requirement given in the global parameter magmom_out.
#Works for NEB, relaxation, and dimer. the script itself will judge which case it is.
#works only for ferromagnetic systems. Because this script given a upper and lower bound for each element given in "magmom_out".
#Can modify the script to account for more complex system such as AFM. Need to modify function "CheckMagmom" to allow for more complicated
# judging criteria.

import sys
import os.path

magmom_in = {"Co":3.0}
magmom_out = {"Co":(1.2,2.0)}


def ReadCONTCAR(contcar_name):
    atom_types = []
    atom_counts = []
    try:
        contcar=open(contcar_name,"r")
        for i in range(6):
            line = contcar.readline();
        atom_types = line.split()
        line = contcar.readline().split();
        for i in line:
            atom_counts.append(int(i))
    except:
        print("Can't open "+contcar_name);
        return (-1,None,None);
    else:
        print("CONTCAR OK.")
        return (0,atom_types,atom_counts);

def ReadOUTCAR(outcar_name):
    mags = []
    try:
        outcar = open(outcar_name,"r")
        outcar_lines = outcar.readlines();
        for i in range(len(outcar_lines)-1,0,-1):
            if outcar_lines[i].strip() == "magnetization (x)":
                break
        else:
            print("Can't find mag information in OUTCAR-")
            raise Exception;

        i = i+4
        while(True):
            words = outcar_lines[i].strip().split()
            if words[0].isdigit() == False:
                break;
            else:
                mags.append(float(words[4]))
            i = i+1;
            
    except:
        print("Can't open "+outcar_name);
        return (-1,None);
    else:
        print("OUTCAR OK.")
        return (0,mags);


def SuggestedMAGMOM(atom_types,atom_counts):
    output = "MAGMOM = "
    for i in range(len(atom_types)):
        mag = None;
        if(atom_types[i] in magmom_in):
            mag = magmom_in[atom_types[i]]
        else:
            mag = 0.0

        if(atom_counts[i] == 1):
            output += "%.1f "%(mag)
        else:
            output += "%d*%.1f "%(atom_counts[i],mag)
    return output

        
def CheckMagmom(atom_types,atom_counts,mags):
    expanded_types = []
    for i in range(len(atom_counts)):
        for j in range(atom_counts[i]):
            expanded_types.append(atom_types[i])
    if(len(expanded_types) != len(mags)):
        print("CheckMagmom failed: expected %d atoms, read %d magmoms in OUTCAR" % (len(expanded_types,len(mags))))
        return -1;
    
    correct = True
    for i in range(len(mags)):
        if(expanded_types[i] in magmom_out):
            allowed_range = magmom_out[expanded_types[i]]
            if(mags[i] < allowed_range[0] or mags[i] > allowed_range[1]): 
                print("CheckMagmom found a disagreement for atom %d, expected (%.1f, %.1f), actually %.3f" % (i,allowed_range[0],allowed_range[1], mags[i]) )
                correct = False
    if(correct == True):
        print("CheckMagmom found MAGMOMs all correct.")
        return 0
    else:
        return -1;

def CheckRelaxationOrDimer(withDash = True):
    if(withDash):
        contcar_name = "CONTCAR-"
        outcar_name = "OUTCAR-"
    else:
        contcar_name = "CONTCAR"
        outcar_name = "OUTCAR"
                        
        
    
    (result,atom_types,atom_counts) = ReadCONTCAR(contcar_name)
    if(result == -1):
        sys.exit()

    (result, mags) = ReadOUTCAR(outcar_name)
    if(result == -1):
        sys.exit();

    result = CheckMagmom(atom_types,atom_counts,mags)

    if(result == -1):
        print("Suggested MAGMOM setting:")
        print(SuggestedMAGMOM(atom_types,atom_counts))

def CheckNEB():
    (result,atom_types,atom_counts) = ReadCONTCAR("01/CONTCAR")
    if(result == -1):
        sys.exit()

    overall_correctness = True;
    for i in range(100):
        name = "%02d" % i
        if os.path.exists(name):
            print("Checking "+name+" ...")
            (result, mags) = ReadOUTCAR(name+"/OUTCAR")
            if(result == -1):
                sys.exit();

            result = CheckMagmom(atom_types,atom_counts,mags)
            if(result == -1):
                overall_correctness = False                            
        else:
            break

    if(overall_correctness == False):
        print("Suggested MAGMOM setting:")
        print(SuggestedMAGMOM(atom_types,atom_counts))
                        



if(os.path.exists("00")):
    CheckNEB()
elif(os.path.exists("DIMCAR")):
    CheckRelaxationOrDimer(withDash = False)
else:
    CheckRelaxationOrDimer(withDash = True)
