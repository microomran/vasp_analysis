#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <sstream>
using namespace std;

map<string,string> incar;

bool Parseline(string line,string& key, string& value){
  int i;
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
  //	istringstream secondPartIss(secondPart);
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

int ReadINCAR(char* filename){
  fstream ifs(filename);
  if(!ifs){
    cout<<"Cannot open "<<filename<<endl;
    return -1;
  }
  string line;
  while(getline(ifs,line)){
    string key,value;
    bool result = Parseline(line,key,value);
    if(result){
      incar[key] = value;
    }
  }

  return 0;
}

int WriteINCAR(){
  for(map<string,string>::iterator itr = incar.begin(); itr!=incar.end(); ++itr ){
    cout<<itr->first<<" = "<<itr->second<<endl;
  }
  return 0;
}

int TakingCommand(){
  string command;
  while(1){
    cin>>command;
    if(command=="done"){
      return 0;
    }
    else if(command=="show"){
      WriteINCAR();
    }
    else if(command=="get"){
      string key;
      cin>>key;
      cout<<incar[key]<<endl;
    }
    else if(command=="set"){
      string key,value;
      cin>>key>>value;
      incar[key] = value;
    }
    else if(command=="delete"){
      string key;
      cin>>key;
      map<string,string>::iterator itr = incar.find(key);
      if(itr!=incar.end()){
	incar.erase(itr);
      }
    }
  }
  return 0;
}

int main(int argc,char* argv[]){
  if(argc!=2){
    cout<<"Usage : INCARModifier INCAR"<<endl;
    return -1;
  }
  if( ReadINCAR(argv[1]) == -1){
    return -1;
  }

  TakingCommand();

  return 0;
}
