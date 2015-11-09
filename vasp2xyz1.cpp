// This program transform a POSCAR file into a .xyz format file
// The usage is exactly the same as vasp2xyz and vasp2xyz3

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>

using namespace std;

#define PRECISION 9

typedef struct{
	string element_type;
	double x;
	double y;
	double z;
} Atom;


int AtomShow(Atom* pAtom){
	cout<<setprecision(PRECISION)
		<<pAtom->x<<"  "
		<<pAtom->y<<"  "
		<<pAtom->z;
	return 0;
}


int ReadPOSCAR(const char* filename,vector<string>& types,vector<int>& typesCount, double& scale,vector<Atom>& surfVectors,vector<Atom>& atoms){
	int i;
	string line;
	int totalAtomCount;

	ifstream ifs(filename);
	if(!ifs){
		cout<<"Cannot open "<<filename<<endl;
		return -1;
	}


	if(getline(ifs,line)){
		istringstream iss(line);
		string element;
		while(iss>>element){
			types.push_back(element);
		}
	}

	ifs>>scale;

	for(i=0;i<3;i++){
		Atom v;
		ifs>>v.x>>v.y>>v.z;
		surfVectors.push_back(v);
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
		atoms.push_back(a);
	}

	return 0;
}

int WriteXYZ(vector<string> types,vector<int> typesCount,double scale,vector<Atom> surfVectors,vector<Atom> atoms){
	// determine each atom's type:
	unsigned int i,atom_index,type_index;
	atom_index = 0;
	type_index = 0;
	for(atom_index=0; atom_index<atoms.size(); atom_index++){
		atoms[atom_index].element_type = types[type_index];
		typesCount[type_index] --;
		if(typesCount[type_index]==0){
			type_index++;
		}
	}

	// Calculate each atom's cartisian coordiantes
	for(atom_index=0;atom_index<atoms.size();atom_index++){
		Atom* pAtom = &atoms[atom_index];
		double newCoord[3];
		newCoord[0] = pAtom->x*surfVectors[0].x + pAtom->y*surfVectors[1].x + pAtom->z*surfVectors[2].x;
		newCoord[1] = pAtom->x*surfVectors[0].y + pAtom->y*surfVectors[1].y + pAtom->z*surfVectors[2].y;
		newCoord[2] = pAtom->x*surfVectors[0].z + pAtom->y*surfVectors[1].z + pAtom->z*surfVectors[2].z;
		pAtom->x = newCoord[0]*scale;
		pAtom->y = newCoord[1]*scale;
		pAtom->z = newCoord[2]*scale;
	}

	// Write
	cout<<atoms.size()<<endl;
	cout<<"Created by vasp2xyz1"<<endl;
	for(i=0;i<atoms.size();i++){
		cout<<atoms[i].element_type<<"     ";
		AtomShow(&atoms[i]);
		cout<<endl;
	}
	return 0;
}




int main(int argc,char* argv[]){
	if(argc!=2){
		cout<<"Usage: vasp2xyz1 POSCAR"<<endl;
		return -1;
	}
	else{
		vector<string> types;
		vector<int> typesCount;
		double scale;
		vector<Atom> surfVectors;
		vector<Atom> atoms;

		ReadPOSCAR(argv[1],types,typesCount,scale,surfVectors,atoms);
		WriteXYZ(types,typesCount,scale,surfVectors,atoms);

		return 0;
	}
}
