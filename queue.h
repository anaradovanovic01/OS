#ifndef QUEUE_H_
#define QUEUE_H_

#include "pcb.h"

class PCB;
struct Elem;

class Queue {
public:
	void put(PCB*);
	PCB* get();

	Queue() : head(0), tail(0) {}

	~Queue();

	int decTime();

private:
	Elem *head, *tail;
};
#endif
