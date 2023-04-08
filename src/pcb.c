#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "shellmemory.h"
#include "pcb.h"
#include "page_table.h"

struct pcb* makePCB(FILE *script, int num_lines) {
	
	if (script == NULL) {
		return NULL;
	}
	
    struct pcb* p = malloc(sizeof(struct pcb));

	struct page_table *page_table = initialise_page_table(script, num_lines);    

    (*p).length = num_lines;
    (*p).pc = 0;
	(*p).agingScore = num_lines;
	(*p).nextPCB = NULL;
	(*p).taken = 0;
	(*p).file_in_backing_store = script;
	(*p).page_table = page_table;

	for(int i = 0; i < (*page_table).size && i < 2; i++){
        load_page(p, i);
    }

    return p;
}


struct pcb* makeBackgroundPCB(FILE *script, int num_lines){

	
	if (script == NULL) {
		return NULL;
	}
	
    struct pcb* p = malloc(sizeof(struct pcb));

	struct page_table *page_table = initialise_page_table(script, num_lines);   
	

    (*p).length = num_lines;
    (*p).pc = 0;
	(*p).agingScore = num_lines;
	(*p).nextPCB = NULL;
	(*p).taken = 0;
	(*p).file_in_backing_store = script;
	(*p).page_table = page_table;

    return p;
}


void free_pcb(struct pcb *p){

	struct page_table *page_table = (*p).page_table;
	struct page_table_entry *table = (*page_table).table;

	free(table);
	table = NULL;

	free(page_table);
	page_table = NULL;

	free(p);
	p = NULL;

	return;
}

void set_page_table_entry(struct pcb *p, int page_number, int frame_number){
    struct page_table *p_table = (*p).page_table;
	(*p_table).table[page_number].frame_number = frame_number;
}
