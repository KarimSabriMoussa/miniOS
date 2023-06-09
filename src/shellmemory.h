void mem_init();
char *mem_get_value(char *var);
void mem_set_value(char *var, char *value);
void mem_clear_var(char *var);
void mem_clean_up(char *script, int lines);
int mem_get_free_space();
char *mem_get_value_from_index(int index);
void encode(char *code, int counter, char *script);
void printMemory(char *filename);