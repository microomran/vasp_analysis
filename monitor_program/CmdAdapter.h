#ifndef CMD_ADAPTER
#define CMD_ADAPTER

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdlib.h>
using namespace std;

const string dir="./";

const string inFileName=dir+"monitor.in";
const string logFileName=dir+"monitor.log";
const string tempFileName=dir+"monitor_temp";
const string pbs_out_relax=dir+"pbs_out";
const string pbs_out_vibration=dir+"vib/pbs_out";
const string pbs_out_neb=dir+"pbs_out";
const string runvasppbs=dir+"runvasp.pbs";
const string runvasppbs_vibration=dir+"vib/runvasp.pbs";
const string incar=dir+"INCAR";
const string poscar_vibration=dir+"vib/POSCAR";

const int adjustment_factor_for_running_time = 5; 
//This first ionic step is equal to how many later ionic steps
// The smaller this factor, the larger the estimated required time will be.
const double safe_factor_for_running_time = 1.05;

class CommandAdapter{
public:
	string GetLastLineOfFile(string filename);
	int AppendLineToFile(string filename,string strToAppend);
	int LineNumberOfFile(string filename);
	int GrepIonicStep(string filename);
	int GrepRunningTime(string filename);
	bool FileContainsString(string filename,string toFind);
	void ShowMessage(string s);
	int ReadFromPBSScript(string filename,int& time,string& name);
	int WriteToPBSScript(string filename,int time,string name);
	int GrepNSWFromINCAR(string incar);
	int GrepFlexibleAtomsFromPOSCAR(string poscar);
	bool AreAllModesReal();
	virtual bool JobIsRunning(int jobID)=0;
	virtual int ContinueRelax()=0;
	virtual int Qsub()=0;
	virtual int PrepareVibration()=0;
	virtual int SubmitVibration()=0;
	virtual int RelaxAfterVibrate()=0;
};

class LinuxAdapter: public CommandAdapter{
public:
	bool JobIsRunning(int jobID);
	int ContinueRelax();
	int Qsub();
	int PrepareVibration();
	int SubmitVibration();
	int RelaxAfterVibrate();
};
class WindowsAdapter:public CommandAdapter{
public:
	bool JobIsRunning(int jobID);
	int ContinueRelax();
	int Qsub();
	int PrepareVibration();
	int SubmitVibration();
	int RelaxAfterVibrate();
};

#endif
