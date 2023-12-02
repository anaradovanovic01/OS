#ifndef PCB_H_
#define PCB_H_

#include <iostream.h>
#include <dos.h>
#include "queue.h"
#include "thread.h"

extern volatile PCB* running;
extern volatile int lockFlag;
extern volatile int requested_change_of_context;

#define intd asm{\
 		pushf;\
 		cli;\
}
#define inte asm popf

#define lock lockFlag++;
#define unlock lockFlag--;\
		if (lockFlag==0 && requested_change_of_context) {\
			dispatch();\
		}

typedef unsigned long StackSize;
typedef unsigned int Time;
typedef int ID;
class Thread;
class Queue;
class PCB;

struct Elem {
	PCB* pcb;
	Elem* next;

	Elem(PCB* p) : pcb(p), next(0) {}
};

class PCB {
public:
	static ID ids;
	static Elem *head, *tail;

	ID id;
	unsigned* stack;
	unsigned sp, ss, bp;
	int finished, blocked;
	Time time;
	Thread* thread;
	Queue* waitingThreads;
	Time semTime;

	PCB(StackSize stackSize, Time timeSlice, Thread* thread);
	PCB(Time timeSlice);
	~PCB();

	static PCB* getFromListByID(int i);
	static void wrapper();
};

#endif
