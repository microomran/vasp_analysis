#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;

enum INCARLineStatus{
  CHANGED,
  DELETED,
  NEW,
ORIGINAL
};

struct LineInINCAR{
  string line;
  string keyword;
  string value;
  enum INCARLineStatus status;
};

class INCAR{
  vector<LineInINCAR> lines;
public:
  int ReadINCAR(char* name);
  int WriteINCAR();
  int TakingCommand(istream& is);
  static bool Parseline(string line,string& key, string& value);
};

bool INCAR::Parseline(string line,string& key, string& value){
  unsigned int i;
  for(i=0;i<line.length();i++){
    if(line[i]=='#'){
      line = line.substr(0,i);
      break;
    }
  }
  string firstPart,secondPart;
  for(i=0;i<line.length();i++){
    if(line[i]=='='){
      firstPart = line.substr(0,i);
      secondPart = line.substr(i+1, line.length()-i);
      break;
    }
  }
  if(i==line.length()){
    return false;
  }

  istringstream firstPartIss(firstPart);
  //    istringstream secondPartIss(secondPart);
  key = "";
  value = "";
  firstPartIss>>key;
  // secondPartIss>>value;
  value = secondPart;
  if(key=="" || value=="" ){
    return false;
  }

  //cout<<line<<"|"<<firstPart<<"|"<<secondPart<<"|"<<key<<"|"<<value<<"|"<<endl;

  return true;
}

int INCAR::ReadINCAR(char* filename){
  fstream ifs(filename);
  if(!ifs){
    cout<<"Can't open "<<filename<<endl;
    return -1;
  }
  string line;
  while(getline(ifs,line)){
    string key,value;
    LineInINCAR newline = {line,"","",ORIGINAL};
    bool result = Parseline(line,key,value);
    if(result){
      newline.keyword = key;
      newline.value = value;
    }
    this->lines.push_back(newline);
  }

  return 0;
}

int INCAR::WriteINCAR(){
  unsigned int i;
  for(i=0;i<this->lines.size();i++){
    LineInINCAR& curLine = this->lines[i];
    switch(curLine.status){
    case ORIGINAL:
      cout<<curLine.line<<endl;
      break;
    case CHANGED:
    case NEW:
      cout<<curLine.keyword <<" = "<<curLine.value<<endl;
      break;
    case DELETED :
      cout<<"# "<<curLine.line<<endl;
      break;
    default:
      break;
    }
  }

  return 0;
}

int INCAR::TakingCommand(istream& is){
  string command;

  while(1){
    is>>command;
    if(command=="done"){
      return 0;
    }
    else if(command=="show"){
      this->WriteINCAR();
    }
    else if(command=="get"){
      unsigned int i;
      string key;
      is>>key;
      for(i=0;i<this->lines.size();i++){
	if(this->lines[i].keyword==key){
	  cout<<this->lines[i].value<<endl;
	  break;
	}
      }
      if(i==this->lines.size()){
	cout<<"No Key \""<<key<<"\" in INCAR"<<endl;
      }
    }
    else if(command=="set"){
      unsigned int i;
      int index;
      string key;
      is>>key;
      for(i=0;i<this->lines.size();i++){
	if(this->lines[i].keyword==key){
	  index = i;
	  this->lines[i].status=CHANGED;
	  break;
	}
      }
      // If can't find the key, append a new key
      if(i==this->lines.size()){
	LineInINCAR l = {"",key,"",NEW};
	this->lines.push_back(l);
	index = this->lines.size()-1;
      }

      // If the new key is quoted like set MAGMOG "0 0 4*0", then read until both quotes are read.
      string word;
      is>>word;
      if(word[0]=='\"'){
	string value = word;
	while(std::count(value.begin(),value.end(),'\"')!=2){
	  is>>word;
	  value += " " + word;
	}
	this->lines[index].value = value.substr(1,value.length()-2);
      }
      else{
	this->lines[index].value = word;
      }
    }

    else if(command=="delete"){
      unsigned int i;
      string key;
      is>>key;
      for(i=0;i<this->lines.size();i++){
	if(this->lines[i].keyword==key){
	  this->lines[i].status = DELETED;
	}
      }
      // If can't find the key, do nothing
    }
  }
  return 0;
}

int main(int argc,char* argv[]){

  if(argc!=2){
    cout<<"Usage : INCARModifier INCAR"<<endl;
    return -1;
  }
  INCAR incar;
  if( incar.ReadINCAR(argv[1]) == -1){
    return -1;
  }
  incar.TakingCommand(cin);

  /*
    // Used for Debugging
    INCAR incar;
    if( incar.ReadINCAR("INCAR") == -1){
    return -1;
    }
    // cout<<"Openning INCAR successful"<<endl;
    ifstream ifs("TestingCommand.txt");
    incar.TakingCommand(ifs);
  */
  return 0;
}
