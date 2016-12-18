inprocess=`qstat -u osu9577 | wc -l`
if [ $inprocess -gt 5 ];then
   anlList.sh ~/oldlist
fi
