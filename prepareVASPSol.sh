ls
mkdir VASPSol
ls
cp INCAR KPOINTS POTCAR CONTCAR- runvasp.pbs VASPSol/
cd VASPSol/
ls
cp ~/runvasp_ruby_sol.pbs runvasp.pbs
ls
mv CONTCAR- POSCAR
editTag.sh modify ISTART 0
editTag.sh modify ICHARG 2
IBRION2.sh
