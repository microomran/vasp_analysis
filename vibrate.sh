vibrate_nosubmit.sh $@
cd vib
qsub runvasp.pbs
cd ..
