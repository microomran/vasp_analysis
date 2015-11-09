#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <stdio.h>
using namespace std;
int main(int argc,char* argv[]){
  if ( argc-1 <=2 || argc%2 !=0 ){
      cout<<"Usage: multiplenebmake IS1 Image.count1 FS1(IS2) Image.count2 FS2(IS3),..."<<endl;
      cout<<"Parameters Count should be 2n+1 (at least 3)"<<endl;
      return -1;
  }
      
  int sectionCount = (argc-2)/2 ;

  vector<string> sectionIS;
  vector<string> sectionFS;
  vector<int> sectionImageCount;
  vector<int> sectionStart;
  vector<int> sectionEnd;
  for(int i=0;i<sectionCount;i++){
    int index = i*2+1;
    sectionIS.push_back(string(argv[index]));
    sectionFS.push_back(string(argv[index+2]));
    sectionImageCount.push_back(atoi(argv[index+1]));

    if(i==0){
      sectionStart.push_back(0);
    }else{
      sectionStart.push_back(sectionEnd[i-1]);
    }

    sectionEnd.push_back(sectionStart[i]+sectionImageCount[i]+1);
  }

  for(int i=0;i<sectionCount;i++){
    cout<<"section "<<i<<": "<<sectionIS[i]<<"-"<<sectionImageCount[i]<<"-"<<sectionFS[i]
	<<"   from "<<sectionStart[i]<<" to "<<sectionEnd[i]<<endl;
  }


  
  for(int i=sectionCount-1;i>=0;i--){
    char command[256];
    sprintf(command,"nebmake.pl %s %s %d",
	    sectionIS[i].c_str(),sectionFS[i].c_str(),sectionImageCount[i]);
    cout<<command<<endl;
    system(command);

    if(i==0){
      continue;
    }

    for(int j=sectionImageCount[i]+1;j>0;j--){
      sprintf(command,"mv %02d %02d",j,sectionStart[i]+j);
      cout<<command<<endl;
      system(command);
    }
  }


  return 0;
}

  
    
    
    
