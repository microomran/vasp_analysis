# find folder named run"X"
ij=1
find . -type d -name "run*" -print > .temp1
for (( ; ; ))
do
  folder=`grep "run$ij" .temp1`
  if [ ! -z $folder ]; then
    ij=$((ij+1))
  else
    break;
  fi
done
mkdir "run$ij"
cpVASP.sh "run$ij"
qclear.sh 2
editTag.sh modify ISTART 1
editTag.sh modify ICHARG 1
