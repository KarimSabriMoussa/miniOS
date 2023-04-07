#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "page_table.h"
#include "shellmemory.h"
#include "pcb.h"

int PAGE_SIZE = 3;
int FRAME_SIZE = 3;
 
struct page_table* initialise_page_table(FILE *script, int num_lines){

    
    struct page_table* p_table = malloc(sizeof(struct page_table));

    int table_size = num_lines/3;

    if(num_lines % 3 > 0){
        table_size++;
    }

    struct page_table_entry* arr = malloc(table_size * sizeof(struct page_table_entry));

    for(int i = 0; i < table_size; i++){
        (arr[i]).frame_number = -1;
    }

    (*p_table).table = arr;
    (*p_table).size = table_size;

    return p_table;
}

int get_page_size(){
    return PAGE_SIZE;
}

int get_frame_size(){
	return FRAME_SIZE;
}





