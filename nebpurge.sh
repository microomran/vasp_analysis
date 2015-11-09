imageCount=$(neb_image_count.sh);

i=1;
while [ $i -lt $imageCount ]
do
    this=$(printf "%02d" $i);
    
    echo $this;
    
    mv $this/POSCAR ./;
    rm $this/* -r;
    mv ./POSCAR $this/;
    
    i=`expr $i + 1`
done;

rm *.eps;
rm *.dat;
rm *.xyz;
rm movie*;
rm vasprun.xml;
#rm vasp;
rm pbs_out;
