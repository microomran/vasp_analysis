#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <iomanip>
using namespace std;

int align(ifstream& new_poscar,ifstream& original_poscar){
  int i;
  string line_new;
  string line_original;
  vector<int> count_new;
  vector<int> count_original;

  for(int i=0;i<6;i++){
    getline(new_poscar,line_new);
    getline(original_poscar,line_original);
    cout<<line_new<<endl;
  }

  istringstream iss_new(line_new);
  istringstream iss_original(line_original);
  int atomCount;
  while(iss_new>>atomCount){
    count_new.push_back(atomCount);
  }
  while(iss_original>>atomCount){
    count_original.push_back(atomCount);
  }

  if(count_new.size() != count_original.size()){
    goto report_error;
  }

  atomCount = 0;
  for(i=0;i<count_new.size();i++){
    if(count_new[i] != count_original[i]){
    report_error:
      cout<<"Error: atom count in two files does not match!"<<endl;
      return -1;
    }
    atomCount += count_new[i];
  }

  for(int i=0;i<2;i++){
    getline(new_poscar,line_new);
    getline(original_poscar,line_original);
    cout<<line_new<<endl;
  }

  for(int i=0;i<atomCount;i++){
    for(int j=0;j<3;j++){
      double coord_new;
      double coord_original;
      new_poscar>>coord_new;
      original_poscar>>coord_original;
      while(fabs(coord_new-coord_original)>0.5){
	if(coord_new > coord_original){
	  coord_new -= 1.0;
	}
	else{
	  coord_new += 1.0;
	}
      }
      
      cout<<"  "<<fixed<<setw(9)<<setprecision(9)<<coord_new;
    }
    getline(new_poscar,line_new);   // Trailing part, like "T T T" or "F F F"
    getline(original_poscar,line_original); // Same as in the original poscar;
    cout<<line_new<<endl;
  }
  return 0;
}

int main(int argc,char* argv[]){
  if(argc!=3){
    cout<<"Usage: poscaralign new_poscar original_poscar"<<endl;
    return -1;
  }
  ifstream new_poscar(argv[1]);
  ifstream original_poscar(argv[2]);
  if(!new_poscar || !original_poscar){
    cout<<"Cannot open "<<argv[1]<<" or "<<argv[2]<<endl;
    return -1;
  }

  align(new_poscar,original_poscar);
  return 0;
}
    
