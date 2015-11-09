run(){
    cd ./$1/
    echo -n $1
    
    cat vib/OUTCAR- | grep THz

    cd ..
}

for i in $(ls)
do
   if [ -d $i ];
      then 
       run $i;
   fi
done
