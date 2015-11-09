# $1 is CONTCAR file, must be the CONTCAR of last time relaxtion
# $2 is OUTCAR file, must be OUTCAR when doing vibrational modes calc.
# $3 is index of the largest imaginary vibrational mode

CONTCAR=$1
OUTCAR=$2
MODE=$3
SCALE=0.5

COMMAND=toxyz.sh

pushalongeigenvector.sh $CONTCAR $OUTCAR $MODE $SCALE > tmp_p1
pushalongeigenvector.sh $CONTCAR $OUTCAR $MODE 0 > tmp_p2
pushalongeigenvector.sh $CONTCAR $OUTCAR $MODE -$SCALE > tmp_p3
$COMMAND tmp_p1 > tmpxyz_p1.xyz
$COMMAND tmp_p2 > tmpxyz_p2.xyz
$COMMAND tmp_p3 > tmpxyz_p3.xyz
cat tmpxyz_p1.xyz tmpxyz_p2.xyz tmpxyz_p3.xyz
rm tmp_p1 tmp_p2 tmp_p3
rm tmpxyz_p1.xyz tmpxyz_p2.xyz tmpxyz_p3.xyz
