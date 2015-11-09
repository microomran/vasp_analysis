OUTCAR=$1
ATOMCOUNT=$2
MODE=$3
echo $ATOMCOUNT 
echo "Comment"  
cat $OUTCAR | grep "$MODE f" -A $(expr $ATOMCOUNT + 1) | awk '{print "ET  "  $4  " "  $5  " "  $6}' | tail -n+3
