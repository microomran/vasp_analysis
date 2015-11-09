if [ $# -le 2 ]
then
    echo "Usage : nebtsvib.sh [directory for FS or IS relax] [directory for TS, eg. 03] [atom indexes to vib]"
    exit
fi

if [ ! -d $1 ]
then
    echo "$1 not exist"
    exit
fi

if [ ! -d $2 ]
then
    echo "$2 not exist"
    exit
fi

if [ -e TSvib ]
then
    rm TSvib -r
fi

cp $1 -r TSvib
for i in OSZICAR OUTCAR CONTCAR
do
    cp $2/$i TSvib/$i-
done

shift
shift

cd TSvib
vibrate_nosubmit.sh $@
cd ..




