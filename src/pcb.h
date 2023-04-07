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
struct pcb* makeBackgroundPCB(FILE *script, int num_lines);
void free_pcb(struct pcb *p);
void set_page_table_entry(struct pcb *p, int page_number, int frame_number);

#endif