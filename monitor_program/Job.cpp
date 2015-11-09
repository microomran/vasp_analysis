#include "Job.h"
using namespace std;



int Job::ReadFromRecord(string record){
	istringstream iss(record);
	string typeName="";
	iss>>typeName;
	if(typeName==""){
		type = JOB_TYPE_NONE;
	}
	else if(typeName=="RELAX"){
		type = JOB_TYPE_RELAX;
	}
	else if(typeName=="VIBRATION"){
		type = JOB_TYPE_VIBRATION;
	}
	else if(typeName=="NEB"){
		type = JOB_TYPE_NEB;
	}
	else if(typeName=="ERROR"){
		type = JOB_TYPE_ERROR;
	}
	else if(typeName=="DONE"){
		type = JOB_TYPE_DONE;
	}
	else{
		cout<<"Unrecognized job type "<<type<<endl;
		exit(0);
	}
	iss>>jobID>>requiredTime>>requiredStep>>name;
	return 0;
}
string Job::Show(){
	ostringstream oss;
	switch(type){
		case JOB_TYPE_NONE:
			oss<<"NONE";break;
		case JOB_TYPE_RELAX:
			oss<<"RELAX";break;
		case JOB_TYPE_VIBRATION:
			oss<<"VIBRATION";break;
		case JOB_TYPE_NEB:
			oss<<"NEB";break;
		case JOB_TYPE_ERROR:
			oss<<"ERROR";break;
		case JOB_TYPE_DONE:
			oss<<"DONE";break;
		default:
			cout<<"Error, Wrong Job Type "<<type<<endl;
			exit(0);
	}
	oss<<"  ";
	oss<<jobID<<"  ";
	oss<<requiredTime<<"  ";
	oss<<requiredStep<<"  ";
	oss<<name;
	return oss.str();
}
string Job::WriteToRecord(){
	ostringstream oss;
	switch(type){
			case JOB_TYPE_RELAX:
				oss<<"RELAX ";break;
			case JOB_TYPE_VIBRATION:
				oss<<"VIBRATION ";break;
			case JOB_TYPE_NEB:
				oss<<"NEB ";break;
			case JOB_TYPE_DONE:
				oss<<"DONE ";break;
			default:	
				oss<<"ERROR ";break;
	}
	oss<<jobID<<" "<<requiredTime<<" "<<requiredStep<<" "<<name;
	return oss.str();
}

int Job::ReadNameAndTimeFromPbsScript(CommandAdapter* pCmd,string filename){
	return pCmd->ReadFromPBSScript(filename,this->requiredTime,this->name);
}

int Job::WriteNameAndTimeToPbsScript(CommandAdapter* pCmd,string filename){
	return pCmd->WriteToPBSScript(filename,this->requiredTime,this->name);
}

bool Job::IsRunning(CommandAdapter* pAdapter){
	bool running = pAdapter->JobIsRunning(jobID);
	return running;
}
int Job::CheckRunningResult(CommandAdapter* pAdapter){
	// This function checks the running result of last job. It works only when this->type==RELAX,VIBRATION,NEB
	// and when the job is not running now.
	// It is based on "pbs_out" file, based on running type, pbs_out is in different location. Main purpose of 
	// this function is to determine the actual excuted steps and running time. In the following 2 cases, the 
	// type will be reset as ERROR to indicate something's going run:
	// 1. pbs_out file does not contain step information or the last job fails to complete even 1 ionic step.
	// 2. pbs_out file does not contain running time information.
	// In the following case, the type will be reset as NONE to indicate nothing happend or the job has been killed
	// by user, so what it can do is simply repeat last job:
	// 1. Cannot find a pbs_out file (line number of pbs_out==0)
	string pbs_out_file_name;
	switch(type){
			case JOB_TYPE_RELAX:
				pbs_out_file_name = pbs_out_relax;break;
			case JOB_TYPE_VIBRATION:
				pbs_out_file_name = pbs_out_vibration;break;
			case JOB_TYPE_NEB:
				pbs_out_file_name = pbs_out_neb;
			default:
				return 0;
	}
	int lineNumberOfPbs_out = pAdapter->LineNumberOfFile(pbs_out_file_name);
	int ionicSteps = pAdapter->GrepIonicStep(pbs_out_file_name);
	int runningTime = pAdapter->GrepRunningTime(pbs_out_file_name);

	if(lineNumberOfPbs_out==0){
		this->type = JOB_TYPE_NONE;
		return -1;
	}

	if(ionicSteps==0){
		this->type = JOB_TYPE_ERROR;
		return -1;
	}else{
		this->actualStep = ionicSteps;
	}

	if(runningTime<=0){
		this->type = JOB_TYPE_ERROR;
		return -1;
	}else{
		this->actualTime = runningTime;
	}

	return 0;
}

bool Job::IsConverged(CommandAdapter* pAdapter){
	// Test whether the job is converged, only applicable when Type==RELAX, VIBRATION, NEB, and the criteria is different:

	bool converged;
	// Only applicable when for Type==RELAX, VIBRATION, NEB
	switch(type){
		// For relaxation only job, it is converged when pbs_out contains "reached required accuracy"
		case JOB_TYPE_RELAX:
			converged = pAdapter->FileContainsString(pbs_out_relax.c_str(),"reached required accuracy");
			break;
		// For vibrational job, it is converged when actaul running step equals to required step, which is 6N+1: 
		case JOB_TYPE_VIBRATION:
			if(requiredStep == actualStep){
				converged = true;
			}else{
				converged = false;
			}
			break;
		// For NEB, same as relax only:
		case JOB_TYPE_NEB:
			converged = pAdapter->FileContainsString(pbs_out_relax.c_str(),"reached required accuracy");
			break;
		default:
			cout<<"Error, this type of job : "<<type<<" cannot be converged."<<endl;
			exit(0);
	}
			
	return converged;
}
