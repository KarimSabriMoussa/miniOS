#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "interpreter.h"
#include "shellmemory.h"

int MAX_USER_INPUT = 1000;
int parseInput(char ui[]);

// int frame_store_size = FRAMESIZE;
// int variable_store_size = VARMEMSIZE;

// Start of everything
int main(int argc, char *argv[]) {
	printf("%s\n\n", "Shell version 1.2 Created January 2023");

    printf("Frame Store Size = %d; Variable Store Size = %d\n");

	char prompt = '$';  				// Shell prompt
	char userInput[MAX_USER_INPUT];		// user's input stored here
	int errorCode = 0;					// zero means no error, default

	//init user input
	for (int i=0; i<MAX_USER_INPUT; i++)
		userInput[i] = '\0';
	
	//init shell memory
	mem_init();
	while(1) {	
        int inInteractive =  isatty(STDIN_FILENO);

        if(inInteractive){
            printf("%c ",prompt);
        }

        fgets(userInput, MAX_USER_INPUT-1, stdin);

        errorCode = parseInput(userInput);
        if (errorCode == -1) exit(99);	// ignore all other errors
        memset(userInput, 0, sizeof(userInput));

        if(!inInteractive){
            if(feof(stdin)){
                char * tname = ctermid(NULL);
                FILE *terminal = fopen(tname, "rt");
                freopen(tname,"r", stdin);
                inInteractive = 1;
            }
        }
	}

	return 0;

}

int parseInput(char ui[]) {
    char tmp[200];
    char *words[100];                            
    int a = 0;
    int b;                            
    int w=0;    // wordID    
    int errorCode;

    for(a=0; ui[a]==' ' && a<1000; a++);    // skip white spaces
    while(ui[a] != '\n' && ui[a] != '\0' && a<1000) {
        for(;ui[a]==' ' && a<1000; a++);    // skip white spaces
        for(b=0;ui[a] != ';' && ui[a]!='\0' && ui[a]!='\n' && ui[a]!=' ' && a<1000; a++, b++){
            tmp[b] = ui[a];     // extract a word
        }
        if(b > 0){
            tmp[b] = '\0';
            words[w] = strdup(tmp);
            w++;
        }
        if(ui[a] == ';'){
            errorCode = interpreter(words,w);
            if(errorCode == -1){
                return errorCode;
            }
            w = 0;
            a++;
        }else{
            if(ui[a] == '\0') break;
        }
    }

    errorCode = interpreter(words, w);
    return errorCode;
}


int get_max_user_input(){
    return MAX_USER_INPUT;
}