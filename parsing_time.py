#!/usr/bin/env python
import sys;
input = sys.stdin.readline().split(":")
sections=[]
for i in input:
    sections.append(int(i))
print(sections)

count = len(sections)
if(count == 4):
    print(sections[0]*24 + sections[1] + sections[2] / 60.0)
elif(count==3):
    print(sections[0] + sections[1]/60.0)

