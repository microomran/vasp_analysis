id=`grep "#PBS -N" runvasp.pbs | awk '{print $3}'`
echo "$id"
