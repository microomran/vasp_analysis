pbsFile = [];
try:
    pbsFile = open("runvasp.pbs","r")
except IOError:
    print "Cannot open runvasp.pbs to read";

lines = pbsFile.readlines();
hh = int(0)
mm = int(0)
ss = int(0)
position =  int(0);
for i in xrange(0,len(lines)):
    if "walltime" in lines[i]:
        [s1,s2] = lines[i].split("=");
        [hh,mm,ss] = s2.split(":")
        hh = int(hh);
        mm = int(mm);
        ss = int(ss);
        position = i;
        break;
pbsFile.close();

print ("walltime="+str(hh)+":"+str(mm)+":"+str(ss));

totalMin = hh*60 + mm;
scale = input();
totalMin = int(totalMin * float(scale));

hh = totalMin/60;
mm = totalMin%60;


try:
    pbsFile = open("runvasp.pbs","w")
except IOError:
    print "Cannot open runvasp.pbs to write";
    

newLine = "#PBS -l walltime="+str(hh)+":"+str(mm)+":"+str(ss);
print("New time is");
print(newLine);
lines[position] = newLine+"\n";
for i in lines:
    pbsFile.write(i);


pbsFile.close();
