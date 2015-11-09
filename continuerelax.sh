dirname=""
for i in {1..100}
do
  if [ ! -d "run$i" ]
      then
      dirname="run$i"
      break
  fi
done

mkdir $dirname

cp POSCAR INCAR KPOINTS POTCAR runvasp.pbs $dirname/
cp CONTCAR- POSCAR

mv *- pbs_out $dirname/ 



if [ -z $1 ]
    then
    qsub runvasp.pbs
else
    qsub runvasp.pbs -N $1
fi

echo $dirname
