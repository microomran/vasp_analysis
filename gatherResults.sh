#Check the list of directory, whether they have results. If so, tabulate the main results
if [ $# -eq 0 ];then
   echo "please specify job list file"
   exit
fi
if [ -d ~\images ];then
   rm -f ~/images/*
   echo -e "Remove old images from ~/images"
fi
filename=$1
IFS=$'\n'
count=1
curDir=`pwd`
source ~/vaspkit/vaspkit_library.sh
for next in `cat $filename`
do
        if [ -d $next ];then
                cd $next
                if [ `isVASPFolder` == 'y' ] && [ `hasResult` == 'y' ] && [ `isConverge` == 'y' ];then
                        res=`extractRes.sh`
			a=$next
			IFS='/' read -r -a array <<< "$a"  # explode varrable to array
			size=$((${#array[@]}-1)) # get the last index
			ModifyXYZ.sh CONTCAR- >> /dev/null
			mv modified.xyz "${array[$size]}"".xyz"
			#vasp2xyz1 CONTCAR- > "${array[$size]}"".xyz"
			mv "${array[$size]}"".xyz" ~/images  # move xyz file to ~/images folder
                        echo -e $next $res #printf "$s $s\n" $next $res
                else
                        running=`IsRunning.sh $next`
			if [ "$running" == "running" ];then
				echo -e $next "running"
			else
				echo -e $next "No results"
			fi
                fi
        fi
done
echo -e "xyz files are in ~/images folder"
echo -e "Mission Completed"
