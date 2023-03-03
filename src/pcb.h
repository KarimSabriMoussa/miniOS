#ifndef pcb_h
#define pcb_h

struct pcb
{
    int pid;
    int startPos;
    int length;
    int pc;
    struct pcb* nextPCB;
};
void setPID (struct pcb process, int pid);
void setStartPos (struct pcb process, int startPos);
void setLength (struct pcb process, int length);
void setPC (struct pcb process, int pc);

#endif