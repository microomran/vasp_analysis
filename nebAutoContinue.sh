Execute(){
    for i in {1..100}
    do
	if [ ! -d run$i ]
	    then
	    break;
	fi
    done;
    prev=$(echo "$i-1"|bc);
    cur=$i

    echo "run$prev run$cur"
    cp run$prev -r run$cur
    cd run$cur
    nebagain.sh
    #qsub runvasp.pbs;
    cd ..
}    

if [ $# != 1 ];
then
    echo "Usage: nebAutoContinue.sh [nebJobID]"
    exit;
fi

startTime=$(date)
status=$(myps|grep $1|wc -l)

if [ $status -eq 0 ]
then
    echo "Job $1 is not running"
    exit;
fi;


endTime=""
for i in {1..100000}
do
    echo "Monitoring $1, checked every 60s and $i times"

    if [ $status -eq 1 ]
    then
	status=$(myps|grep $1|wc -l)
	if [ $status -eq 0 ];
	then
	    Execute;
	    endTime=$(date);
	fi;
    else
	echo "New job already submitted at $endTime"
    fi;    
    
    sleep 60
done;
