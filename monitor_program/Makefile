ENV=LINUX
monitor: monitor.cpp cmdadapter.o job.o task.o
	 g++ monitor.cpp cmdadapter.o job.o task.o -o monitor -D${ENV} -g

cmdadapter.o: CmdAdapter.cpp CmdAdapter.h
	g++ CmdAdapter.cpp -c -o cmdadapter.o -D${ENV} -g

job.o: Job.cpp Job.h
	g++ Job.cpp -c -o job.o -D${ENV} -g

task.o: Task.cpp Task.h
	g++ Task.cpp -c -o task.o -D${ENV} -g

clean:
	rm *.o
