if [ $# -lt 1 ];then
	echo -e "Please specifiy POSCAR or CONTCAR file path"
	exit 0
fi
python ~/luo/ModifyXYZ.py $1
cat a.xyz | awk '{ print $1" " $2" " $3" " $4}'> modified.xyz
echo -e "output to modified.xyz"
