if [ $# -eq 0 ];then
   echo -e "please specify job list file"
   exit
fi
filename=$1
IFS=$'\n'
rm -f ~/images/*
i=1
for next in `cat $filename`
do
   if [ -d $next ];then
     cd $next
     ModifyXYZ.sh CONTCAR-
     #folder_name=$(basename $next)  //get base name of the folder
     cp modified.xyz ~/images/"$i.xyz"
     echo -e "$next\t\t$i.xyz"
   fi
   i=$((i+1))
done
echo "success"
echo -e "xyzs put in ~/images/"
exit 0
