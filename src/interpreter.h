int interpreter(char* command_args[], int args_size);
int help();
int run(char* script);
int get_background_flag();
void clean_scripts();
void set_script(char *script_name, int num_lines);
int get_background_num_lines();
void set_background_script(char *script_name, int num_lines);