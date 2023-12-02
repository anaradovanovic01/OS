#include "kevent.h"

extern IVTEntry* ivtentries[256];
extern volatile PCB* running;

KernelEv::KernelEv(IVTNo num){
	lock
	val = 0;
	ivtNo = num;
	thread = (PCB*)running;
	ivtentries[num]->setEvent(this);
	ivtentries[ivtNo]->oldRout = getvect(ivtNo);
	setvect(ivtNo, ivtentries[ivtNo]->newRout);
	unlock
}

KernelEv::~KernelEv() {
	lock
	ivtentries[ivtNo]->setEvent(0);
	setvect(ivtNo,ivtentries[ivtNo]->oldRout);
	unlock
}

void KernelEv::wait() {
	lock
	if(thread != running) return;
	if(val) {
		val = 0;
		unlock
	}
	else {
		thread->blocked = 1;
		unlock
		dispatch();
	}
}

void KernelEv::signal() {
	if(thread->blocked) {
		thread->blocked = 0;
		Scheduler::put(thread);
	}
	else val = 1;
}
