#ifndef KSEM_H_
#define KSEM_H_

#include "pcb.h"
#include "queue.h"
#include "schedule.h"

class KernelSem;

struct ElemS {
	KernelSem* sem;
	ElemS* next;

	ElemS(KernelSem* s) : sem(s), next(0) {}
};

class KernelSem {

public:
	KernelSem (int init=1);
	~KernelSem ();
	int wait (Time maxTimeToWait);
	void signal();
	int getVal () const;
	static void clockTicks();

private:
	static ElemS* head, *tail;
	int val;
	Queue* blocked;
	void block ();
	void unblock ();

};
#endif
