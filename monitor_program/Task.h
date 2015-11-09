#ifndef TASK
#define TASK

#include "Job.h"
#include "CmdAdapter.h"

enum TaskType{
	TASK_TYPE_RELAX_ONLY,
	TASK_TYPE_RELAX_VIBRATION,
	TASK_TYPE_NEB
};

class Task{
	TaskType taskType;
protected:
	CommandAdapter* pCom;
public:
	Task(TaskType type,CommandAdapter* pCmdAdapter):
	  taskType(type),pCom(pCmdAdapter){}
	  void SubmitARelaxJob(Job lastJob);
	  void ContinueARelaxJob(Job lastJob);
	  void SubmitAVibrationJob(Job lastJob);
	  void ContinueAVibrationJob(Job lastJob);

	  virtual int Run() = 0;
	  virtual string Show()=0;
};

class TaskRelaxOnly: public Task{
public:
	TaskRelaxOnly(TaskType type,CommandAdapter* pCmdAdapter):Task(type,pCmdAdapter){}
	string Show();
	int Run();
};

class TaskRelaxVibration: public Task{
public:
	TaskRelaxVibration(TaskType type,CommandAdapter* pCmdAdapter):Task(type,pCmdAdapter){}
	string Show();
	int Run();
};

class TaskNEB: public Task{
public:
	TaskNEB(TaskType type,CommandAdapter* pCmdAdapter):Task(type,pCmdAdapter){}
	string Show();
	int Run();
};

class TaskFactory{
public:
	static Task* GenerateTask(string filename,CommandAdapter* pCmd);
};

#endif
