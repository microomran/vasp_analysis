run(){
    echo $1
    cd ./$1/
    rm vasp
    cd ..
}

for i in $(ls)
do
   if [ -d $i ];
      then 
       run $i;
   fi
done
