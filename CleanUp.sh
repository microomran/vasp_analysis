# remove all unrelevant file, and restart a initial vasp run
source ~/vaspkit/vaspkit_library.sh
curFolder=`pwd`
if [ `isVASPFolder` == 'n' ];then  # tell whether this is a VASP folder
        echo "This is not a VASP folder"
        exit 0
fi
if [ -d "run1" ];then  # has run1 folder
	cd run1
	cp INCAR KPOINTS POSCAR runvasp.pbs POTCAR ..   # move five files to current folder from run1
	cd "$curFolder"
	ij=1
	for (( ; ; ))
        do
		folder="run$ij"
                if [ -d "$folder" ]; then
                  rm -rf "$folder"
		  echo -e "remove folder ${folder}"
		else
		  break
                fi
		ij=$((ij+1))
        done
fi
if [ `hasResult` == 'y' ];then
	qclear.sh 1
fi
if [ `isVASPFolder` == 'n' ];then  # tell whether this is a VASP folder
        echo "This is not a VASP folder"
        exit 0
fi
for i in `ls`
do
	if [ "$i" == "POSCAR" ] || [ "$i" == "INCAR" ] || [ "$i" == "runvasp.pbs" ] || [ "$i" == "KPOINTS" ] || [ "$i" == "POTCAR" ];then
		echo -e "keep " "$i"
	else
		rm -rf "$i"
		echo -e "delete" "$i"
	fi
done
echo -e "Mission Completed"
