#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

int contcar2poscar(ifstream& ifs){
  int i;
  char line[256];
  for(i=0;i<7;i++){
    ifs.getline(line,256);
    if(i!=5){
      cout<<line<<endl;
    }
  }
  istringstream lineOfNumbers(line);
  int totalAtomCount = 0;
  int atomCount;
  while(lineOfNumbers>>atomCount){
    totalAtomCount += atomCount;
  }
  
  for(i=0;i<totalAtomCount+2;i++){
    ifs.getline(line,256);
    cout<<line<<endl;
  }
  
  return 0;
}

int main(int argc,char* argv[]){
  if(argc!=2){
    cout<<"Usage: contcar2poscar CONTCAR"<<endl;
    return -1;
  }
  
  ifstream ifs(argv[1]);
  if(!ifs){
    cout<<"Cannot open "<<argv[1]<<endl;
    return -1;
  }
  else{
    contcar2poscar(ifs);
    return 0;
  }
}
