PATH=$PATH:/nfs/08/osu9577/luo:/usr/local/torque/5.1.1-1_fba25d92/bin/
export PATH
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
        if [ ! -f OSZICAR- ];then
               continue;
        fi
    anlAll.sh >& log 
   job_id=`grep "#PBS -N" runvasp.pbs | awk '{print $3}'` 
   reachAccuracy=`grep "reached required accuracy" log`
   accuracy=`grep "EDIFF = 1e-5" log`
   running=`grep " $job_id" jobs`
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
          rmWAVECHG.sh
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
      iter=`grep F OSZICAR- | tail -n 1 | awk '{print $1}'`
      nsw=`grep NSW INCAR | awk '{print $3}'`
      nohermitan=`grep -c "WARNING: Sub-Space-Matrix is not hermitian in DAV" pbs_out `
      if [ ! -z $vib ];then
          qstat -u osu9577 > jobs
          pwd | tee -a ~/.images/continues
          running=`grep " $job_id" jobs`
          if [ -z $running ]; then
            echo $job_id  >> ~/.images/problem
            pwd >>  ~/.images/problem
          else
            echo "it is still running" > /dev/null
          fi
      elif [ $nohermitan -ne 0 ];then
          qclear.sh 1
          sed -i 's/IALGO .*/IALGO = 48/g' INCAR
          qsub runvasp.pbs
      elif [ -z $iter ];then
          echo $job_id > ~/.images/problem
          pwd > ~/.images/problem

      elif [[ $iter -eq $nsw ]];then
              ij=1
              find . -type d -name "run*" -print > .temp1
              for (( ; ; ))
              do
                folder=`grep "run$ij" .temp1`
                if [ ! -z "$folder" ]; then
                  ij=$((ij+1))
                else
                  break;
                fi
              done
              mkdir "run$ij"
              cpVASP.sh "run$ij"
              qclear.sh 2
              sed -i 's/ISTART .*/ISTART = 1/g' INCAR
              sed -i 's/ICHARG .*/ICHARG = 1/g' INCAR
              removeMAGMOM.sh
              qsub runvasp.pbs
      elif [[ $iter -lt $nsw ]];then
              qclear.sh 1
	      if [[ $iter -lt 20 ]];then
		 $iter=20
	      fi
              sed -i "s/NSW .*/NSW = $iter/g" INCAR
              qsub runvasp.pbs
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
echo "Mission completed"
exit 0
