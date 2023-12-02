#include "queue.h"

void Queue::put(PCB* p) {
	Elem* elem = new Elem(p);
	if(head) tail->next = elem;
	else head = elem;
	tail = elem;
}

PCB* Queue::get() {
	if(!head) return 0;
	Elem* elem = head;
	PCB* p = head->pcb;
	if(head->next) head = head->next;
	else head = 0;
	if(!head) tail = 0;
	delete elem;
	return p;
}

Queue::~Queue() {
	while(head) {
		Elem* elem = head;
		head = head->next;
		lock
		delete elem;
		unlock
	}
}

int Queue::decTime() {
	lock
	Elem* curr = head, *prev = 0, *elem;
	int cnt = 0;
	while(curr) {
		curr->pcb->semTime--;
		if(curr->pcb->semTime==0) {
			cnt++;
			//remove from list
			if(prev) prev->next = curr->next;
			else head = curr->next;
			elem = curr;
			curr = curr->next;

			//unblock thread
			elem->pcb->blocked = 0;
			Scheduler::put(elem->pcb);
			delete elem;
		}
		else {
			prev = curr;
			curr = curr->next;
		}
	}
	//update tail
	for(elem = head; elem->next; elem=elem->next);
	tail = elem;

	unlock
	return cnt;
}
