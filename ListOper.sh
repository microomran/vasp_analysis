if [ $# -eq 0 ];then
   echo -e "please specify job list file"
   echo -e "Use ~/luo/Oper.sh"
   exit
fi
filename=$1
IFS=$'\n'
i=1
for next in `cat $filename`
do
   if [ -d $next ];then
     cd $next
     Oper.sh
   fi
   i=$((i+1))
done
echo "success"
exit 0

