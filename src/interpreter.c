#include <stdio.h>
#include <stdlib.h>
#include <dirent.h> 
#include <string.h> 
#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include "shellmemory.h"
#include "shell.h"

int MAX_ARGS_SIZE = 7;

int badcommand(int code){
	if(code == 1){
		printf("%s\n", "Bad Command: Too many tokens");
	} else if(code == 2){
		printf("%s\n", "Bad Command: filename must be alphanumeric");
	} else if(code == 3){
		printf("%s\n", "Bad Command: my_cd");
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
int run(char* script);
int badcommandFileDoesNotExist();
int touch(char* filename);
int cd(char* dir);

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
	
	} else if (strcmp(command_args[0], "my_touch")==0){
		if(args_size != 2) return badcommand(0);
		return touch(command_args[1]);
	} else if (strcmp(command_args[0], "my_cd")==0) {
		if(args_size != 2) return badcommand(0);
		return cd(command_args[1]);		
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
	char *link = "=";
	char buffer[1000];
	char *delimeter = " ";
	char value[500];

	strcpy(buffer, var);
	strcat(buffer, link);
	strcat(buffer, value);	

	strcpy(value,"");
	for(int i = 2;i<args_size;i++){
		strcat(value,command_args[i]);
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

int my_ls(){
	DIR *directory;
	struct dirent *entry;
	char **direct;
	char *temp;

	directory = opendir(".");

	if (directory == NULL) {
		printf("Error opening the directory.\n");
		return 1;
	}

	while ((entry = readdir(directory)) != NULL) {
		if (strcmp(entry->d_name, ".")!=0 && strcmp(entry->d_name, "..")!=0)
			printf("%s\n", entry->d_name);
	}

	// while ((entry = readdir(directory)) != NULL) {
	// 	direct++ = entry->d_name;
	// }

	// for(i=0;i<100;i++) {
    // 	for(j=i+1;j<100;j++) {
	// 		if(strcmp(direct[i],direct[j])>0){
	// 			strcpy(temp,dirct[i]);
	// 			strcpy(direct[i],direct[j]);
	// 			strcpy(direct[j],temp);
	// 		}
	// 	}
	// }

	if (closedir(directory) == -1) {
		printf("Error closing the directory.\n");
		return 1;
	}

	return 0;
}

int run(char* script){
	int errCode = 0;
	char line[1000];
	FILE *p = fopen(script,"rt");  // the program is in a file

	if(p == NULL){
		return badcommandFileDoesNotExist();
	}

	fgets(line,999,p);
	while(1){
		errCode = parseInput(line);	// which calls interpreter()
		memset(line, 0, sizeof(line));

		if(feof(p)){
			break;
		}
		fgets(line,999,p);
	}

    fclose(p);

	return errCode;
}

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

int cd(char* dir){

	char relativeDir[100];
	char* slash = "/";

	for(int i = 0 ; i < strlen(dir); i++){
		if(!isalnum(dir[i])){
			badcommand(3);
			return 0;
		}
	}
	
	strcpy(relativeDir, slash);
	strcat(relativeDir,dir);

	if(chdir(dir) != 0){
		badcommand(3);
		return 0;
	}

	return 0;
}