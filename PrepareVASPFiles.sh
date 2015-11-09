# Parameter 1: Notation Name for this run(will appear in runvasp.pbs)
# Parameter 2: Surface structure file POSCAR
# Parameter 3: Smallmol structure in xyz format
# Parameter 4: INCAR file
# Parameter 5: x,y,z offset for smallmol on surface (3 words)
# Parameter 6: x,y,z rotation for smallmol on surface (3 words)
# Parameter 7: kpoints used for this calculation (3 words)
# Parameter 8: fixed and flexible atoms count (smallmol is always in the former part of POSCAR, in Tx Fx Tx Fx format) 
# Parameter 9: Estimated running time, node# and ppn# (will appear in runvasp.pbs)

    casename=$1;

    SURF_POSCAR=$2;
    if [ ! -f $SURF_POSCAR ]
	then
	echo "Surface POSCAR file $SURF_POSCAR not exist";
	exit;
    fi

    vasp2xyz1 $SURF_POSCAR > $SURF_POSCAR.xyz;
    SURF_XYZ=$SURF_POSCAR.xyz;
    
    SMALLMOL=$3;
    if [ ! -f $SMALLMOL ]
	then
	echo "Small Molecule file $SMALLMOL not exist";
	exit;
    fi

    
    INCAR=$4;
    if [ ! -f $INCAR ]
	then
	echo "INCAR file $INCAR not exist";
	exit;
    fi
    
    KPOINTS=$7;

    TF=$8;

    if [ ! -d $casename ]
	then
	mkdir $casename;
    fi

# 1. Move adsorbed molecule on its position, relative to the lower left corner
#    atom on Cu surface.
    tmp=$5;
    xyzmove ${SMALLMOL} ${tmp[@]} > MovedSmallmol.xyz;
    tmp=$6;
    xyzrotate MovedSmallmol.xyz ${tmp[@]} > RotatedSmallmol.xyz; 


# 2. Sort the surface slab by layers, from top to buttom [ optional ]
    SORT_THE_SURF=0
    if [ $SORT_THE_SURF -eq 1 ]
    then 
	
    #    option for sort command: r: from top to down(descending);
    #    s:stable; k3:sort by third segment,i.e. z coord.
    #    g: general numerical value; Do not use -n,  it doesn't recogized scientific number such at 1e-5
	head -2 $SURF_XYZ > sortedSurf.xyz
	tail -n+3 $SURF_XYZ | sort -rgs -k4 >> sortedSurf.xyz
    else
    #   if you don't want to sort the surf    
	cp $SURF_XYZ sortedSurf.xyz
    fi


# 3. Combine the Smallmol and the surface to get a whole xyz file
    xyzcat sortedSurf.xyz MovedSmallmol.xyz > Combined.xyz

# 4. Convert the xyz file into a POSCAR
    xyz2vasp Combined.xyz $SURF_POSCAR > PrePOSCAR

# 5. Adjust the T/F flag, Smallmol first, then is the surface. Need to provide
#    the number of atoms in smallmol and in surface slab as parameters
    echo $TF > TFFile
    adjustTF PrePOSCAR TFFile > $casename/POSCAR

# 6. Make a correct POTCAR
    head -1 $casename/POSCAR | make_potcar.sh > $casename/POTCAR
    
# 7. Delete all temp files
    DEBUGGING=0
    if [ ! $DEBUGGING -eq 1 ]
	then
	rm MovedSmallmol.xyz
	rm RotatedSmallmol.xyz
	rm Combined.xyz
	rm PrePOSCAR
	rm TFFile
	rm sortedSurf.xyz
	rm $SURF_XYZ
    fi

# 8. Make a correct pbs script
    echo "$casename $9" | make_runvasp.sh > $casename/runvasp.pbs
    
# 9. Copy INCAR to destination
    cp $INCAR $casename/INCAR

# 11. KPOINTS
    echo $KPOINTS | make_kpoints.sh > $casename/KPOINTS;

# 12. monitor.in
    echo "RELAX_ONLY" > $casename/monitor.in

# 13. Start
    cd $casename
    pwd
    cd ..
