
if [ -d run1 ]
then
    ORIGINAL=run1/POSCAR;
else
    ORIGINAL=POSCAR;
fi

alignvasp2xyz1.sh CONTCAR- $ORIGINAL > tmp1.xyz;
cat tmp1.xyz | grep -v Cu > tmp2.xyz
echo "$(cat tmp2.xyz | sed '/^\s*$/d' | wc -l) - 2 "|bc
tail -n+2 tmp2.xyz
rm tmp1.xyz tmp2.xyz
