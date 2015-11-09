#include "CmdAdapter.h"
#include "Job.h"
#include "Task.h"
using namespace std;


int main(){

	Task* pTask;
	CommandAdapter* pCmd;

#ifdef LINUX
	pCmd = new LinuxAdapter();
#else
	pCmd = new WindowsAdapter();
#endif

	pTask = TaskFactory::GenerateTask(inFileName,pCmd);
	if(pTask==NULL){
		return -1;
	}

	pTask->Run();

	delete pTask;
	delete pCmd;
	return 0;
} 
