if [ $# -eq 1 ];then
	dir=$1
else
	dir="`pwd`"
fi
job_id="`grep "#PBS -N" runvasp.pbs | awk '{print $3}'`"
a=`~/sqlite/sqlite3 ~/sqlite/job.db "insert into run (id,path) values ('$job_id','$dir');"`
echo $a
