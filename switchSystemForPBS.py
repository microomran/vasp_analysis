pbsFile = [];
try:
    pbsFile = open("runvasp.pbs","r")
except IOError:
    print "Cannot open runvasp.pbs to read";

lines = pbsFile.readlines();
node = int(0)
ppn = int(0)
position =  int(0);
for i in xrange(0,len(lines)):
    if "nodes" in lines[i]:
        [s1,s2,s4] = lines[i].split("=");
        [s2,s3] = s2.split(":")
        node = int(s2);
        ppn = int(s4);
        position = i;
        break;
print ("node="+str(node)+":ppn="+str(ppn));
if (ppn == 12):
    print("Current mode is Oakley, switching to Glenn");
    ppn = 8;
    node = node * 2;
elif (ppn == 8):
    print("Current mode is Glenn, switching to Oakley");
    ppn = 12;
    node = node / 2;
else:
    print("Current mode unrecogonized, will do nothing");
    exit();

pbsFile.close();
try:
    pbsFile = open("runvasp.pbs","w")
except IOError:
    print "Cannot open runvasp.pbs to write";
    

newLine = "#PBS -l nodes="+str(node)+":ppn="+str(ppn);
print("New mode is");
print(newLine);
lines[position] = newLine+"\n";
for i in lines:
    pbsFile.write(i);


pbsFile.close();
