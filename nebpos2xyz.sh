

for i in {0..99};
  do
  j=`expr $i + 1`

  this=$(printf "%02d" $i);
  next=$(printf "%02d" $j);
#  echo -n $this; echo -n " "; echo $next;


  toxyz.sh $this/POSCAR
#  toxyz222.sh $this/POSCAR

  if [ ! -d $next ]
      then
      break;
  fi  
done;
