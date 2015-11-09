vasp2xyz1 POSCAR > adjfree_nP
tail -n+9 POSCAR | awk '{print $4}' > adjfree_TF
head -4 POSCAR > adjfree_HEAD
echo "  0 0 $1" >> adjfree_HEAD
xyz2vasp adjfree_nP adjfree_HEAD > adjfree_prePOSCAR
adjustTF adjfree_prePOSCAR adjfree_TF
rm adjfree_nP adjfree_TF adjfree_HEAD adjfree_prePOSCAR
