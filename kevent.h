#ifndef KEVENT_H_
#define KEVENT_H_

#include <dos.h>
#include "pcb.h"
#include "ivtentry.h"

typedef void interrupt (*pInterrupt)(...);
typedef unsigned char IVTNo;
class IVTEntry;

class KernelEv {

public:
	KernelEv (IVTNo ivtNo);
	~KernelEv();
	void wait ();
	void signal();
	//pInterrupt oldRout;

private:
	int val;
	IVTNo ivtNo;
	PCB* thread;

};
#endif
