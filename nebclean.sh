for i in {0..9}
do
  for j in {0..9}
  do
    if [ -d $i$j ]
	then
	cd $i$j
	rm CHG CHGCAR WAVECAR
	cd ..
    fi
  done
done
rm vasp
