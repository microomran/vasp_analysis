dirname=""
for i in {1..100}
do
  if [ ! -d "run$i" ]
      then
      dirname="run$i"
      break
  fi
done

echo $dirname
mkdir $dirname

cp POSCAR INCAR KPOINTS POTCAR runvasp.pbs $dirname/
cp CONTCAR- POSCAR

mv *- pbs_out $dirname/ 
if [ -d vib ]
    then
    mv vib $dirname;
fi
