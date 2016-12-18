mkdir vib
cp INCAR KPOINTS POTCAR CONTCAR- runvasp.pbs vib
ls
cd vib
ls
cp ~/freq_INCAR INCAR
ls
mv CONTCAR- POSCAR
TtoF.sh

