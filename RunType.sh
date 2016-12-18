# Test what kind of calculations you are running
# Normanl optimization 		OPT
#	IBRION = 1,2,3
# Vibrational Calculation	VIB
#	IBRION = 5
# Initial VASPSol Calculation	VASPSOL
#	pwd=VASPSol
# Final VASPSol Calculation	LSOL
#	LSOL = .TRUE., pwd=LSOL
# Bader charge Calculations	BADER
#	LAECHG = .TRUE., NSW = 0
#-----------------------------Bader charge-----------------------
if [ `editTag.sh get LAECHG` == ".TRUE." ]  && [ `editTag.sh get NSW` == "0" ];then
	echo "BADER"
#----------------------------Initial VASPSol--------------------
elif [ `basename $PWD` == "VASPSol" ];then
	echo "VASPSol"
#----------------------------Final VASPSol----------------------
elif [ `basename $PWD` == "LSOL" ] && [ `editTag.sh get LSOL` == ".TRUE." ];then
	echo "LSOL"
#----------------------------Frequnecy Calculation-----------------
elif [ `editTag.sh get IBRION` == "5" ];then
	echo "VIB"
else
	echo "OPT"
fi

