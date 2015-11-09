for i in $(cat $1/vib/OUTCAR- |grep THz|awk '{print $8}');
do 
    echo "$i"|python -c "import sys;sys.stdout.write(\"%.0f \"%round(float(raw_input())))"
done
