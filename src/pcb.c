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

	struct page_table * page_table = initialise_page_table(script, num_lines);

	for(int i = 0; i < (*page_table).size && i < 2; i++){
        load_page(script, i);
    }
    

    (*p).length = num_lines;
    (*p).pc = 0;
	(*p).agingScore = num_lines;
	(*p).nextPCB = NULL;
	(*p).taken = 0;
	(*p).file_in_backing_store = script;
	(*p).page_table = page_table;

    return p;
}


struct pcb* makeBackgroundPCB(char *script){
	
	if (script == NULL) {
		return NULL;
	}
	
    struct pcb* p = malloc(sizeof(struct pcb));

    char line[1000];

	// get process starting index
	int pStart = 1000-mem_get_free_space();
	
	// store process in shell memory
	int counter = 0;
	fgets(line,999,stdin);
	while(1){
		char encoding[100];
		encode(encoding, counter, script);
		mem_set_value(encoding, line);
		memset(line, 0, sizeof(line));
		counter++;
		if(feof(stdin)){
			break;
		}
		fgets(line,999,stdin);
	}

    (*p).length = counter;
    (*p).pc = 0;
	(*p).agingScore = counter;
	(*p).nextPCB = NULL;
	(*p).taken = 0;

    return p;
}


void free_pcb(struct pcb *p){

	struct page_table *page_table = (*p).page_table;

	for(int i = 0; i < (*page_table).size; i++){
		struct page_table_entry entry = (*page_table).table[i];
		free(&entry);
	}

	free(page_table);

	free(p);

	return;
}