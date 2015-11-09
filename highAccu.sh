qclear.sh 2
sed -i "4s/.*/ISTART = 1 /" INCAR
sed -i "5s/.*/ICHARG = 1 /" INCAR
sed -i "11s/.*/EDIFF = 1e-5 /" INCAR
grep ISTART INCAR
grep ICHARG INCAR
grep EDIFF INCAR
echo "success"
qsub1.sh
