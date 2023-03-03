#ifndef pcb_h
#define pcb_h

struct pcb
{
    char *pid;
    int startPos;
    int length;
    int pc;
    struct pcb* nextPCB;
};

struct pcb* makePCB(char *script, int numlines);

#endif