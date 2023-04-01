#include "pcb.h"


void mem_init();
char *mem_get_value(char *var);
int mem_set_value(char *var, char *value);
void mem_clear_var(char *var_in, int num);
int mem_get_first_free_index();
void mem_clean_up(char *script, int lines);
int mem_get_free_space();
char *mem_get_value_from_index(int index);
void encode(char *code, int counter, char *script);
void printMemory(char *filename);
FILE *code_loading(char *script_name);
void remove_backing_store();
void execute_line(int pc, struct pcb *p);
