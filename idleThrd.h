#ifndef IDLETHRD_H_
#define IDLETHRD_H_

#include "thread.h"
#include "pcb.h"

class IdleThread : public Thread {
public:
	IdleThread() : Thread(256, 1) { getPCB()->finished = 1; } //da je niko ne bi cekao i da pri startovanju ne bi isla u Scheduler
	virtual void run() { while(1); }
	PCB* getPCB() { return PCB::getFromListByID(2); }
};

#endif
