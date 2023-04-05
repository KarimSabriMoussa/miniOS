#ifndef page_table_h
#define page_table_h

#include "pcb.h"

struct page_table_entry{
	int frame_number;
};

struct page_table{
    struct page_table_entry *table;
    int size;
};

struct page_table* initialise_page_table(FILE *script, int num_lines);
void load_page(struct pcb *p, int page_number);
int get_page_size();
void set_page_table_entry(struct pcb *p, int page_number, int frame_number);

#endif