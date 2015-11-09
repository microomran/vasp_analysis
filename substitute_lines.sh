if [ $# != 4 ]
then
    echo "Usage : substitue_line.sh src_file dest_file line_no_in_src line_no_in_dest"
    exit
fi

nminus1=$(echo "$4-1"|bc)
nplus1=$(echo "$4+1"|bc)

head -$nminus1 $2 > substitute_lines_temp_file
head -$3 $1 | tail -1 >> substitute_lines_temp_file
tail -n+$nplus1 $2 >> substitute_lines_temp_file
mv substitute_lines_temp_file $2