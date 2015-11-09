if [ $# != 2 ]
    then
    echo "Usage : qcopy.sh [job-ID] [file_copy_destination]"
    exit
fi


if [ ! -d $2 ] 
then
    echo "Destination $2 is invalid"
    exit
fi


if [[ $(hostname) == *'oakley'* ]];
then
    qstat -n $1 | tail -n+7 | tr " +/" "\n" | grep n | sort -u > hosts
else
    qstat -n $1 | tail -n+7 | tr " +/" "\n" | grep opt|sort -u > hosts
fi;



if [ $(cat hosts | wc -l) = 0 ]
    then
    echo "Job $1 is not running, cannot find hosts for it";
    exit
fi


#for i in $(cat hosts)
#do
#  rm -rf $i
#  mkdir $i
#  echo "Copying from node $i";
#  rcp -r $i:/tmp/pbstmp.$1/* $2/$i/;
#done

# Combine according to time stamp
cd $2
for h in $(cat hosts)
do
  if [ ! -d $h ];then
      echo "$h not found"
      continue
  fi
  cd $h
  for file in $(find .)
  do
    if [ -d $file ]; then
	echo "mkdir $file"
	mkdir $2/$file 2>/dev/null
    elif [[ -f $file  &&  ! -e $2/$file ]]; then
	echo "cp $file $2"
	cp $file $2/
    elif [[ -f $file  &&  -f $2/$file  &&  $file -nt $2/$file ]]; then
	echo "cp $file $2"
	cp $file $2/
    else
	echo "$file is older than existing file"
    fi
  done
  cd ..
done

rm hosts
