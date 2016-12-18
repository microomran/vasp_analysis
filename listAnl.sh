filename=$1
IFS=$'\n'
for next in `cat $filename`
do
   if [ -d $next ];then
     cd $next
     anlAll.sh
   fi
done
echo "success"
exit 0

