path=$1
a=$(~/sqlite/sqlite3 ~/sqlite/job.db "select id from run where path='$path'")
echo -e "$a"
