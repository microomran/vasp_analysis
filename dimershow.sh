if [ ! $# -eq 2 ]
then
    echo "Usase: dimershow.sh POSCAR[or CONTCAR] MODECAR[or NEWMODECAR]";
    exit;
fi



if [ -e mins ]
then
    rm mins -r;
fi


if [ -e $1 ] && [ -e $2 ]
then
    
    dimmins.pl $1 $2 1> /dev/null
    for i in min1 min2
    do
	cp mins/${i}/POSCAR ./CONTPOS
	toxyz.sh CONTPOS 2>/dev/null
    done;
    rm mins CONTPOS* -r
else
    echo "Cannot find $1 or $2"
fi

