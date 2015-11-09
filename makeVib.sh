mkdir vib
cp KPOINTS POTCAR runvasp.pbs CONTCAR- vib
cd vib
mv CONTCAR- POSCAR
cp ~/freq_INCAR INCAR
