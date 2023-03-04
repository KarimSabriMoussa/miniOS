#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "shellmemory.h"
#include "pcb.h"

struct pcb* makePCB(char *script, int numlines) {
	
	if (script == NULL) {
		return NULL;
	}
	
    struct pcb* p = malloc(sizeof(struct pcb));

    char line[1000];
	FILE *f = fopen(script,"rt");  // the program is in a file

	// get process starting index
	int pStart = 1000-mem_get_free_space();
	
	// store process in shell memory
	int counter = 0;
	fgets(line,999,f);
	while(1){
		char encoding[100];
		encode(encoding, counter, script);
		mem_set_value(encoding, line);
		memset(line, 0, sizeof(line));
		counter++;
		if(feof(f)){
			break;
		}
		fgets(line,999,f);
	}
    fclose(f);

    (*p).startPos = pStart;
    (*p).length = numlines;
    (*p).pc = 0;
    (*p).pid = script;
	(*p).agingScore = numlines;
	(*p).nextPCB = NULL;

    return p;
}
