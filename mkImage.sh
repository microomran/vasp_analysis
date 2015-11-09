for i in {1..{$1}}
do
xyz2vasp "Fe$i.xyz" vector > "Fe$i/POSCAR"
cd "Fe$i"
vasp2xyz1 POSCAR > 1.xyz
cd ..
done
