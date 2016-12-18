mkdir LSOL
cp INCAR KPOINTS POTCAR CONTCAR- runvasp.pbs WAVECAR- LSOL/
cd LSOL/
ls
mv CONTCAR- POSCAR
ls
mv WAVECAR- WAVECAR
ls
editTag.sh add LSOL .TRUE.
editTag.sh modify ISTART 1
editTag.sh modify ICHARG 2
editTag.sh modify EDIFF 1e-3
