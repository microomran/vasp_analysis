mkdir CNZ1
cp INCAR KPOINTS POTCAR CONTCAR- WAVECAR- CHGCAR- runvasp.pbs bader
cd CNZ1
editTag.sh modify ISTART 1
editTag.sh modify ICHARG 1
editTag.sh modify NSW 0
mv CONTCAR- POSCAR
mv CHGCAR- CHGCAR
mv WAVECAR- WAVECAR
editTag.sh add ICORELEVEL 2
editTag.sh add CLNT  1
editTag.sh add CLN  1
editTag.sh add CLL  0
editTag.sh add CLZ  1
editTag.sh add LVTOT .TRUE.
addMix.sh
qsub runvasp.pbs
pwd
