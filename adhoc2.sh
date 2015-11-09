#k-points
echo "2 2 1"|make_kpoints.sh > KPOINTS

#fix surf
vasp2xyz1 01/POSCAR > 1.xyz
NATOM=$(head -1 1.xyz)
FIX=18
FLEX=$(echo "$NATOM-$FIX"|bc)
echo "F$FIX T$FLEX" > TF
rm 1.xyz

for i in {0..9};
  do
  if [ -d 0$i ];
      then
      echo $i;
      adjustTF 0$i/POSCAR TF > p
      mv p 0$i/POSCAR
  fi
done

#incar

echo "set ISPIN 2 show done"|INCARModifier INCAR > nI
mv nI INCAR
