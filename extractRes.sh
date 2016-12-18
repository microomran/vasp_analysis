energy=`grep F OSZICAR- | tail -n 1 | awk '{print $5}'`
mag=`grep F OSZICAR- | tail -n 1 | awk '{print $10}'`
printf "%f %f\n" $energy $mag
