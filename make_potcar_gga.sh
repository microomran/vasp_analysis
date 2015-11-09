read -a elements
POTCAR_DIR=~/Shared/potcar
for i in ${elements[@]}
  do
  if [ -d ${POTCAR_DIR}/PAW/GGA/$i ];
      then
      cat ${POTCAR_DIR}/PAW/GGA/$i/POTCAR
  else
      echo "Error, Can't find $i"
      exit
  fi
done

