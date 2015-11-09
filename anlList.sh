if [ $# -eq 0 ];then
   echo "please specify job list file"
   exit
fi

  if [ -d $next1 ];then
    cd $next1
  fi
  echo $next1
filename=$1
IFS=$'\n'
count=1
rm -f ~/.images/*
for next in `cat $filename`
do
  if [ -d $next ];then
    cd $next
    anlAll.sh >& log 
   job_id=`grep "#PBS -N" runvasp.pbs | awk '{print $3}'` 
   reachAccuracy=`grep "reached required accuracy" log`
   accuracy=`grep "EDIFF = 1e-5" log`
   if [[ ! -z $reachAccuracy ]]; then
      if [[ ! -z $accuracy ]]; then
          echo $job_id  | tee -a ~/.images/success
          echo "success" | tee -a  ~/.images/success
          cp 1.xyz ~/.images/"$count.xyz"
          echo $count".xyz" | tee -a  ~/.images/success
          pwd | tee -a ~/.images/success
          echo "----------------------------------------------" | tee -a ~/.images/success
          count=$((count+1))
          grep F= log | tail -1 | tee -a ~/.images/success
      else
          qstat -u osu9577 > jobs
          pwd | tee -a ~/.images/continues
          running=`grep " $job_id" jobs`
          if [[ -z $running ]]; then
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
              removeMAGMOM.sh
              highAccu.sh >& /dev/null
          else
             echo "it is still running" > /dev/null
          fi
      fi
   else
      vib=`grep "IBRION = 5" INCAR`
      if [ -z $vib ];then
          qstat -u osu9577 > jobs
          pwd | tee -a ~/.images/continues
          running=`grep " $job_id" jobs`
          if [ -z $running ]; then
            echo $job_id  >> ~/.images/problem
            pwd >>  ~/.images/problem
          else
            echo "it is still running" > /dev/null
          fi
      else
          echo $job_id >> ~/.images/success
          echo "vib" >> ~/.images/success
          pwd >> ~/.images/success
          getVib.sh >> ~/.images/success
          echo "-----------------------end-----------------------" >> ~/.images/success
      fi 
   fi
  fi
done
mv ~/list ~/old_list
mv ~/.images/continues ~/list
echo "Mission completed"
exit 0
