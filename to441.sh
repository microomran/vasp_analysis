continuenostart.sh
echo "4 4 1" | make_kpoints.sh > KPOINTS
echo "RELAX_VIBRATION" > monitor.in
rm monitor.log
