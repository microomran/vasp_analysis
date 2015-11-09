#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <set>
#include <stdlib.h>
using namespace std;

int CheckAtomIndex(int i, int total){
  if(i<=0 || i>total){
    cout<<"Atom index "<<i<<" is beyond total atom count "<<total<<endl;
    exit(0);
  }
  return 0;
}


int main(int argc, char* argv[]){

  vector<int> atomEachType;
  int i;
  int max = -1;
  int totalAtomCount = 0;
  while(cin>>i){
    totalAtomCount += i;
    atomEachType.push_back(i);
    if(i>max){
      max = i;
    }
    
  }



  set<int> flexibleAtom;
  for(int i=1;i<argc;i++){
    int j;
    for(j=0;j<strlen(argv[i]);j++){
      if(argv[i][j]=='-'){
	break;
      }
    }
    if(j==strlen(argv[i])){
      CheckAtomIndex(atoi(argv[i]),totalAtomCount);
      flexibleAtom.insert(atoi(argv[i]));
    }
    else{
      int start = atoi(argv[i]);
      int end = atoi(argv[i]+j+1);
      if(start>end){
	cout<<"Atom index range "<<argv[i]<<" is invalid"<<endl;
	exit(0);
      }
      for(int k=start;k<=end;k++){
	CheckAtomIndex(k,totalAtomCount);
	flexibleAtom.insert(k);
      }
    }
  }

  if(flexibleAtom.empty()){
    
    for(i=0;i<atomEachType.size();i++){
      int c = atomEachType[i];
      if(c==max){
	cout<<"F"<<c<<" ";
      }
      else{
	cout<<"T"<<c<<" ";
      }
    }
  }
  else{
    for(i=1;i<=totalAtomCount;i++){
      if(flexibleAtom.find(i)!=flexibleAtom.end()){
	cout<<"T ";
      }
      else{
	cout<<"F ";
      }
    }
  }

  return 0;
}
  
