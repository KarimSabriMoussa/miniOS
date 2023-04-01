#ifndef page_table_h
#define page_table_h

#include "pcb.h"

struct page_table_entry{
	char *frame_number;
	char *lru_value;
};

struct page_table{
    struct page_table_entry *table;
    int size;
};

struct page_table* initialise_page_table(FILE *script, int num_lines);
void load_page(struct pcb *p, int page_number);
int get_page_size();

#endif