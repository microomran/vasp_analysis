job_id=$1
a=$(~/sqlite/sqlite3 ~/sqlite/job.db "select path from run where id='$job_id'")
echo -e "$a"
cd "$a"
