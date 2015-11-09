#CMD=toxyz222.sh
CMD=toxyz.sh


$CMD 00/POSCAR


for i in {1..99};
  do
  j=`expr $i + 1`

  this=$(printf "%02d" $i);
  next=$(printf "%02d" $j);

#  echo -n $this; echo -n " "; echo $next;
  if [ -d $next ]
      then
      $CMD $this/CONTCAR
  else
      $CMD $this/POSCAR
      break;
  fi  
done;
