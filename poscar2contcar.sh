line1=$(head -1 $1)
head -5 $1
echo "      $line1"
tail -n+6 $1
