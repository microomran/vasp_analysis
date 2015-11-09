#include "CmdAdapter.h"
using namespace std;


string CommandAdapter::GetLastLineOfFile(string filename){
	if(filename==tempFileName){
		cout<<"Error, File lock for "+tempFileName<<endl;
		exit(0);
	}
	string cmd = "tail -1 " + filename + " > " + tempFileName;
	system(cmd.c_str());
	ifstream ifs(tempFileName.c_str());
	string content;
	getline(ifs,content);
	ifs.close();
	cmd = "rm "+tempFileName;
	system(cmd.c_str());
	return content;
}

int CommandAdapter::AppendLineToFile(string filename,string strToAppend){
	string cmd = "echo " + strToAppend + " >> "+filename;
	system(cmd.c_str());
	return 0;
}
int CommandAdapter::LineNumberOfFile(string filename){
	if(filename==tempFileName){
		cout<<"Error, File lock for "+tempFileName<<endl;
		exit(0);
	}
	string cmd = "cat "+filename+" | wc -l >>"+tempFileName;
	system(cmd.c_str());
	ifstream ifs(tempFileName.c_str());
	int n;
	ifs>>n;
	ifs.close();
	cmd = "rm "+tempFileName;
	system(cmd.c_str());	
	return n;
}
int CommandAdapter::GrepIonicStep(string filename){
	// if return 0, then it cannot even complete 1 ionic step, or serious error happens.
	if(filename==tempFileName){
		cout<<"Error, File lock for "+tempFileName<<endl;
		exit(0);
	}
	string cmd = "cat "+filename+" | grep \" F=\" | wc -l > "+tempFileName;
	system(cmd.c_str());

	ifstream ifs(tempFileName.c_str());
	int ionicStep;
	ifs>>ionicStep;
	ifs.close();

	cmd = "rm "+tempFileName;
	system(cmd.c_str());
	return ionicStep;
}
int CommandAdapter::GrepRunningTime(string filename){
	// if return -1, then it does not contain time information.
	int minutes;
	if(filename==tempFileName){
		cout<<"Error, File lock for "+tempFileName<<endl;
		exit(0);
	}
	string cmd = "cat "+filename+" | grep \"^real\" > "+tempFileName;
	system(cmd.c_str());

	ifstream ifs(tempFileName.c_str());
	string s="";
	ifs>>s>>s;
	ifs.close();
	int pos = s.find('m');
	if(pos==-1){
		minutes = -1;
	}
	else{
		string substr = s.substr(0,pos);
		sscanf(substr.c_str(),"%d",&minutes);
		minutes++;
	}

	cmd = "rm "+tempFileName;
	system(cmd.c_str());
	return minutes;
}

bool CommandAdapter::FileContainsString(string filename,string toFind){
	if(filename==tempFileName){
		cout<<"Error, File lock for "+tempFileName<<endl;
		exit(0);
	}
	string cmd = "cat "+filename+" | grep \""+toFind+"\" | wc -l > "+tempFileName;
//	cout<<cmd<<endl;
	system(cmd.c_str());
	ifstream ifs(tempFileName.c_str());
	int i;
	bool contains;
	ifs>>i;
	if(i>0){
		contains = true;
	}else{
		contains = false;
	}
	ifs.close();
	cmd = "rm "+tempFileName;
	system(cmd.c_str());
	return contains;
}
void CommandAdapter::ShowMessage(string s){
	cout<<s<<endl;
}

int CommandAdapter::ReadFromPBSScript(string filename,int& time,string& name){
	ifstream ifs(filename.c_str());
	string s;
	ifs>>s>>s>>s;
	ifs>>name;
	if(s==""){
		return -1;
	}
	ifs>>s>>s>>s;
	string hour = s.substr(9,2);
	string min = s.substr(12,2);
	time = atoi(hour.c_str())*60 + atoi(min.c_str());
	ifs.close();
	if(time<=0){
		return -1;
	}
	
	return 0;
}
int CommandAdapter::WriteToPBSScript(string filename,int time,string name){
	if(filename==tempFileName){
		cout<<"Error! File lock on "<<filename<<endl;
		exit(0);
	}
	ofstream ofs(tempFileName.c_str());
	ofs<<"#!/bin/bash"<<endl;
	ofs<<"#PBS -N "<<name<<endl;
	char buf[32];
	sprintf(buf,"%02d:%02d:00",time/60, time%60);
	ofs<<"#PBS -l walltime="<<buf<<endl;
	ofs.close();

	string cmd = "tail -n+4 "+runvasppbs+" >> "+tempFileName;
	system(cmd.c_str());
	cmd = "rm "+runvasppbs;
	system(cmd.c_str());
	cmd = "mv "+tempFileName+" "+runvasppbs;
	system(cmd.c_str());
	return 0;
}

int CommandAdapter::GrepNSWFromINCAR(string incar){
	if(incar==tempFileName){
		cout<<"File lock for "+tempFileName<<endl;
		exit(0);
	}
	string cmd = "cat "+incar+" | grep NSW > "+tempFileName;
	system(cmd.c_str());

	ifstream ifs(tempFileName.c_str());
	string s;
	getline(ifs,s);
	int pos = s.find('=');
	if(pos==-1 || pos==s.length()-1){
		return -1;
	}
	s = s.substr(pos+1,s.length()-pos-1);
	istringstream iss(s);
	int nsw;
	iss>>nsw;
	
	ifs.close();
	cmd = "rm "+tempFileName;
	system(cmd.c_str());
	return nsw;
}

int CommandAdapter::GrepFlexibleAtomsFromPOSCAR(string poscar){
	if(incar==tempFileName){
		cout<<"File lock for "+tempFileName<<endl;
		exit(0);
	}
	string cmd = "cat "+poscar+" | grep \" T \" | wc -l > "+tempFileName;
	system(cmd.c_str());

	ifstream ifs(tempFileName.c_str());
	int count=-1;
	ifs>>count;
	ifs.close();

	cmd = "rm "+tempFileName;
	system(cmd.c_str());
	return count;
}

bool CommandAdapter::AreAllModesReal(){
	string cmd = "imgmodes.sh > "+tempFileName;
	system(cmd.c_str());

	ifstream ifs(tempFileName.c_str());
	int count=-1;
	ifs>>count;
	ifs.close();

	cmd = "rm "+tempFileName;
	system(cmd.c_str());
	
	if(count==0){
		return true;
	}
	else{
		return false;
	}
}

//////////////////////////////////////////////////////////
// Linux Adapter

bool LinuxAdapter::JobIsRunning(int jobID){
	bool found;
	char cmdstr[128];
	sprintf(cmdstr,"qstat | grep %d > %s",jobID,tempFileName.c_str());
	system(cmdstr);
	ifstream ifs(tempFileName.c_str());
	int check = -1;
	string b1,b2,b3,status="";
	ifs>>check>>b1>>b2>>b3>>status;
	if(check==jobID && status!="C"){
		found = true;
	}
	else{
		found = false;
	}
	ifs.close();

	sprintf(cmdstr,"rm %s",tempFileName.c_str());
	system(cmdstr);

	return found;
	/*  */
}
int LinuxAdapter::ContinueRelax(){
	string cmd = "continuerelax.sh > "+tempFileName;
	system(cmd.c_str());
	ifstream ifs(tempFileName.c_str());
	int jobID;
	ifs>>jobID;
	cmd = "rm "+tempFileName;
	system(cmd.c_str());
	return jobID;
}
int LinuxAdapter::Qsub(){
	string cmd = "qsub runvasp.pbs > "+tempFileName;
	system(cmd.c_str());
	ifstream ifs(tempFileName.c_str());
	int jobID;
	ifs>>jobID;
	cmd = "rm "+tempFileName;
	system(cmd.c_str());
	return jobID;
}
int LinuxAdapter::PrepareVibration(){
	string cmd = "vibrate_nosubmit.sh";
	system(cmd.c_str());
	return 0;
}
int LinuxAdapter::SubmitVibration(){
	string cmd;

	cmd = "vibrate_submit.sh "+tempFileName;
	system(cmd.c_str());

	ifstream ifs(tempFileName.c_str());
	int jobID;
	ifs>>jobID;
	cmd = "rm "+tempFileName;
	system(cmd.c_str());

	cmd = "cd ..";
	system(cmd.c_str());
	return jobID;
}
int LinuxAdapter::RelaxAfterVibrate(){
	string cmd = "relax_after_vib.sh";
	system(cmd.c_str());
	return 0;
}

///////////////////////////////////////////////////////////
//Windows Adapter
bool WindowsAdapter::JobIsRunning(int jobID){
	return false;
}

int WindowsAdapter::ContinueRelax(){
	cout<<"continuerelax.sh"<<endl;
	return 1234567;
}
int WindowsAdapter::Qsub(){
	cout<<"qsub runvasp.pbs"<<endl;
	return 1234567;
}
int WindowsAdapter::PrepareVibration(){
	string cmd = "vibrate_nosubmit.sh";
	cout<<cmd<<endl;
	return 0;
}
int WindowsAdapter::SubmitVibration(){
	string cmd;

	cmd = "cd vib";
	cout<<cmd<<endl;

	cmd = "qsub runvasp.pbs > "+tempFileName;
	cout<<cmd;

	int jobID = 1234567;

	cmd = "rm "+tempFileName;
	cout<<cmd<<endl;

	cmd = "cd ..";
	cout<<cmd<<endl;
	return jobID;
}
int WindowsAdapter::RelaxAfterVibrate(){
	string cmd = "relax_after_vib.sh";
	cout<<cmd;
	return 0;
}

