FILES='./*'
Dest=$1
if [ -d $Dest ] && [ ! -z $Dest ];then
   echo "to folder $Dest"
else
   echo "no destination file"
   exit
fi
for f in $FILES
do
   filename=`basename $f`
if [[ $filename == WAVE* ]];then
    a=1  
  else
    if [[ $filename == CHG* ]];then
      a=1
    else
      cp $f $Dest
    fi
  fi
done
