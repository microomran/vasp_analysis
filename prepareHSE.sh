mkdir HSE
cp INCAR KPOINTS POTCAR CONTCAR- runvasp.pbs WAVECAR- HSE
cd HSE
editTag.sh modify ISTART 1
editTag.sh modify ICHARG 0
editTag.sh modify NSW 0
editTag.sh add LHFCALC .TRUE.
editTag.sh delete IALGO
editTag.sh add HFSCREEN 0.2
editTag.sh add ALGO All
editTag.sh add Time 0.4
editTag.sh add PRECFOCK FAST
mv CONTCAR- POSCAR
mv WAVECAR- WAVECAR
qsub runvasp.pbs
pwd
