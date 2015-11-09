type=$(filesorter $1);
if [ "$type" == "POSCAR" ];
    then
    poscar_mul $1 2 2 1 > $1.P
else
    contcar2poscar $1 > tmp;
    poscar_mul tmp 2 2 1 > $1.P
    rm tmp
fi

vasp2xyz1 $1.P
rm $1.P
