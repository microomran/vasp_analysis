run(){
    cd ./$1/
    echo -n $1
    if [ -e OSZICAR- ]
    then
	tail -1 OSZICAR-;
    elif [ -e OSZICAR ]
    then
	echo -n " !OSCIAR! ";
	tail -1 OSZICAR;
    else
	echo  " No energy info";
    fi

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