if [ ! -d vib ]
    then
    mkdir vib
fi

contcar2poscar CONTCAR- > PrePOSCAR
if [ -e vibrate_mask ]
then
    head -6 PrePOSCAR | tail -1 | tfForVib $(cat vibrate_mask) > TFFile
else
    head -6 PrePOSCAR | tail -1 | tfForVib $@ > TFFile
fi

adjustTF PrePOSCAR TFFile > ./vib/POSCAR

echo "set IBRION 5 delete POTIM delete IOPT show done" | INCARModifier INCAR > ./vib/INCAR

cp KPOINTS POTCAR runvasp.pbs ./vib/

rm PrePOSCAR
rm TFFile