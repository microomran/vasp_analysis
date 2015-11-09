cd vib
qsub runvasp.pbs > $1
mv $1 ../
cd ..
