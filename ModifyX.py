from ase.io.vasp import read_vasp,write_vasp
from ase.constraints import FixAtoms
from ase.io import write
import sys
filein=sys.argv[1]
atoms=read_vasp(filename=filein)
print "read file %s" % filein
a=atoms.get_cell();
b=atoms.get_scaled_positions()
criteria=0.8
for i in b:
	if i[0]>criteria:
		i[0]=i[0]-1;
#print b
atoms.set_scaled_positions(b)
#for atom in atoms:
#        print atom
write("a.xyz",atoms)
