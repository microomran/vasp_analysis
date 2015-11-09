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
