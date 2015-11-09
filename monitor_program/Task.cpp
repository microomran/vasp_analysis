#include "Task.h"
#include <algorithm>

void Task::SubmitARelaxJob(Job lastJob){
	Job newJob = lastJob;
	pCom->ShowMessage(this->Show()+"  Submit a Relax job according to last job : "+lastJob.Show());
	newJob.type = JOB_TYPE_RELAX;
	newJob.requiredStep = pCom->GrepNSWFromINCAR(incar);
	newJob.ReadNameAndTimeFromPbsScript(pCom,runvasppbs);
	newJob.WriteNameAndTimeToPbsScript(pCom,runvasppbs);
	int id = pCom->Qsub();
	newJob.jobID = id;
	pCom->AppendLineToFile(logFileName,newJob.Show());
	pCom->ShowMessage("New Relax job is : "+newJob.Show());
}

void Task::ContinueARelaxJob(Job lastJob){
	pCom->ShowMessage(this->Show()+"  Continue a Relax job according to last job : "+lastJob.Show());
	Job newJob = lastJob;
	newJob.type = JOB_TYPE_RELAX;
	// Need to estimate an sufficient running time:
	//double requiredTime = 
	//	lastJob.actualTime / 
	//	(double)(lastJob.actualStep+adjustment_factor_for_running_time) *
	//	(lastJob.requiredStep+adjustment_factor_for_running_time) * 
	//	safe_factor_for_running_time;
	
	// I have changed the way of estimating a new time based on previous running time
	// The new way is newTime = max( previous running time * spanRatio, previous requiredTime);
	double spanRatio = 1.20;
	newJob.requiredTime = (int)std::max( lastJob.requiredTime * 1.0 , lastJob.actualTime * spanRatio);

	newJob.WriteNameAndTimeToPbsScript(pCom,runvasppbs);
	int id = pCom->ContinueRelax();
	newJob.jobID = id;
	pCom->AppendLineToFile(logFileName,newJob.Show());
	pCom->ShowMessage("New Relax job is: "+newJob.Show());
}

void Task::SubmitAVibrationJob(Job lastJob){
	Job newJob = lastJob;
	pCom->ShowMessage(this->Show()+"  Submit a Vibration job according to last job : "+lastJob.Show());
	newJob.type = JOB_TYPE_VIBRATION;
	pCom->PrepareVibration();
	newJob.requiredStep = pCom->GrepFlexibleAtomsFromPOSCAR(poscar_vibration) * 6 + 1;
	newJob.ReadNameAndTimeFromPbsScript(pCom,runvasppbs);
	newJob.WriteNameAndTimeToPbsScript(pCom,runvasppbs_vibration);
	int id = pCom->SubmitVibration();
	newJob.jobID = id;
	pCom->AppendLineToFile(logFileName,newJob.Show());
	pCom->ShowMessage("New Vibration job is : "+newJob.Show());
}

void Task::ContinueAVibrationJob(Job lastJob){
	pCom->ShowMessage(this->Show()+"  Continue a Vibration job according to last job : "+lastJob.Show());
	Job newJob = lastJob;
	newJob.type = JOB_TYPE_VIBRATION;
	// Need to estimate an sufficient running time:
	double requiredTime = 
		lastJob.actualTime / 
		(double)(lastJob.actualStep+adjustment_factor_for_running_time) *
		(lastJob.requiredStep+adjustment_factor_for_running_time) * 
		safe_factor_for_running_time;
	newJob.requiredTime = (int)(requiredTime)+1;

	newJob.WriteNameAndTimeToPbsScript(pCom,runvasppbs_vibration);
	int id = pCom->SubmitVibration();
	newJob.jobID = id;
	pCom->AppendLineToFile(logFileName,newJob.Show());
	pCom->ShowMessage("New Vibration job is: "+newJob.Show());
}

string TaskRelaxOnly::Show(){
	return "Task-RelaxOnly";
}

int TaskRelaxOnly::Run(){
	string content = pCom->GetLastLineOfFile(logFileName);
	Job lastJob;
	lastJob.ReadFromRecord(content);
	bool running = lastJob.IsRunning(pCom);

	// Consider the following 5 situations: Running, no previous job, converged, non-converged, error and take different actions:
	// case 1: last job is running
	if(running){
		pCom->ShowMessage(this->Show()+"  Waiting or running job: "+lastJob.Show());
		return 0;
	}
	lastJob.CheckRunningResult(pCom);

	// case 2: no previous job found, may never start or killed by user or abnormal quit.
	if(lastJob.type==JOB_TYPE_NONE){
		SubmitARelaxJob(lastJob);
		return 0;
	}

	else if(lastJob.type==JOB_TYPE_RELAX){
		//case 3: last job converged, then show summary of last job and quit:
		bool converged = lastJob.IsConverged(pCom);
		if(converged){
			pCom->ShowMessage(this->Show()+"  Last Job Converged: "+lastJob.Show());
			return 0;
		}else{
		// case 4: last job not converged, may reached maximum time limit or step limit.
			ContinueARelaxJob(lastJob);
			return 0;
		}
	}
	else if(lastJob.type==JOB_TYPE_ERROR){
		// case 5: last job error, show summary of last job:
		pCom->ShowMessage(this->Show()+" Last Job ERROR! "+lastJob.Show());
		return 0;
	}
	else{
		// abnormal:
		cout<<"Fatal Error: Last job: \""<<lastJob.Show()<<"\" cannot appear in a "<<this->Show()<<" task"<<endl;
		exit(0);
	}
	
	return 0;
}

string TaskRelaxVibration::Show(){
	return "Task-RelaxVibration";
}
int TaskRelaxVibration::Run(){
	string content = pCom->GetLastLineOfFile(logFileName);
	Job lastJob;
	lastJob.ReadFromRecord(content);

	// Consider the following 5 situations: 
	// 1. No log history : submit a relax job
	// 2. Last job done: refuse to continue and report
	// 3. Last job error: refuse to continue and report
	// 4. Last job is running: wait and report:
	// 5. Last job is relaxing but:
	// 5(a). error(less than 1 ionic step) : refuse to continue and report
	// 5(b). cannot find a pbs_out file: submit the relax job again
	// 5(c). not converged : continue relax
	// 5(d). converged : create a vibration job (need to estimate the time).
	// 6. Last job is vibration but:
	// 6(a). vibration ended but not finished: vibrate again with longer time
	// 6(b). vibration ended with no pbs_out file: vibrate again
	// 6(c). vibration ended but error happened(less than 1 ionic step): refuse to continue and report
	// 6(d). vibration ended, all modes are real : done and report
	// 6(e). vibration ended, but with imaginary modes: push along the maximum imaginary mode and relax


	// 1.
	if(lastJob.type==JOB_TYPE_NONE){
		SubmitARelaxJob(lastJob);
		return 0;
	}
	// 2.
	else if(lastJob.type==JOB_TYPE_DONE){
		pCom->ShowMessage(this->Show()+"  Last Job Converged: "+lastJob.Show());
		return 0;
	}
	// 3.
	else if(lastJob.type==JOB_TYPE_ERROR){
		pCom->ShowMessage(this->Show()+" Last Job ERROR! "+lastJob.Show());
		return 0;
	}
	// 4.
	else{
		bool running = lastJob.IsRunning(pCom);
		if(running){
			pCom->ShowMessage(this->Show()+"  Waiting or running job: "+lastJob.Show());
			return 0;
		}
	}

	// Now job type can only be RELAX or VIBRATION
	// 5.
	if(lastJob.type == JOB_TYPE_RELAX){
		lastJob.CheckRunningResult(pCom);
		// 5(a).
		if(lastJob.type==JOB_TYPE_ERROR){
			pCom->ShowMessage(this->Show()+" Last Job ERROR! "+lastJob.Show());			
		}
		// 5(b).
		else if(lastJob.type==JOB_TYPE_NONE){
			SubmitARelaxJob(lastJob);
		}
		else{
			bool done = lastJob.IsConverged(pCom);
			// 5(d)
			if(done){
				SubmitAVibrationJob(lastJob);
			}else{
				// 5(c)
				ContinueARelaxJob(lastJob);
			}
		}
	}
	// 6.
	else if(lastJob.type == JOB_TYPE_VIBRATION){
		lastJob.CheckRunningResult(pCom);
		// 6(c).
		if(lastJob.type==JOB_TYPE_ERROR){
			pCom->ShowMessage(this->Show()+" Last Job ERROR! "+lastJob.Show());			
		}
		// 6(b).
		else if(lastJob.type==JOB_TYPE_NONE){
			SubmitAVibrationJob(lastJob);
		}else{
			bool done = lastJob.IsConverged(pCom);
			if(!done){
				// 6(a)
				ContinueAVibrationJob(lastJob);
				return 0;
			}
			bool real = pCom->AreAllModesReal();
			if(real){
				// 6(d)
				pCom->ShowMessage(this->Show()+"All modes are real, task done: "+lastJob.Show());
				return 0;
			}
			else{
				// 6(e)
				pCom->RelaxAfterVibrate();
				SubmitARelaxJob(lastJob);
			}
		}
	}
	// Abnormal.
	else{
		cout<<"Fatal Error: Last job: \""<<lastJob.Show()<<"\" cannot appear in a "<<this->Show()<<" task"<<endl;
		exit(0);
	}

	return 0;
}

string TaskNEB::Show(){
	return "NEB";
}
int TaskNEB::Run(){
	cout<<"TaskNEB"<<endl;
	return 0;
}



Task* TaskFactory::GenerateTask(string filename,CommandAdapter* pCmd){
	ifstream ifs(filename.c_str());
	if(!ifs){
		cout<<"Cannot open "<<filename<<endl;
		return NULL;
	}
	string s;
	ifs>>s;
	if(s=="RELAX_ONLY"){
		return new TaskRelaxOnly(TASK_TYPE_RELAX_ONLY,pCmd);
	}
	else if(s=="RELAX_VIBRATION"){
		return new TaskRelaxVibration(TASK_TYPE_RELAX_VIBRATION,pCmd);
	}
	else if(s=="NEB"){
		return new TaskNEB(TASK_TYPE_NEB,pCmd);
	}
	else{
		cout<<"Cannot recognize job type "<<s<<" in file "<<filename<<endl;
		return NULL;
	}
}

