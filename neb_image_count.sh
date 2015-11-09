for i in {0..99};
do
    
    this=$(printf "%02d" $i);
    
#	echo $this;
    if [ ! -d $this ]
    then
	count=$(echo "$this-1"|bc);
	echo $count;
	break;
    fi
done;
