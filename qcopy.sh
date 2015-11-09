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
elif [[ $(hostname) == *'ruby'* ]];
then
    qstat -n $1 | tail -n+7 | tr " +/" "\n" | grep r | sort -u > hosts
else
    qstat -n $1 | tail -n+7 | tr " +/" "\n" | grep opt|sort -u > hosts
fi;



if [ $(cat hosts | wc -l) = 0 ]
    then
    echo "Job $1 is not running, cannot find hosts for it";
    exit
fi


for i in $(cat hosts)
do
  echo "Copying files from node $i";
  rcp -r $i:/tmp/pbstmp.$1/* $2/;
#  mkdir $2/$i
#  rcp -r $i:/tmp/pbstmp.$1/* $2/$i/
done

rm hosts
