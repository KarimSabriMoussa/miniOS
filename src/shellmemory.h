#include "pcb.h"
#include <stdio.h>


void mem_init();
char *mem_get_variable(char *var);
int mem_set_variable(char *var, char *value);
void mem_clear_var(char *var_in, int num);
int mem_get_first_free_index();
void mem_clean_up(char *script, int lines);
int mem_get_free_space();
char *mem_get_value_by_index(int index);
void encode(char *code, int counter, char *script);
void printMemory(char *filename);
FILE *code_loading(char *script_name);
FILE *background_code_loading(char *script_name);
void remove_backing_store();
int execute_line(struct pcb *p);
void load_page(struct pcb *p, int page_number);
void initialise_backing_store();
