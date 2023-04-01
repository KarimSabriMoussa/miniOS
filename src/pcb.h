#ifndef pcb_h
#define pcb_h

#include "page_table.h"

struct pcb
{
    int length;
    int pc;
    int agingScore;
    struct pcb *nextPCB;
    int taken;
    FILE *file_in_backing_store;
    struct page_table *page_table;
    
};

struct pcb* makePCB(FILE *script, int numlines);
struct pcb* makeBackgroundPCB(char *script); 
void free_pcb(struct pcb *p);

#endif