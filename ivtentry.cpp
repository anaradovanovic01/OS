#include "ivtentry.h"

IVTEntry* ivtentries[256];

IVTEntry::IVTEntry(IVTNo num, pInterrupt intr) {
	lock
	ivtNo = num;
	newRout = intr;
	oldRout = 0;
	ivtentries[ivtNo] = this;
	event = 0;
	unlock
}

void IVTEntry::signal() {
	if(event) event->signal();
}

void IVTEntry::callOldRout() {
	(*oldRout)();
}
