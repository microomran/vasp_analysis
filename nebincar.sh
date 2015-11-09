if [ $# != 1 ];
then
    echo "Usage : nebincar.sh [An INCAR file used for structrual relaxation]"
    exit
fi

for i in {0..99}
do
    name=$(printf "%02d\n" $i)
    if [ ! -d $name ];
    then
	break
    fi
done

if [ $i -le 2 ];
then
    echo "Must have at least 1 image"
    exit
fi

images=$(echo "$i-2"|bc)

echo "set IBRION 1 delete IOPT set POTIM 0.2 set NSW 1000 set ICHAIN 0 \
set SPRING -5 set LCLIMB TRUE set IMAGES $images show done" | INCARModifier $1