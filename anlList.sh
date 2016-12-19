PATH=$PATH:/nfs/08/osu9577/luo:/usr/local/torque/5.1.1-1_fba25d92/bin/
export PATH
if [ ! -f ~/.yes ];then   # tell if autoupdate is turned on (has .yes file) or not
	exit 0
fi
if [ $# -eq 0 ];then
   echo "please specify job list file"
   exit
fi
filename=$1
IFS=$'\n'
count=1
source ~/vaspkit/vaspkit_library.sh
echo > ~/errors  # clean the errors file
for next in `cat $filename`
do
  if [ -d $next ];then
    cd $next
    echo -e $next
    if [ `isVASPFolder` == 'n' ];then
	continue
    fi
    if [ `isRunning` == 'y' ];then  # the job is still running
        continue
    fi
    if [ `hasResult` == 'y' ];then  # has VASP Results
        # ----------------------Bader---------------------------------------
        if [ `RunType.sh` == "BADER" ];then
	  if [ -f "ACF.dat" ];then   # already has result and performed bader analsyis
            rmWAVECHG.sh
            continue
          fi
          if [ ! -z "`grep "1 F=" pbs_out`" ] && [ -z "`ErrorDetect.sh`" ];then   # has good but have not done bader analysis
            baderAna.sh
            continue
          else
            pwd >> ~/errors
            continue
          fi
        fi
        #-----------------------Vib--------------------------------------------
        if [ `RunType.sh` == "VIB" ];then
          if [ ! -z "`grep cm OUTCAR-`" ];then  # already got restuls
            rmWAVECHG.sh
            continue
          else
            echo -e "`pwd`" "has errors"
            pwd >> ~/errors
            continue;
          fi
        fi
        #-----------------------VASPSol-----------------------------------------
        if [ `RunType.sh` == "VASPSol" ];then
          if [ `isConverge` == 'y' ];then  # already converged
            if [ -d "LSOL" ];then # already has LSOL job
              rm -f CHG*
              rm -f WAVECAR*
              continue
            else  # no LSOL job, start to create LSOL job
                prepareLSOL.sh
                qsub LSOL/runvasp.pbs
                echo "`pwd`/LSOL" >> "$filename"
                continue
            fi
            continue
          fi
        fi
        #----------------------Normal OPT-----------------------------------------
        if [ `isConverge` == 'y' ];then  # converged to required accuracy level
          rm -f CHG*
          rm -f WAVECAR*
          continue
        else  #has not converged to results
          err=`ErrorDetect.sh`
          if [ -z "$err" ];then #-------------------------no error at all -------------------------
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
              editTag.sh remove MAGMOM
              editTag.sh modify ISTART 1
              editTag.sh modify ICHARG 1
              lowAccu=`grep "reached required accuracy" pbs_out`    # tell if the result reach a lower accuracy
              if [ ! -z "$lowAccu" ];then
                editTag.sh modify EDIFF 1e-5
              fi  
              qsub runvasp.pbs   #-----------------no error at all----------------------------
          else  # has some error
              echo -e $next
              echo -e "$err"
              echo -e $next >> ~/errors
              echo -e "$err" >> ~/errors
              rm -f CHG*
              rm -f WAVECAR*
	      continue
          fi
        fi
    else
        qsub runvasp.pbs
    fi
  fi     
done
echo "Mission completed"
exit 0
