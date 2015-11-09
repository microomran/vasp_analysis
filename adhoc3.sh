contcar2poscar CONTCAR- > C
if [ -d run1 ]
then
    poscar=run1/POSCAR
else
    poscar=POSCAR
fi
poscaralign C $poscar > CP
vasp2xyz1 CP 
rm C CP
