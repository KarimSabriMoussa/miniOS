#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
	method signatures
*/
void encode(char *code, int counter, char *script);

struct memory_struct{
	char *var;
	char *value;
};

struct memory_struct shellmemory[1000];

// Helper functions
int match(char *model, char *var) {
	int i, len=strlen(var), matchCount=0;
	for(i=0;i<len;i++)
		if (*(model+i) == *(var+i)) matchCount++;
	if (matchCount == len)
		return 1;
	else
		return 0;
}

char *extract(char *model) {
	char token='=';    // look for this to find value
	char value[1000];  // stores the extract value
	int i,j, len=strlen(model);
	for(i=0;i<len && *(model+i)!=token;i++); // loop till we get there
	// extract the value
	for(i=i+1,j=0;i<len;i++,j++) value[j]=*(model+i);
	value[j]='\0';
	return strdup(value);
}


// Shell memory functions

void mem_init(){

	int i;
	for (i=0; i<1000; i++){		
		shellmemory[i].var = "none";
		shellmemory[i].value = "none";
	}
}

// Set key value pair
void mem_set_value(char *var_in, char *value_in) {
	
	int i;

	for (i=0; i<1000; i++){
		if (strcmp(shellmemory[i].var, var_in) == 0){
			shellmemory[i].value = strdup(value_in);
			return;
		} 
	}

	//Value does not exist, need to find a free spot.
	for (i=0; i<1000; i++){
		if (strcmp(shellmemory[i].var, "none") == 0){
			shellmemory[i].var = strdup(var_in);
			shellmemory[i].value = strdup(value_in);
			return;
		} 
	}

	return;

}

//get value based on input key
char *mem_get_value(char *var_in) {
	int i;

	for (i=0; i<1000; i++){
		if (strcmp(shellmemory[i].var, var_in) == 0){

			return strdup(shellmemory[i].value);
		} 
	}
	return "Variable does not exist";

}

void mem_clear_var(char *var_in, int num) {
	for (int i=0; i<1000; i++){
		if (strcmp(shellmemory[i].var, var_in) == 0){
			for (int j=i+num; j<1000; j++) {
				if (shellmemory[j].var != strdup("none")) {
					shellmemory[j-num].var = shellmemory[j].var;
					shellmemory[j-num].value = shellmemory[j].value;
				} else {
					shellmemory[j-num].var = shellmemory[j].var;
					shellmemory[j-num].value = shellmemory[j].value;
					break;
				}
			}
			return;
		} 
	}
	return;
}

void mem_clean_up(char *script, int lines) {
	char offset[100];
	char encoding[100];
	encode(encoding,0,script);
	mem_clear_var(encoding, lines);
	memset(encoding, 0, sizeof(encoding));
	memset(offset, 0, sizeof(offset));
}

int mem_get_free_space() {
	int i;
	for (i=0; i<1000; i++){
		if (strcmp(shellmemory[i].var, "none") == 0){
			break;
		} 
	}
	return 1000-i;
}

char *mem_get_value_from_index(int index) {
	if(index >= 0 && index < 1000) {
		return shellmemory[index].value;
	}
	return "Given index is out of bounds";
}

void encode(char *code, int counter, char *script){
	char offset[5];
	strcpy(code, script);
	sprintf(offset,"_%d",counter);
	strcat(code, offset);
}

void printMemory(char *filename){

	FILE *f = fopen(filename, "w");
	int counter = 0;
	int flag = 0;


	for(int i = 0; i < 1000; i++){
		if(strcmp(shellmemory[i].value, "none") != 0){
			if(flag == 1 ){
				fprintf(f,"%d lines of none\n", counter);
				counter = 0;
				flag = 0;
			}
			if(strchr(shellmemory[i].value, '\n') == NULL){
				fprintf(f,"%d:\t\t%s\t\t\t\t%s\n", i, shellmemory[i].var,shellmemory[i].value);
			}else{
				fprintf(f,"%d:\t\t%s\t\t\t\t%s", i, shellmemory[i].var,shellmemory[i].value);
			}
			
		}else{
			if (flag == 0){
				flag = 1;
			}
			counter++;
		}
	}

	if(flag == 1 ){
				fprintf(f,"%d lines of none\n", counter);
				counter = 0;
				flag = 0;
	}

	fclose(f);
}