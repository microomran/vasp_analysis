for i in $(ls);do du -sch $i|grep -v total;done;
