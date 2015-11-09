#ifndef JOB
#define JOB

#include "CmdAdapter.h"


enum JobType{
	JOB_TYPE_NONE,
	JOB_TYPE_RELAX,
	JOB_TYPE_VIBRATION,
	JOB_TYPE_NEB,
	JOB_TYPE_ERROR,
	JOB_TYPE_DONE
};

class Job{
public:
	JobType type;
	int jobID;
	int requiredTime;
	int requiredStep;
	int actualTime;
	int actualStep;
	string name;
public:
	int ReadFromRecord(string record);
	string WriteToRecord();
	int ReadNameAndTimeFromPbsScript(CommandAdapter* pAdapter,string filename);
	int WriteNameAndTimeToPbsScript(CommandAdapter* pAdapter,string filename);
	string Show();
	
	bool IsRunning(CommandAdapter* pAdapter);
	int CheckRunningResult(CommandAdapter* pAdapter);
	bool IsConverged(CommandAdapter* pAdapter);

};

#endif
