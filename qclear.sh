# clear files in a vasp run, to start a new run
# parameters
#	1. remove all *-, pbs_out
#	2. remove OSICAR*, OUTCAR*, pbs_out, PCDAC, CONTCAR , rename CONTCAR- to POSCAR, WAVECAR- to WAVECAR, CHG- to CHG
#	   CHGCAR- to CHGCAR
#	3. remove CHG*, WAVECAR*
if [ $# -eq 0 ];then
echo "1. remove all *-, pbs_out
      2. remove OSICAR*, OUTCAR*, pbs_out, PCDAC, CONTCAR , rename CONTCAR- to POSCAR, WAVECAR- to WAVECAR, CHG- to CHG
          CHGCAR- to CHGCAR
      3. remove CHG*, WAVECAR*";
exit
fi
if [ $1 -eq 1 ];then
  rm -f *-
  rm -f pbs_out
  rm -f CONTCAR OSZICAR OUTCAR PCDAT XDATCAR
fi
if [ $1 -eq 2 ];then
  rm -f OUTCAR* OSZICAR* pbs_out CONTCAR
  mv CHG- CHG
  mv CHGCAR- CHGCAR
  mv WAVECAR- WAVECAR
  mv CONTCAR- POSCAR
  rm -f vasprun.xml PCDAT XDATCAR* DOSCAR* PROCAR*
fi
if [ $1 -eq 3 ];then
  rm -f CHG*  WAVECAR*
fi

