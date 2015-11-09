totalCPU=$(showq|grep "processors"|awk '{print $6}');


echo "Running Jobs:"
showq|grep Running|awk '{print $2" "$4}' | sort > tmp1_task;
cat tmp1_task | awk '{print $1}' | sort -u > tmp2_user;
totalUser=$(cat tmp2_user | wc -l);
echo "Total CPU# = $totalCPU; Total User# = $totalUser" > tmp3_result; 
for i in $(cat tmp2_user)
do
    sum=0;
    for j in $(cat tmp1_task | grep $i | awk '{print $2}')
    do
	sum=$(echo "$sum+$j"|bc);
    done;
    
    echo "$sum"$'\t'"$(echo "scale=2;100*$sum/$totalCPU"|bc)%"$'\t'$i >> tmp3_result;
    
done;

cat tmp3_result | sort -nr ;
rm tmp1_task tmp2_user tmp3_result; 



echo "Waiting Jobs:"
echo "CUP_count  UserName"
showq|grep Idle|awk '{print $2" "$4" "$5}' | sort > tmp1_task;
cat tmp1_task | awk '{print $1}' | sort -u > tmp2_user;
totalUser=$(cat tmp2_user | wc -l);

for i in $(cat tmp2_user)
do
    sum=0;
    for j in $(cat tmp1_task | grep $i | awk '{print $2}')
    do
        sum=$(echo "$sum+$j"|bc);
    done;

    echo -n "$sum     " >> tmp3_result;
    echo "$i" >> tmp3_result;

done;

cat tmp3_result | sort -nr ;

rm tmp1_task tmp2_user tmp3_result; 
