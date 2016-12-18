mkdir bader
cp INCAR KPOINTS POTCAR CONTCAR- runvasp.pbs bader
cd bader
editTag.sh modify ISTART 0
editTag.sh modify ICHARG 2
editTag.sh modify NSW 0
editTag.sh add LAECHG .TRUE.
mv CONTCAR- POSCAR
cp ~/bader_runvasp.pbs runvasp.pbs
qsub runvasp.pbs
pwd
