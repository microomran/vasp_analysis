run(){
    cd ./$1/
    echo -n "$1    "
    cat *.log |grep "SCF Done" | tail -1
    cd ..
}


OLD=$(pwd)
if [ ! -z $1 ]
then
    cd $1
fi

for i in $(ls)
do
   if [ -d $i ];
      then 
       run $i;
   fi
done

cd $OLD