images=$1
if [ -z $images ];then
  echo "Please specify how many images to make"
  exit
fi
for((i=2;i<=${images};i++)){
  cp -r Fe1 "Fe$i"
}
if [ ! -f ./vector ];then
  echo "no vector file"
  exit
fi
for((i=1;i<=${images};i++)){
  xyz2vasp "Fe$i.xyz" vector > "Fe$i/POSCAR"
  cd "Fe$i"
  vasp2xyz1 POSCAR > 1.xyz
  cd ..
}

