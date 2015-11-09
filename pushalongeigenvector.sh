# $1 is CONTCAR file, must be the CONTCAR of last time relaxtion
# $2 is OUTCAR file, must be OUTCAR when doing vibrational modes calc.
# $3 is index of the largest imaginary vibrational mode
# $4 is the portion of eigenvalue that has been pushed, OPTIONAL, default is 1.0

CONTCAR=$1
OUTCAR=$2
MODE=$3
if [ -z $4 ]
    then    
    SCALE=1.0
else
    SCALE=$4
fi

# 1. Convert original CONTCAR into xyzfile, count atoms
contcar2poscar $CONTCAR > CONTPOS;
# Alternative for cases where CONTCAR is in same format of POSCAR
#cp $CONTCAR CONTPOS;
vasp2xyz1 CONTPOS > CONTPOS.xyz;
ATOMCOUNT=$(head -1 CONTPOS.xyz)

# 2. Fetch the eigenvector, and turn it into a xyz file
cat $OUTCAR | grep " $MODE f" -A $(expr $ATOMCOUNT + 1) | awk '{print "ET  "  $4  " "  $5  " "  $6}' > EigenVector
echo $ATOMCOUNT > EigenVector.xyz
echo "Comment"  >> EigenVector.xyz
tail -n+3 EigenVector >> EigenVector.xyz

# 2*(OPTIONAL) scale the eigenvector
xyzscale EigenVector.xyz $SCALE > EigenVectorScaled.xyz

# 3. Push original atoms along eigenvector
xyzadd CONTPOS.xyz EigenVectorScaled.xyz > Moved.xyz

# 4. Fetch the original T/F scheme and apply to new POSCAR
xyz2vasp Moved.xyz CONTPOS > POSCAR_moved_woTF
tail -n+9 CONTPOS | awk '{print $4}' > TFFile
adjustTF POSCAR_moved_woTF TFFile

# 5. Output on screen 

# 6.Delete temp files
for tempfile in CONTPOS CONTPOS.xyz EigenVector EigenVector.xyz EigenVectorScaled.xyz Moved.xyz POSCAR_moved_woTF TFFile;
do
    rm $tempfile
done
