#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

string stripFront(string s){
  for(int i=0;i<s.length();i++){
    if( ! isblank(s[i]) ){
      return s.substr(i,s.length()-i);
    }
  }
  return "";
}


int main(){

  ifstream ifs("XDATCAR");
  if (!ifs){
    cout<<"Cannot open XDATCAR"<<endl;
    return -1;
  }

  vector<string> lines;
  char buffer[256];
  while(ifs.getline(buffer,256)){
    lines.push_back(string(buffer));
  }
  
  //Atom count in 7th line
  istringstream atomCountString(lines[6]);
  int atomCount = 0;
  int count;
  while(atomCountString>>count){
    atomCount += count;
  }


  // Check total lines count is ok
  // 7 lines header, the rest should be (atomCount+1) lines for each poscar
  if( (lines.size()-7) % (atomCount+1) != 0 ){
    cout << "XDATCAR format error"<<endl;
    cout << "There should be ("<<atomCount<<"+1)*N+7 lines, but there are "
	 <<lines.size()<<" lines"<<endl;
    return -1;
  }


  int poscarCount = (lines.size()- 8)/(atomCount+1);
  cout<<poscarCount<<endl;

  for (int i=0;i<poscarCount;i++){
    ostringstream oss;
    oss<<"xdatpos"<<i+1;
    ofstream ofs(oss.str().c_str());
    
    ofs << stripFront(lines[5]) << endl;
    ofs << lines[1] << endl;
    ofs << lines[2] << endl;
    ofs << lines[3] << endl;
    ofs << lines[4] << endl;
    ofs << lines[6] << endl;
    ofs <<"Selective Dynamics"<<endl;
    ofs <<"Fractional"<<endl;

    for (int j=0;j<atomCount;j++){
      int pos = 7 + i*(atomCount+1) + j + 1;
      ofs << lines[pos]<<" F F F"<<endl;
    }
  }
  
  return 0;      
}

