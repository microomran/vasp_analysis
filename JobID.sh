if [ -f "runvasp.pbs" ];then
	grep "#PBS -N" runvasp.pbs | awk '{print $3}'
else
	echo -e "not a VASP Folder"
fi
