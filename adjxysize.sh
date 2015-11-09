if [ $# != 1 ]
then
    echo "Usage: adjxysize.sh POSCAR"
    exit
fi
poscar=$1
tempHead=tempHead

head -2 $poscar > $tempHead
for line in 3 4
do
    echo -n "  " >> tempHead
    for i in $(head -$line $poscar | tail -1 );
    do
	echo -n $(echo "$i*2.0"|bc) >> $tempHead;
	echo -n "    " >> $tempHead
    done
echo "" >> $tempHead
done;
head -5 $poscar | tail -1 >> $tempHead


~/luo/vasp2xyz2 $poscar 2> /dev/null
~/luo/xyz2vasp $poscar.xyz $tempHead



rm $poscar.xyz $tempHead
