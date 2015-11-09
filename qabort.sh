if [ $# != 1 ]
    then
    echo "Usage : qcopy.sh [job-ID]"
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

# This is the "soft stop" (stops at the next inoic step)
#echo "LSTOP = .TRUE." > STOPCAR
# This is the "hard stop" (stops at the next electronic step)
 echo "LABORT = .TRUE." > STOPCAR

for i in $(cat hosts)
do
  echo "Copy STOPCAR to node $i";
  rcp ./STOPCAR $i:/tmp/pbstmp.$1/
done

rm hosts
rm STOPCAR