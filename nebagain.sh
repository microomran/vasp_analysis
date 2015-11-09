imageCount=$(neb_image_count.sh);

i=1;
while [ $i -lt $imageCount ]
do
    this=$(printf "%02d" $i);
    
    echo $this;
    
    contcar2poscar $this/CONTCAR > $this/POSCAR;
    
    i=`expr $i + 1`
done;


nebpurge.sh