#include "main.h"

extern int userMain(int argc, char** argv);

void interrupt timer(){
	if (!requested_change_of_context) {
		asm int 60h;
		tick();
		counter--;
		KernelSem::clockTicks();
	}
	if (counter == 0 || requested_change_of_context) {
		if(!lockFlag) {
			requested_change_of_context = 0;
			asm {
				mov tsp, sp
				mov tss, ss
				mov tbp, bp
			}

			running->sp = tsp;
			running->ss = tss;
			running->bp = tbp;

			if(!running->blocked && !running->finished) Scheduler::put((PCB*)running);

			running = Scheduler::get();
			if(running == 0) running = idleTh->getPCB();

			tsp = running->sp;
			tss = running->ss;
			tbp = running->bp;

			counter = running->time;

			asm {
				mov sp, tsp
				mov ss, tss
				mov bp, tbp
			}
		}
		else requested_change_of_context = 1;
	}

	/*if(!requested_change_of_context) {
		asm int 60h;
		tick();
	}*/

}

void inic() {
	asm{
		cli
		push es
		push ax

		mov ax,0
		mov es,ax

		mov ax, word ptr es:0022h
		mov word ptr oldTimerSEG, ax
		mov ax, word ptr es:0020h
		mov word ptr oldTimerOFF, ax

		mov word ptr es:0022h, seg timer
		mov word ptr es:0020h, offset timer

		mov ax, oldTimerSEG
		mov word ptr es:0182h, ax
		mov ax, oldTimerOFF
		mov word ptr es:0180h, ax

		pop ax
		pop es
		sti
	}
}

void restore() {
	asm {
		cli
		push es
		push ax

		mov ax,0
		mov es,ax


		mov ax, word ptr oldTimerSEG
		mov word ptr es:0022h, ax
		mov ax, word ptr oldTimerOFF
		mov word ptr es:0020h, ax

		pop ax
		pop es
		sti
	}
}

int main(int argc, char* argv[]) {
	lock

	mainPCB = new PCB(2);
	running = mainPCB;

	idleTh = new IdleThread();
	idleTh->start();

	unlock

	inic();

	int ret = userMain(argc, argv);

	restore();

	delete idleTh;

	delete mainPCB;

	return ret;
}
