#ifndef MAIN_H_
#define MAIN_H_

#include <iostream.h>
#include "thread.h"
#include "idleThrd.h"
#include "pcb.h"
#include "ksem.h"

volatile PCB* running;
PCB* mainPCB;
IdleThread* idleTh;

volatile int lockFlag = 0;
volatile unsigned counter = 0;
volatile int requested_change_of_context = 0;
unsigned oldTimerOFF, oldTimerSEG;
volatile unsigned tsp, tss, tbp;

void interrupt timer();

void inic();

void restore();

void tick();

int main(int argc, char* argv[]);

#endif
