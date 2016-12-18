# This program insert, edit or remove tags in VASP INCAR
# delete a tag, 1 parameter, Tag name

if [ $# -eq 0 ];then
	echo "please specify 'add', 'delete', 'modify':"
	echo "for examples:"
	echo -e "\teditTag.sh delete MAGMOM"
	echo -e "\teditTag.sh comment LSOL"
	echo -e "\teditTag.sh uncomment LSOL"
	echo -e "\teditTag.sh add LSOL .TRUE."
	echo -e "\teditTag.sh modify ISTART 1"
fi
if [ $# -eq 2 ];then
	if [ "$1" == "delete" ];then  #-------------------delete--------------#
        tag=`cat INCAR | awk '{print $1}' | grep -m 1 "^$2"` # find if the tag exists in INCAR, grep only one entry
		if [ -e $tag ];then
			echo "No such tag, program exit"
			exit 0
		else
			if [ "$tag" == "$2" ];then #has that tag
				sed "/$2/d" INCAR > temp
				mv temp INCAR
				echo "successfully delete Tag $2"
			else
				echo "No such tag, program exit"
                        	exit 0
			fi
		fi	
	fi
	if [ "$1" == "comment" ];then #-------------------comment--------------#
		tag=`cat INCAR | awk '{print $1}' | grep -m 1 "^$2"` # find if the tag exists in INCAR, grep only one entry
		if [ -e $tag ];then
			echo "No such tag, program exit"
			exit 0
		else
			if [ "$tag" == "$2" ];then #has that tag
				sed "s/$2/#$2/g" INCAR > temp
				mv temp INCAR
				echo "successfully comment Tag $2"
			else
				echo "No such tag, program exit"
                exit 0
			fi
		fi
	fi	
	if [ "$1" == "uncomment" ];then #-------------------uncomment--------------#
		tag=`cat INCAR | awk '{print $1}' | grep -m 1 "^#$2"` # find if the tag exists in INCAR, grep only one entry
		if [ -e $tag ];then
			echo "No such tag, program exit"
			exit 0
		else
			if [ "$tag" == "#$2" ];then #has that tag
				sed "s/#$2/$2/g" INCAR > temp
				mv temp INCAR
				echo "successfully uncomment Tag $2"
			else
				echo "No such tag, program exit"
                exit 0
			fi
		fi
	fi
	if [ "$1" == "get" ];then #-------------------get--------------#
		tag=`cat INCAR | awk '{print $1}' | grep -m 1 "$2"`
		if [ -z $tag ];then
			echo "no"	# no such tag
		else
			res=`cat INCAR | grep -m 1 "^$2 " | awk '{print $3}'`
			echo "$res"
		fi	
	fi	
fi
if [ $# -eq 3 ];then
	if [ "$1" == "add" ];then	# -----------------------add------------------------#
        tag=`cat INCAR | awk '{print $1}' | grep -m 1 "^$2"` # find if the tag exists in INCAR, grep only one entry
		if [ -e $tag ];then
			echo "$2 = $3" >> INCAR
			echo "successfully add $2 = $3"
			exit 0
		else
			if [ "$tag" == "$2" ];then #has that tag
				echo "already has this tag"
				exit 0
			else
				echo "$2 = $3" >> INCAR
				echo "successfully add $2 = $3"
				exit 0
			fi
		fi
	fi
	if [ "$1" == "modify" ];then	# -----------------------modify------------------------#
        tag=`cat INCAR | awk '{print $1}' | grep -m 1 "^$2"` # find if the tag exists in INCAR, grep only one entry
		if [ -e $tag ];then
			echo "No such tag, exit"
			exit 0
		else
			if [ "$tag" == "$2" ];then #has that tag
				sed -i "s/$2 .*/$2 = $3/g" INCAR
				echo "successfully change to $2 = $3"
				exit 0
			else
				echo "No such tag, exit"
				exit 0
			fi
		fi
	fi
fi

# sed -i 's/ISTART .*/ISTART = 1/g' INCAR
