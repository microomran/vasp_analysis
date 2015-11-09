# arguments
# Name, Time, Node#, ppn#, Time
read -a elements
echo "#!/bin/bash";
echo "#PBS -N ${elements[0]}";
echo "#PBS -l walltime=${elements[1]}";
echo "#PBS -l nodes=${elements[2]}:ppn=${elements[3]}";
tail -n+5 ~/bin/make_runvasp.dat;

