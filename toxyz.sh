type=$(filesorter $1);
if [ "$type" == "POSCAR" ];
    then
    vasp2xyz2 $1;
    cat $1.xyz
    rm $1.xyz
else
    contcar2poscar $1 > tmp;
    vasp2xyz2 tmp 2>/dev/null;
    cat tmp.xyz;
    rm tmp tmp.xyz;
fi
