#ifndef EVENT_H_
#define EVENT_H_

#include "pcb.h"
#include "kevent.h"
#include "ivtentry.h"

#define PREPAREENTRY(numEntry, callOld)\
void interrupt inter##numEntry(...); \
IVTEntry newEntry##numEntry(numEntry, inter##numEntry); \
void interrupt inter##numEntry(...) {\
	newEntry##numEntry.signal();\
	if (callOld == 1) newEntry##numEntry.callOldRout();\
}

typedef unsigned char IVTNo;
class KernelEv;

class Event {

public:
	Event (IVTNo ivtNo);
	~Event ();
	void wait ();

protected:
	friend class KernelEv;
	void signal();

private:
	KernelEv* myImpl;

};
#endif
