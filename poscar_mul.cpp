// This program transform a POSCAR file into a .xyz format file
// The usage is exactly the same as vasp2xyz and vasp2xyz3

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <stdlib.h>
#include <string.h>

using namespace std;

#define PRECISION 9

class Atom{
public:
  string element_type;
  double x;
  double y;
  double z;
  void Show() const;
  const Atom operator*(double ratio) const;
  const Atom operator+(const Atom& a) const;
};

void Atom::Show() const{
  //cout<<element_type<<"  ";
  cout<<setprecision(PRECISION)
      <<x<<"  "
      <<y<<"  "
      <<z;
}

const Atom Atom::operator*(double ratio) const{
  Atom newAtom;
  newAtom.x = x * ratio;
  newAtom.y = y * ratio;
  newAtom.z = z * ratio;
  return newAtom;
}

const Atom Atom::operator+(const Atom& a) const{
  Atom newAtom;
  newAtom.x = x + a.x;
  newAtom.y = y + a.y;
  newAtom.z = z + a.z;
  return newAtom;
}

struct AtomCmp{
  bool operator()(const Atom& a1,const Atom& a2){

    if(a1.element_type < a2.element_type){
      return true;
    }
    else if(a1.element_type > a2.element_type){
      return false;
    }
    else{
      return a1.z > a2.z;
    }
  }
};
  

class Poscar{
public:
  double scale;
  vector<Atom> surfVectors;
  vector<Atom> atoms;
  
  void Sort();//First Atom type, then Z desc, then X incre, Y incre 
  void Read(const char* filename);
  void Show();
  void Multiply(int fold[3]);
};

void Poscar::Sort(){
  /*
  for(vector<Atom>::iterator iter=atoms.begin();
      iter<atoms.end();++iter){
    (*iter).Show();cout<<endl;
  }
  */
  // atoms.sort();
  std::sort(atoms.begin(),atoms.end(),AtomCmp());
}

void Poscar::Multiply(int fold[3]){
  vector<Atom> newAtoms;
  
  if(fold[0] <=0 || fold[1] <=0 || fold[2] <= 0){
    return;
  }
  
  for(int i=0;i<atoms.size();i++){
  
    for(int x=0;x<fold[0];x++){
      for(int y=0;y<fold[1];y++){
	for(int z=0;z<fold[2];z++){
	  Atom a;
	  Atom offset;
	  offset.x = x;
	  offset.y = y;
	  offset.z = z;
	  a = atoms[i] + offset;

	  a.element_type = atoms[i].element_type;

	  newAtoms.push_back(a);
	}
      }
    }
  }
  
  atoms = newAtoms;
}
	

void Poscar::Read(const char* filename){
  int i;
  string line;
  int totalAtomCount;
  vector<string> types;
  vector<int> typesCount;
  
  this->atoms.clear();
  this->surfVectors.clear();
  

  ifstream ifs(filename);
  if(!ifs){
    cout<<"Cannot open "<<filename<<endl;
    return;
  }
  
  
  if(getline(ifs,line)){
    istringstream iss(line);
    string element;
    while(iss>>element){
      types.push_back(element);
    }
  }

  ifs>>this->scale;
  
  for(i=0;i<3;i++){
    Atom v;
    ifs>>v.x>>v.y>>v.z;
    this->surfVectors.push_back(v);
  }
  
  while(getline(ifs,line)){
    istringstream iss(line);
    int count;
    totalAtomCount = 0;
    while(iss>>count){
      typesCount.push_back(count);
      totalAtomCount += count;
    }
    if(totalAtomCount > 0){
      break;
    }
  }
  
  getline(ifs,line);
  getline(ifs,line);
  
  for(i=0;i<totalAtomCount;i++){
    getline(ifs,line);
    istringstream iss(line);
    Atom a;
    iss>>a.x>>a.y>>a.z;
    this->atoms.push_back(a);
  }


  unsigned int atom_index,type_index;
  atom_index = 0;
  type_index = 0;
  for(atom_index=0; atom_index< this->atoms.size(); atom_index++){
    this->atoms[atom_index].element_type = types[type_index];
    typesCount[type_index] --;
    if(typesCount[type_index]==0){
      type_index++;
    }
  }
}

void Poscar::Show(){
  int i;
  vector<string> types;
  vector<int> counts;
  for(i=0;i<atoms.size();i++){
    if(types.empty() || atoms[i].element_type != types[types.size()-1]){
      types.push_back(atoms[i].element_type);
      counts.push_back(1);
    }
    else{
      counts[counts.size()-1] ++;
    }
  }
  
  for(i=0;i<types.size();i++){
    cout<<types[i]<<"  ";
  }
  cout<<endl;

  cout<<"  "<<scale<<endl;
  
  for(i=0;i<3;i++){
    cout<<"  ";
    cout<<surfVectors[i].x<<"  "
	<<surfVectors[i].y<<"  "
	<<surfVectors[i].z<<endl;
  }
  cout<<"  ";
  for(i=0;i<counts.size();i++){
    cout<<counts[i]<<" ";
  }
  cout<<endl;
  cout<<"Selective Dynamics"<<endl
      <<"Fractional"<<endl;

  for(i=0;i<atoms.size();i++){
    cout<<"  ";
    atoms[i].Show();
    cout<<endl;
  }
}

int main(int argc,char* argv[]){
  if(argc!=5){
    cout<<"Usage: vasp2xyzn POSCAR [multiple times in x, y, z]"<<endl;
    cout<<"For example \"vasp2xyzn POSCAR 3 3 2\" with multiply the image 3,3, and 2 times in x,y and z direction."<<endl;
    return -1;
  }
  else{
    Poscar poscar;
    int multiple[3];
    
    for(int i=0;i<3;i++){
      multiple[i] = atoi(argv[i+2]);
    }
    
    
    poscar.Read(argv[1]);
    //poscar.Show();
    poscar.Multiply(multiple);
    //poscar.Show();
    poscar.Sort();
    poscar.Show();
    //WriteXYZ(types,typesCount,scale,surfVectors,atoms);
    
    return 0;
  }
}
