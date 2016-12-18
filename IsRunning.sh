if [ $# -eq 1 ];then
	path=$1
else
	path=`pwd`
fi
res=$(GetDir.sh $path) # get the job id
if [ -z "$res" ];then
	echo -e "not running"
	exit 0
fi
qstat -u osu9577 | awk '{print $4}'> ~/running
running=`grep "${res:1}" "$HOME/running"`
if [ -z "$running" ];then
	echo -e "not running"
else
	echo -e "running"
fi
