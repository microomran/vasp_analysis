#!/bin/env python
# This script reads from stdin a series of atoms, then grabs their POTCAR files from the preset directory
# This script by default takes no input parameter, but input parameters can be given to specify the POTCAR 
# file desired for each type of element. For example, if Mg_pv instead of Mg POTCAR is desired, this script
# can be run as 
#    echo "Mg"|make_potcar.py Mg Mg_pv
# instead of
#    echo "Mg"|make_potcar.py
# The latter form of use will generate the Mg instead of Mg_pv version of POTCAR for element Mg

import os
import sys

potcar_dir = "/nfs/01/osu6212/Shared/potcar/PAW/PBE/"
substitution_map = {}
if len(sys.argv) > 1:
    # make sure even number of arguments are given. Remeber argv[0] is always the script name so it's not counted in
    if (len(sys.argv)%2 != 1):
        print "Usage: make_potcar.py [ele1 potcar_for_ele2 [ele2 potcar_for_ele2] ...]"
        exit(-1)
    for i in range(1,len(sys.argv)-1,2):
        substitution_map[sys.argv[i]] = sys.argv[i+1]

elements = []
for line in sys.stdin:
    elements.extend(line.split())

for i in range(len(elements)):
    if elements[i] in substitution_map:
        elements[i] = substitution_map[elements[i]]

for e in elements:
    filename = potcar_dir+str(e)+"/POTCAR"
    if os.path.exists(filename):
        os.system("cat "+filename)
    else:
        print "Sorry, can't find %s"%filename
        exit(0)
