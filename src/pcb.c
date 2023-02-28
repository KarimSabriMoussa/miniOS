#include "pcb.h"

void setPID (struct pcb process, int pid){
    process.pid = pid;
}
void setStartPos (struct pcb process, int startPos){
    process.startPos = startPos;
}
void setLength (struct pcb process, int length){
    process.length = length;
}
void setPC (struct pcb process, int pc){
    process.pc = pc;
}


