#include "ksem.h"

extern volatile PCB* running;

ElemS* KernelSem::head = 0;
ElemS* KernelSem::tail = 0;

KernelSem::KernelSem (int init) {
	val = init;
	lock
	blocked = new Queue();
	unlock
	if(!head) {
		lock
		head = tail = new ElemS(this);
		unlock
	} else {
		lock
		tail->next = new ElemS(this);
		unlock
		tail = tail->next;
	}
}

KernelSem::~KernelSem (){
	PCB* pcb = blocked->get();
	while(pcb) {
		pcb->blocked = 0;
		Scheduler::put(pcb);
		pcb = blocked->get();
	}
	lock
	delete blocked;
	unlock

	ElemS* curr = head, *prev = 0;
	while(curr) {
		if(curr->sem == this) {
			if(prev) prev->next = curr->next;
			else head = curr->next;
			if(tail == curr) tail = prev;
			if(!head) tail = 0;
			break;
		}
		prev = curr;
		curr = curr->next;
	}
	delete curr;
}

void KernelSem::block () {
	blocked->put((PCB*)running);
	running->blocked = 1;
	unlock
	dispatch();
}

void KernelSem::unblock () {
	PCB* pcb = blocked->get();
	pcb->blocked = 0;
	Scheduler::put(pcb);
}

int KernelSem::wait (Time maxTimeToWait) {
	lock
	if (--val<0) {
		running->semTime = maxTimeToWait;
		block();
		if(running->semTime == 0 && maxTimeToWait != 0) return 0;
		else return 1;
	}
	else {
		unlock
		return 1;
	}
}

void KernelSem::signal () {
	lock
	if (++val<=0) unblock();
	unlock
}

int KernelSem::getVal() const { return val; }

void KernelSem::clockTicks() {
	ElemS* curr = head;
	while(curr) {
		int num = curr->sem->blocked->decTime();
		curr->sem->val += num;
		curr = curr->next;
	}
}
