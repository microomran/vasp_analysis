from ase.io.vasp import read_vasp,write_vasp
from ase.constraints import FixAtoms
from ase.io import write
import sys
filein=sys.argv[1]
atoms=read_vasp(filename=filein)
print "read file %s" % filein
a=atoms.get_cell();
b=atoms.get_scaled_positions()
criteria=0.90
for i in b:
	if i[0]>criteria:
		i[0]=i[0]-1;
	if i[1]>criteria:
                i[1]=i[1]-1;
	if i[2]>criteria:
                i[2]=i[2]-1;
#print b
atoms.set_scaled_positions(b)
#for atom in atoms:
#        print atom
write("a.xyz",atoms)
