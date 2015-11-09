if [ ! $# -eq 2 ];
    then
    echo "Usage: xdat2xyz.sh XDATCAR POSCAR"
    exit;
fi

if [ -e tmpXDAT ]
then
    rm -r tmpXDAT;
fi

mkdir tmpXDAT
cp $1 tmpXDAT/XDATCAR
cp $2 tmpXDAT/POSCAR

cd tmpXDAT
count=$(xdat2pos)
i=1;
while [ $i -le $count ]
do
    vasp2xyz1 xdatpos$i > tmp;
    xyz2vasp tmp $2 > pos$i;
    rm tmp xdatpos$i;

    toxyz.sh pos$i;
    
    i=$(echo "$i+1"|bc);
done;
cd ..
#rm -r tmpXDAT
