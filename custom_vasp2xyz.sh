type=$(filesorter $1)
if [ $type == "CONTCAR" ]
then
    contcar2poscar $1 > custom_vasp2xyz_p
else
    cp $1 custom_vasp2xyz_p
fi

poscar_mul custom_vasp2xyz_p 2 2 1 > custom_vasp2xyz_p2
vasp2xyz1 custom_vasp2xyz_p2
rm custom_vasp2xyz_p custom_vasp2xyz_p2