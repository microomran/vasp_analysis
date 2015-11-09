run(){
    echo -n "$1  " 
    cd ./$1/
    if [ -e pbs_out ]
	then
	cat pbs_out | grep "real"
	else
	cat *e* | grep "real"
    fi
    cd ..
}

for i in $(ls)
do
   if [ -d $i ];
      then 
       run $i;
   fi
done
