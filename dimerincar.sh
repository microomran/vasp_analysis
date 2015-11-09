echo "set IBRION 3 set IOPT 2 set POTIM 0 set NSW 500 set ICHAIN 2 \
delete SPRING delete LCLIMB delete IMAGES \
set DRotMax 4 \
show done" | INCARModifier $1
