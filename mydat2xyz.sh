startimage=$1
endimage=$2
atomperimage=$3

~/bin/mydat2poscar.sh $startimage $endimage $atomperimage

rm movie.xyz
touch movie.xyz
i=$startimage
while [ $i -le $endimage ]
do
  echo $i

  vasp2xyz2 POSCAR${i}.out
  cat POSCAR${i}.out.xyz >> movie.xyz

  rm POSCAR${i}.out
  rm POSCAR${i}.out.xyz


  i=`expr $i + 1`
done
