type=$(filesorter $1);
if [ "$type" == "POSCAR" ];
    then
    poscaralign $1 $2 > tmp;
    vasp2xyz1 tmp;
    rm tmp;
else
    contcar2poscar $1 > tmp;
    poscaralign tmp $2 > tmp2;
    mv tmp2 tmp;
    vasp2xyz1 tmp;
    rm tmp;
fi

