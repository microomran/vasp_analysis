cat vib/OUTCAR- | grep THz > allmodes_temp;
n=$(cat allmodes_temp | wc -l);
if [ "$n" == 0 ];
    then
    echo "-1";
else
    cat vib/OUTCAR- | grep THz | grep "/i" | wc -l;
fi
rm allmodes_temp;
