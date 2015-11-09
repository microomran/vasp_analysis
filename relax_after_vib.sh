atom=$(cat vib/POSCAR | grep " T " | wc -l)
echo $atom;
N=`expr $atom \* 3`;
echo $N;
pushalongeigenvector.sh CONTCAR- vib/OUTCAR- $N > prePOSCAR
continuenostart.sh
mv prePOSCAR POSCAR


