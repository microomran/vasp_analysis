run(){
    echo $1
    cd ./$1/
    rm vasp
    rm *e*
    rm *o*
    cd ..
}

for i in $(ls)
do
   if [ -d $i ];
      then 
       run $i;
   fi
done
