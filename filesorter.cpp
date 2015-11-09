// Originally designed to recognize POSCAR, CONTCAR and xyz files,
// However this is a simplfied version only able to distinguish between
// POSCAR and CONTCAR
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

enum FileType{
  POSCAR,
  CONTCAR,
  XYZ,
  UNKNOWN
};

FileType fileSorter(vector<vector<string> > fileContent){
  if(fileContent.size()>=8 && fileContent[7].size()==1){
    return POSCAR;
  }
  else if(fileContent.size()>=9 && fileContent[8].size()==1){
    return CONTCAR;
  }
  else{
    return UNKNOWN;
  }
}

string FileTypeToString(FileType type){
  switch(type){
  case POSCAR:
    return "POSCAR";
  case CONTCAR:
    return "CONTCAR";
  case UNKNOWN:
    return "UNKNOWN";
  default:
    return "";
  }
}

int main(int argc, char* argv[]){
  if(argc != 2){
    cout<<"Usage: filesorter <File Name>"<<endl;
    return -1;
  }
  
  FileType fileType;
  
  ifstream ifs(argv[1]);
  if(!ifs){
    cout<<"Cannot open "<<argv[1]<<endl;
    return -1;
  }

  vector<vector<string> > fileContent;
  
  string line;
  while(getline(ifs,line)){
    vector<string> words;
    istringstream iss(line);
    string word;
    while(iss>>word){
      words.push_back(word);
    }
    fileContent.push_back(words);
  }

  ifs.close();

  /*  
  for(int i=0;i<fileContent.size();i++){
    for(int j=0;j<fileContent[i].size();j++){
      cout<<fileContent[i][j]<<" ";
    }
    cout<<endl;
  }
  */
  
  cout<<FileTypeToString(fileSorter(fileContent));

  return 0;
}
