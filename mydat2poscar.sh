startimage=$1
endimage=$2
atomperimage=$3

i=$startimage
while [ $i -le $endimage ]
do
  echo $i
  contcar2poscar CONTCAR > temp
  head -8 temp > POSCAR$i.out
  rm temp
  endline=`expr $i \* $atomperimage + $i + 7`
  head -$endline XDATCAR | tail -$atomperimage >> POSCAR$i.out
  
  i=`expr $i + 1`
done
