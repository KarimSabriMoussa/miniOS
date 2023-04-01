#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "page_table.h"
#include "shellmemory.h"
#include "pcb.h"

int PAGE_SIZE = 3;
int MAX_USER_INPUT = 1000;


struct page_table* initialise_page_table(FILE *script, int num_lines){

    
    struct page_table* table = malloc(sizeof(struct page_table));

    int table_size = num_lines/3;

    if(num_lines % 3 > 0){
        table_size++;
    }

    struct page_table_entry* arr[table_size];

    for(int i = 0; i < table_size; i++){
        struct page_table_entry *entry = malloc(sizeof(struct page_table_entry));
        arr[i]  = entry;
        (*arr[i]).frame_number = -1;
        (*arr[i]).lru_value = 0;
    }

    (*table).table = arr;
    (*table).size = table_size;

    return table;
}

void load_page(struct pcb *p, int page_number){

    /*TODO : check if there is enough space to insert a new page into the frame store
            if there isn't then evict the frame with the highest LRU value, insert the page into its place
            update page tables of both pcbs
    */

    char line[MAX_USER_INPUT];

    FILE *file = (*p).file_in_backing_store;

    fseek(file, page_number * PAGE_SIZE,SEEK_SET);

    for(int i = 0; i < PAGE_SIZE; i++){
        
        if(feof(file)){
            break;
        }
        fgets(line, MAX_USER_INPUT - 1, file);

        char encoding[100];
		encode(encoding, page_number * PAGE_SIZE + i, file);
		mem_set_value(encoding, line);
		memset(line, 0, sizeof(line));
    }
}

int get_page_size(){
    return PAGE_SIZE;
}

