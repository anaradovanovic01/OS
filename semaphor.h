#ifndef SEMAPHOR_H_
#define SEMAPHOR_H_

#include "pcb.h"
#include "ksem.h"

typedef unsigned int Time;

class KernelSem;

class Semaphore {

public:
	Semaphore (int init=1);
	virtual ~Semaphore ();
	virtual int wait (Time maxTimeToWait);
	virtual void signal();
	int val () const;

private:
	KernelSem* myImpl;

};

#endif
