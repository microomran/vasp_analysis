i=`head -n 1 ~/.id`
let "i-=1"
echo $i
if [ -f runvasp.pbs ]; then
sed -i "2s/.*/#PBS -N $i /" runvasp.pbs
echo "success"
qsub runvasp.pbs
pwd
echo $i > ~/.id
JobSqlite.sh
fi


