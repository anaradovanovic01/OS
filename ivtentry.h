#ifndef IVTENTRY_H_
#define IVTENTRY_H_

#include <iostream.h>
#include <dos.h>
#include "kevent.h"

typedef unsigned char IVTNo;
typedef void interrupt (*pInterrupt)(...);
class KernelEv;

class IVTEntry {
public:
	IVTEntry(IVTNo ivtNo, pInterrupt intr);
	void signal();
	void callOldRout();
	void setEvent(KernelEv* e) { event = e; }
	pInterrupt newRout,oldRout;

private:
	IVTNo ivtNo;
	KernelEv* event;

};
#endif
