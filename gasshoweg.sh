curDIR=$(pwd);
cd $1
GibbsFreeEnergy.py OSZICAR- vib/OUTCAR- 300 6
cd $curDIR