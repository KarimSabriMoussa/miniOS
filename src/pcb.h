#ifndef pcb_h
#define pcb_h

struct pcb
{
    char *pid;
    int startPos;
    int length;
    int pc;
    int agingScore;
    struct pcb* nextPCB;
};

struct pcb* makePCB(char *script, int numlines);
struct pcb* makeBackgroundPCB(char *script); 

#endif