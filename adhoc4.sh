if [ $# != 3 ]
then
    echo "adhoc4.sh name CONTCAR_of_small_cell poscar_for_alignment"
    exit
fi

cp ~/_Project/c2/100/HEAD2x3 ./
cp ~/_Project/c2/100/prep.sh ./
cp ~/_Project/c2/100/INCAR ./
if [ ! -d xyzs ]
then
    mkdir xyzs
fi
vasp2xyz1 ~/_Project/c2/100/2x3/POSCAR  > 2x3.xyz


contcar2poscar $2 > C
poscaralign C $3 > CP
vasp2xyz1 CP | grep -v Cu > ori.txt
NATOM=$(cat ori.txt|wc -l)
NATOM=$(echo "$NATOM-2"|bc)
echo $NATOM > small.xyz
tail -n+2 ori.txt >> small.xyz

xyzcat.py 2x3.xyz small.xyz > xyzs/$1.xyz

echo "Run $1 2x3" >> prep.sh

./prep.sh
rm C CP ori.txt small.xyz 2x3.xyz HEAD2x3 prep.sh INCAR
rm xyzs -r
