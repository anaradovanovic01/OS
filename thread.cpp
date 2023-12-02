#include "thread.h"

extern void interrupt timer();

void dispatch() {
	intd
	requested_change_of_context = 1;
	timer();
	inte
}

Thread::Thread (StackSize stackSize, Time timeSlice) {
	lock
	myPCB = new PCB(stackSize, timeSlice, this);
	unlock
}

Thread::~Thread() {
	lock
	delete myPCB;
	unlock
}

ID Thread::getId(){
	return myPCB->id;
}

ID Thread::getRunningId() {
	return running->thread->getId();
}

Thread* Thread::getThreadById(ID id) {
	Elem* curr = PCB::head;
	while(curr) {
		if(curr->pcb->id == id) return curr->pcb->thread;
		curr = curr->next;
	}
	return 0;
}

void Thread::start() {
	if(myPCB && !myPCB->finished) Scheduler::put(this->myPCB);
}

void Thread::waitToComplete() {
	lock
	if(myPCB &&  running!=this->myPCB && !myPCB->finished) {
		running->blocked = 1;
		this->myPCB->waitingThreads->put((PCB*)running);
		unlock
		dispatch();
	}
	else unlock
}
