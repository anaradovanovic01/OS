#include "pcb.h"

ID PCB::ids=1;
Elem* PCB::head=0;
Elem* PCB::tail=0;

void PCB::wrapper() {
	running->thread->run();
	running->finished = 1;
	lock
	PCB* pcb = running->waitingThreads->get();
	while(pcb) {
		pcb->blocked = 0;
		Scheduler::put(pcb);
		pcb = running->waitingThreads->get();
	}
	unlock
	dispatch();
}

PCB::PCB(Time timeSlice) { //konstruktor za main thread
	id = ids++;

	stack = 0;
	thread = 0;

	if(timeSlice == 0) time = -1;
	else time = timeSlice;

	finished = 0; blocked = 0;

	lock
	waitingThreads = new Queue();
	unlock

	if(!head) {
		lock
		head = tail = new Elem(this);
		unlock
	} else {
		lock
		tail->next = new Elem(this);
		unlock
		tail = tail->next;
	}
}

PCB::PCB(StackSize stackSize, Time timeSlice, Thread* thread) {
	id = ids++;

	//inicijalizacija steka
	if (stackSize > 65535) stackSize = 65535;
	stackSize/= sizeof(unsigned);
	lock
	stack = new unsigned[stackSize];
	unlock

	#ifndef BCC_BLOCK_IGNORE
	stack[stackSize-1] = 0x200;

	stack[stackSize-2] = FP_SEG(PCB::wrapper);
	stack[stackSize-3]= FP_OFF(PCB::wrapper);

	sp = FP_OFF(stack+stackSize-12);
	ss = FP_SEG(stack+stackSize-12);
	#endif
	bp=sp;

	if(timeSlice == 0) time = -1;
	else time = timeSlice;

	finished = 0; blocked = 0;

	this->thread = thread;

	lock
	waitingThreads = new Queue();
	unlock

	//dodavanje u listu svih pcbova
	if(!head) {
		lock
		head = tail = new Elem(this);
		unlock
	} else {
		lock
		tail->next = new Elem(this);
		unlock
		tail = tail->next;
	}
}

PCB:: ~PCB() {
	Elem* curr = PCB::head, *prev = 0;
	while(curr) {
		if(curr->pcb == this) {
			if(prev) prev->next = curr->next;
			else head = curr->next;
			if(!head) tail = 0;
			break;
		}
		prev = curr;
		curr = curr->next;
	}
	lock
	delete curr;

	delete waitingThreads;

	if(stack) delete[] stack;
	unlock
}

PCB* PCB::getFromListByID(int i) {
	Elem* curr = PCB::head;
	while(curr) {
		if(curr->pcb->id == i) return curr->pcb;
		curr = curr->next;
	}
	return 0;
}
