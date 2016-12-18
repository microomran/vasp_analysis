source ~/vaspkit/vaspkit_library.sh
if [ `isVASPFolder` == 'n' ];then  # tell whether this is a VASP folder
	echo "This is not a VASP folder"
	exit 0
fi
if [ `hasResult` == 'n' ]; then # tell whether this folder has VASP Results
	echo "No results yet"
	exit 0
fi
#----------------error: WARNING: Sub-Space-Matrix is not hermitian in DAV---------------------------
err="WARNING: Sub-Space-Matrix is not hermitian in DAV"
out=`grep -m 1 "$err" pbs_out`
if [ ! -z "$out" ];then
	echo "error: WARNING: Sub-Space-Matrix is not hermitian in DAV"
fi
#---------------Not converge even once--------------------------------------------------------------
err="1 F="
out1=`grep -m 1 "$err" OSZICAR-`
if [ -z "$out1" ];then
        echo "the running does not converge even once"
fi
#---------------Reach the Maximum NELM step---------------------------------------------------------
temp=`grep "NELM " INCAR | awk '{print $3}'`
out2=`cat pbs_out | awk '{print $2}' | grep -m 1 "$temp"`
if [ ! -z "$out2" ];then
        echo "reached maximum NELM step"
fi
#---------------very serious problems---------------------------------------------------------------
err="very serious problems"
out3=`grep -m 1 "$err" pbs_out`
if [ ! -z "$out3" ];then
        echo "encouter very serious problems"
fi
