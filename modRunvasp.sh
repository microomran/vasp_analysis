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
if [[ `hostname` == *'owens'* ]];then
        echo 'owens'
        pwd >> ~/newlist
elif [[ `hostname` == *'ruby'* ]];then
        echo 'ruby'
        pwd >> ~/oldlist
fi
