back(){
    mkdir ~/dump/$1;
    qcopy.sh $1 ~/dump/$1/
}

for i in $(showq|grep Running|grep $(whoami)|awk '{print $1}');
do
    back $i;
done
