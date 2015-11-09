if [ $# -ne 2 ]
then
    echo "Usage: scale_for_energy_plot.sh interval max_steps";
    exit;
fi;
interval=$1;
max=$2;
i=1.0;
continue_flag=1;
while [ $continue_flag -eq 1 ]
do
    echo $(echo "$i - $interval"|bc);
    echo $(echo "$i + $interval"|bc);
    i=$(echo  "$i+1"|bc);
    continue_flag=$(echo "$i<=$max"|bc);
done;

