run(){
    if [ -d $1 ]
	then
	cd $1
	cp CONTCAR- POSCAR
	cd ..
    fi
}

set -x
for i in {1..9}
do
  run "0$i"
done
