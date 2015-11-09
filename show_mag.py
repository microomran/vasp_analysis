#!/usr/bin/python
import sys
import itertools;
poscar = open("POSCAR","r")
poscar_lines = poscar.readlines();
elements = poscar_lines[0].split()
element_counts = poscar_lines[5].split()
#print(elements)
#print(element_counts)


sorted_element_types = []
for i in range(0,len(element_counts)):
    sorted_element_types.extend( itertools.repeat(elements[i],int(element_counts[i])) )
print(sorted_element_types)

spin_elements = ["Co","O"]
spin_ups = ["Co","O"]
spin_downs = ["Cd","N"]


incar = open("INCAR","r")
mag_pars = []
for line in incar.readlines():
    if "MAGMOM" in line:
        for i in range(0,len(line)):
            if line[i] == '=':
                effect_line = line[i+1:len(line)]
#                print(effect_line);
                mag_pars = effect_line.split();

print("mag_parts = " + str(mag_pars))

sorted_mags = []
for i in mag_pars:
    if "*" in i:
        parts = i.split("*");
#        print("parts = " + str(parts))
        sorted_mags.extend( itertools.repeat(parts[1], int(parts[0]) ) )
    else:
        sorted_mags.append( i)

print(sorted_mags)

if (len(sorted_element_types) != len(sorted_mags)):
    print ("Atom count in MAGMOM mismatches count in POSCAR!")
    exit()

for i in range(0,len(sorted_element_types)):
    
    if sorted_element_types[i] in spin_elements:
        pos = spin_elements.index(sorted_element_types[i])

        if float(sorted_mags[i]) > 0.0:
            sorted_element_types[i] = spin_ups[pos]
        elif float(sorted_mags[i]) < 0.0:
            sorted_element_types[i] = spin_downs[pos]

#print (sorted_element_types)

elements = []
element_counts = []
for i in range(0,len(sorted_element_types)):
    if (len(elements) == 0 or elements[-1] != sorted_element_types[i] ):
        elements.append (sorted_element_types[i])
        element_counts.append (1)
    else:
        element_counts[-1] = element_counts[-1] + 1
#print(elements)
#print(element_counts)

l0 = ""
l5 = ""
for i in range(0,len(elements)):
    l0 = l0 + " " + str(elements[i])
    l5 = l5 + " " + str(element_counts[i])
    
l0 = l0 + "\n"
l5 = l5 + "\n"
poscar_lines[0] = l0
poscar_lines[5] = l5

poscar_show = open("POSCAR_SHOW","w")
for i in poscar_lines:
    poscar_show.write(i)
poscar.close()
poscar_show.close()
