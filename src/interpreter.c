#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h> 
#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include "shellmemory.h"
#include "shell.h"
#include "pcb.h"
#include "ready_queue.h"

int MAX_ARGS_SIZE = 7;

int badcommand(int code){
	if(code == 1){
		printf("%s\n", "Bad command: Too many tokens");
	} else if(code == 2){
		printf("%s\n", "Bad command: filename must be alphanumeric");
	} else if(code == 3){
		printf("%s\n", "Bad command: my_cd");
	} else if(code == 4){
		printf("%s\n", "Bad command: my_mkdir");
	} else if(code == 5){
		printf("%s\n", "Bad command: not enough space in OS shell memory");
	} else if(code == 6){
		printf("%s\n", "Bad command: this is not a valid policy");
	} else{	
		printf("%s\n", "Unknown Command");
	}

	return 1;
}

// For run command only
int badcommandFileDoesNotExist(){
	printf("%s\n", "Bad command: File not found");
	return 3;
}

int help();
int quit();
int set(char* var, char* value[],int args_size);
int print(char* var);
int echo(char* var);
int my_ls();
int my_mkdir(char* var);
int run(char* script);
int exec(char * command_args[], int args_size);
int badcommandFileDoesNotExist();
int touch(char* filename);
int cd(char* dir);
int strcompare(char* str1, char* str2);
int count_script_lines(char * script);

// Interpret commands and their arguments
int interpreter(char* command_args[], int args_size){
	int i;

	if (args_size < 1){
		return badcommand(0);
	}
	if(args_size > MAX_ARGS_SIZE){
		return badcommand(1);
	}
	for ( i=0; i<args_size; i++){ //strip spaces new line etc
		command_args[i][strcspn(command_args[i], "\r\n")] = 0;
	}

	if (strcmp(command_args[0], "help")==0){
	    //help
	    if (args_size != 1) return badcommand(0);
	    return help();
	
	} else if (strcmp(command_args[0], "quit")==0) {
		//quit
		if (args_size != 1) return badcommand(0);
		return quit();

	} else if (strcmp(command_args[0], "set")==0) {
		//set
		if (args_size < 3) return badcommand(0);
		if (args_size > 7) return badcommand(1);
		return set(command_args[1], command_args, args_size);
	
	} else if (strcmp(command_args[0], "print")==0) {
		if (args_size != 2) return badcommand(0);
		return print(command_args[1]);
	
	} else if (strcmp(command_args[0], "run")==0) {
		if (args_size != 2) return badcommand(0);
		return run(command_args[1]);

	} else if (strcmp(command_args[0], "echo")==0) {
		if (args_size != 2) return badcommand(0);
		return echo(command_args[1]);
	
	} else if (strcmp(command_args[0], "my_ls")==0) {
		if (args_size != 1) return badcommand(0);
		return my_ls();

	} else if (strcmp(command_args[0], "my_mkdir")==0) {
		if(args_size != 2) return badcommand(0);
		return my_mkdir(command_args[1]);	
	
	} else if (strcmp(command_args[0], "my_touch")==0){
		if(args_size != 2) return badcommand(0);
		return touch(command_args[1]);

	} else if (strcmp(command_args[0], "my_cd")==0) {
		if(args_size != 2) return badcommand(0);
		return cd(command_args[1]);		
	} else if (strcmp(command_args[0], "exec")==0){
		if(args_size < 3 || args_size > 5) return badcommand(0);
		return exec(command_args, args_size);
	} else {
		return badcommand(0);
}}

int help(){

	char help_string[] = "COMMAND\t\t\tDESCRIPTION\n \
help\t\t\tDisplays all the commands\n \
quit\t\t\tExits / terminates the shell with “Bye!”\n \
set VAR STRING\t\tAssigns a value to shell memory\n \
print VAR\t\tDisplays the STRING assigned to VAR\n \
run SCRIPT.TXT\t\tExecutes the file SCRIPT.TXT\n ";
	printf("%s\n", help_string);
	return 0;
}

int quit(){
	printf("%s\n", "Bye!");
	exit(0);
}

int set(char* var, char* command_args[], int args_size){
	char *delimeter = " ";
	char value[500];

	strcpy(value,"");
	for(int i = 2;i<args_size;i++){
		strcat(value,command_args[i]);
		if(i != args_size - 1)
			strcat(value, delimeter);
	}
	mem_set_value(var, value);

	return 0;

}

int print(char* var){
	printf("%s\n", mem_get_value(var)); 
	return 0;
}

int echo(char* var){
	if (var[0] == '$') {
		if (strcmp(mem_get_value(++var), "Variable does not exist")!=0) {
			printf("%s\n", mem_get_value(var));
		} else printf("\n");
	} else printf("%s\n", var);
	
	return 0;
}

// creates a directory in the current directory with name var "my_mkdir"
// checks in the memory if the given name is a variable
int my_mkdir(char* var) {
	if (var[0] == '$') {
		if (strcmp(mem_get_value(++var), "Variable does not exist")!=0) {
			if (strstr(mem_get_value(var), " ")) {
				badcommand(4);
			} else mkdir(mem_get_value(var), S_IRWXU | S_IRWXG | S_IRWXO);
		} else badcommand(4);
	} else mkdir(var, S_IRWXU | S_IRWXG | S_IRWXO);

	return 0;
}

// lists all directories/files in the current directory "my_ls"
int my_ls(){
	DIR *directory;
	struct dirent *entry;
	int count = 0;

	directory = opendir(".");

	if (directory == NULL) {
		printf("Error opening the directory.\n");
		return 1;
	}

	while ((entry = readdir(directory)) != NULL) {
		if (strcmp(entry->d_name, ".")!=0 && strcmp(entry->d_name, "..")!=0) {
			count++;
		}
	}

	rewinddir(directory);

	char dirnames[count][100];
	int i=0;
	while ((entry = readdir(directory)) != NULL) {
		if (strcmp(entry->d_name, ".")!=0 && strcmp(entry->d_name, "..")!=0) {
			strcpy(dirnames[i], entry->d_name);
			i++;
		}
	}

	for (int i=0;i<count-1;i++) {
		for (int j=0;j<count-i-1;j++){
			if(strcompare(dirnames[j], dirnames[j+1]) > 0){
				char* str1 = strdup(dirnames[j]);
				char* str2 = strdup(dirnames[j+1]);
				strcpy(dirnames[j], str2);
				strcpy(dirnames[j+1], str1);
			}
		}
	}

	for (int i=0;i<count;i++){
		puts(dirnames[i]);
	}

	if (closedir(directory) == -1) {
		printf("Error closing the directory.\n");
		return 1;
	}

	return 0;
}

// runs a file with name script if it exists "run"
int run(char* script){
	int errCode = 0;
	char line[1000];
	FILE *p = fopen(script,"rt");  // the program is in a file

	if(p == NULL){
		return badcommandFileDoesNotExist();
	}

	//count script lines
	int numlines = 0;
	while(1){
		fgets(line,999,p);
		numlines++;
		if(feof(p)){
			break;
		}
	}

	int freeSpace = mem_get_free_space();

	// print error if no available space
	if (numlines > freeSpace) {
		return badcommand(5);
	}

	// get process starting index
	int pStart = 1000-freeSpace;
	
	// store process in shell memory
	int counter = 0;
	rewind(p);
	fgets(line,999,p);
	while(1){
		char encoding[100];
		encode(encoding, counter, script);
		mem_set_value(encoding, line);
		memset(line, 0, sizeof(line));
		counter++;
		if(feof(p)){
			break;
		}
		fgets(line,999,p);
		
	}

	// execute instructions
	counter = 0;
	while(counter<numlines) {
		errCode = parseInput(mem_get_value_from_index(pStart+counter));
		counter++;
	}

	// shell memory cleanup
	mem_clean_up(script, numlines);

    fclose(p);

	return errCode;
}

// creates a file with name filename "my_touch"
int touch(char* filename){
	FILE* file;

	for(int i = 0 ; i < strlen(filename); i++){
		if(!isalnum(filename[i]) && filename[i] != '.' ){
			badcommand(2);
			return 0;
		}
	}

	file = fopen (filename, "w");

	if(file == NULL){
		puts(strerror(errno));
		return 0;
	}
	fclose(file);

	return 0;
}

// change directory "my_cd"
int cd(char* dir){

	for(int i = 0 ; i < strlen(dir); i++){
		if(!isalnum(dir[i])){
			badcommand(3);
			return 0;
		}
	}

	if(chdir(dir) != 0){
		badcommand(3);
		return 0;
	}

	return 0;
}

// Using this method instead of strcmp to place strings that start with uppercase letters right before strings with the same letter in lowercase.
// It compares the two strings and returns -1 if str1 should be before str2, returns 1 if str1 should be after str2, returns 0 if they are equal.
int strcompare(char* str1, char* str2) {
	int i = 0;
	while (1) {
		if (str1[i] == str2[i]) {
			i++;
		} else if (str1[i] < str2[i]) {
			if(isupper(str1[i]) && islower(str2[i]) && str1[i] + 32 > str2[i]) {
				return 1;
			} else return -1;
		} else {
			if(isupper(str2[i]) && islower(str1[i]) && str2[i] + 32 > str1[i]) {
				return -1;
			} else return 1;
		}
	}
	return 0;
}

int exec(char* command_args[], int args_size) {
	
	char* policy = command_args[args_size-1];
	int errCode = 0;

	//TODO: check if two files have the same name
	
	if (strcmp(policy, "FCFS") != 0 && strcmp(policy, "SJF") != 0 && strcmp(policy, "RR") != 0 && strcmp(policy, "RR30") != 0 && strcmp(policy, "AGING") != 0 ) {
		return badcommand(6);
	}


	char *scripts[3] = {NULL, NULL, NULL};
	struct pcb* pcbs[3] = {NULL, NULL, NULL};


	if (args_size > 5) {
		return badcommand(1);
	} else if (args_size == 5) {
		scripts[0] = command_args[1];
		scripts[1] = command_args[2];
		scripts[2] = command_args[3];
	} else if (args_size == 4) {
		scripts[0] = command_args[1];
		scripts[1] = command_args[2];
	} else if (args_size == 3) {
		scripts[0] = command_args[1];
	} else {
		//error
	}


	for (int i=0; i<args_size-2; i++){
		
		FILE *f = fopen(scripts[i],"rt");

		if(f == NULL){
			return badcommandFileDoesNotExist();
		}
		
		fclose(f);
	}

	int scriptLines[3] = {0, 0, 0};
	int totalSpace = 0;
	for (int i=0; i<args_size-2; i++){
		scriptLines[i] = count_script_lines(scripts[i]);
		totalSpace += scriptLines[i];
	}


	if (mem_get_free_space() < totalSpace) {
		return badcommand(5);
	}

	for (int i=0; i<args_size-2; i++){
		pcbs[i] = makePCB(scripts[i], scriptLines[i]);
	}


	errCode =  scheduler(pcbs[0], pcbs[1], pcbs[2], NULL,policy);

	//printMemory("memoryLog1.txt");

	for(int i = 0 ; i < args_size-2 ; i++ ){
		// shell memory cleanup
		mem_clean_up(scripts[i], scriptLines[i]);
	}

	//printMemory("memoryLog2.txt");

	return errCode;
}

int count_script_lines(char *script) {

	if (script == NULL) {
		return 0;
	}

	FILE *f = fopen(script,"rt");
	char line[1000];
	int numlines = 0;

	while(1){
		fgets(line,999,f);
		numlines++;
		if(feof(f)){
			break;
		}
	}

	fclose(f);
	return numlines;
}