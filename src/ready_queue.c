#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "pcb.h"
#include "ready_queue.h"
#include "shellmemory.h"
#include "shell.h"


#define QUEUE_LENGTH 5
struct pcb *readyQueue[QUEUE_LENGTH];
struct pcb *head;
int numProcesses;

void printAgingScore(char *filename);


//initialises the queue to all NULL pointers
void initialize_ready_queue(){
    for (int i = 0; i < QUEUE_LENGTH; i++){
        readyQueue[i] = NULL;
    }

    return;
}

//Adds pcb to the tail of the queue
void add_pcb_to_ready_queue(struct pcb *p){
 
    for (int i = 0; i < QUEUE_LENGTH; i++){
        if (readyQueue[i] == NULL){
            readyQueue[i] = p;
            break;
        }
    }

    return;
}

//This function finds which pcb in the ready queue that is pointing to p.
struct pcb* find_previous_PCB(struct pcb *p){
    for(int i = 0 ; i < QUEUE_LENGTH ; i++){
        if(readyQueue[i] != NULL && (*readyQueue[i]).nextPCB == p){
                return readyQueue[i];
            }
    }

    return NULL;
}

void decrement_aging_score(){

    for(int i = 0 ; i < QUEUE_LENGTH; i++){
        if(readyQueue[i] != NULL && readyQueue[i] != head && (*readyQueue[i]).agingScore > 0){
            (*readyQueue[i]).agingScore--;
        }
    }   
    return;
}

void shift_pcbs(){
    for(int i = 1; i < QUEUE_LENGTH; i++){
        readyQueue[i-1] = readyQueue[i];
        if(readyQueue[i] == NULL){
            break;
        }
    }
}

void sort_pcbs_by_shortest_length(int numProcesses){

    struct pcb *temp = NULL;
    
    for (int i = 0; i < numProcesses; i++) {     
            for (int j = i+1; j < numProcesses; j++) {     
                if((*readyQueue[i]).length > (*readyQueue[j]).length) {    
                    temp = readyQueue[i];    
                    readyQueue[i] = readyQueue[j];    
                    readyQueue[j] = temp;    
                }     
            }     
        } 

    return;
}

void sort_pcbs_by_aging_score(int numProcesses){

    struct pcb *temp = NULL;
    
    for (int i = 0; i < numProcesses; i++) {     
            for (int j = i+1; j < numProcesses; j++) {     
                if((*readyQueue[i]).agingScore > (*readyQueue[j]).agingScore) {    
                    temp = readyQueue[i];    
                    readyQueue[i] = readyQueue[j];    
                    readyQueue[j] = temp;    
                }     
            }     
        } 

    return;
}

void add_pcbs_to_ready_queue_FCFS(struct pcb *p1, struct pcb *p2, struct pcb *p3, struct pcb *p4){

    head = NULL; 
    if (p1 != NULL){
        add_pcb_to_ready_queue(p1);
        numProcesses++;

        if (p2 != NULL){
            (*p1).nextPCB = p2;
            add_pcb_to_ready_queue(p2);
            numProcesses++;
        }
        if (p3 != NULL){
            (*p2).nextPCB = p3;
            add_pcb_to_ready_queue(p3);
            numProcesses++;
        }
        if (p4 != NULL){
            (*p3).nextPCB = p4;
            add_pcb_to_ready_queue(p4);
            numProcesses++;
        }

        head = readyQueue[0];
    }

    return;
}

void add_pcbs_to_ready_queue_SJF(struct pcb *p1, struct pcb *p2, struct pcb *p3, struct pcb *p4){
    
    numProcesses = 0;
    head = NULL;

    if (p1 != NULL){
        readyQueue[0] = p1;
        numProcesses++;

        if (p2 != NULL){
            readyQueue[1] = p2;
            numProcesses++;
        }
        if (p3 != NULL){
            readyQueue[2] = p3;
            numProcesses++;
        }
        if (p4 != NULL){
            readyQueue[3] = p4;
            numProcesses++;
        }

        sort_pcbs_by_shortest_length(numProcesses);

        for(int i = 0 ; i < numProcesses; i++){
            if(i != 0){
                (*readyQueue[i-1]).nextPCB = readyQueue[i];
            }
        }

        head = readyQueue[0];
    }

    return;
}

void add_pcbs_to_ready_queue_RR(struct pcb *p1, struct pcb *p2, struct pcb *p3, struct pcb *p4){
    
    numProcesses = 0;
    head = NULL;

    if(p1 != NULL){
        readyQueue[0] = p1;
        numProcesses++;

        if (p2 != NULL){
            readyQueue[1] = p2;
            numProcesses++;
        }
        if (p3 != NULL){
            readyQueue[2] = p3;
            numProcesses++;
        }
        if (p4 != NULL){
            readyQueue[3] = p4;
            numProcesses++;
        }

        for(int i = 0; i < numProcesses; i++){
            (*readyQueue[i % numProcesses]).nextPCB = (readyQueue[(i+1) % numProcesses]);
        }

        head = readyQueue[0];
    }

    return;
}

void add_pcbs_to_ready_queue_AGING(struct pcb *p1, struct pcb *p2, struct pcb *p3, struct pcb *p4){

    numProcesses = 0;
    head = NULL;

    if(p1 != NULL){
        readyQueue[0] = p1;
        numProcesses++;

        if (p2 != NULL){
            readyQueue[1] = p2;
            numProcesses++;
        }
        if (p3 != NULL){
            readyQueue[2] = p3;
            numProcesses++;
        }
        if (p4 != NULL){
            readyQueue[3] = p4;
            numProcesses++;
        }
    }

    return;
}

void execute_script(struct pcb *process){
    
    int numLines = (*process).length;

    for(int i = 0; i < numLines; i++){
        
        char* line;
        int index = (*process).startPos + (*process).pc;


        //executing command at index in the shellmemory
        parseInput(mem_get_value_from_index(index));
        (*process).pc++;
    }

    if((*process).pc == ((*process).length)){
        head = (*process).nextPCB;
        free(process);
        process = NULL;
        numProcesses--; 
    }

    return;
}

void execute_script_lines_RR(struct pcb *process, int numLinesToExecute){

    for(int i = 0 ; i < numLinesToExecute ; i++){

        char* line;
        int index = (*process).startPos + (*process).pc;

        //executing command at index in the shellmemory
        parseInput(mem_get_value_from_index(index));
        (*process).pc++;

        if((*process).pc == ((*process).length)){
            
            //if it is the last procces in the queue, the head is set to null
            if((*process).nextPCB == process){
                head = NULL;
                free(process);
                process = NULL; 
                numProcesses--;
                return;
            }
    
            /*        
            if there are more processes in the queue, the current process is removed.
            the pcb pointing to the current proccess will instead point to what the current process was pointing to
            */
            struct pcb *previousPCB = find_previous_PCB(process); 
            struct pcb *nextPCB = (*process).nextPCB;
            (*previousPCB).nextPCB = nextPCB;
            head = nextPCB;
            free(process);
            process = NULL;
            numProcesses--; 
            return;
        }
    }

    head = (*process).nextPCB;

    return;
}

void execute_script_lines_AGING(struct pcb *process){   

    char* line;
    int index = (*process).startPos + (*process).pc;

    //executing command at index in the shellmemory
    parseInput(mem_get_value_from_index(index));
    (*process).pc++;

    if((*process).pc == ((*process).length)){
        free(process);
        process = NULL;
        numProcesses--;
        shift_pcbs();
    }

}

int scheduleFCFS(struct pcb *p1, struct pcb *p2, struct pcb *p3, struct pcb *p4){

    add_pcbs_to_ready_queue_FCFS(p1, p2, p3,p4);
       
    while(head != NULL){
        execute_script(head);
    }
    return 0;
}

int scheduleSJF(struct pcb *p1, struct pcb *p2, struct pcb *p3, struct pcb *p4){

    add_pcbs_to_ready_queue_SJF(p1, p2, p3,p4);

    while(head != NULL){
        execute_script(head);
    }

    return 0;
}

int scheduleRR(struct pcb *p1, struct pcb *p2, struct pcb *p3, struct pcb *p4){
    
    add_pcbs_to_ready_queue_RR(p1, p2, p3,p4);

    while(head != NULL){
        execute_script_lines_RR(head,2);
    }

    return 0;
}

int scheduleRR30(struct pcb *p1, struct pcb *p2, struct pcb *p3, struct pcb *p4){
    
    add_pcbs_to_ready_queue_RR(p1, p2, p3,p4);

    while(head != NULL){
        execute_script_lines_RR(head,30);
    }

    return 0;
}

int scheduleAGING(struct pcb *p1, struct pcb *p2, struct pcb *p3, struct pcb *p4){

    add_pcbs_to_ready_queue_AGING(p1,p2,p3,p4);

    sort_pcbs_by_aging_score(numProcesses);  
    head = readyQueue[0];
    
    while(head != NULL){
        execute_script_lines_AGING(head);
        decrement_aging_score();
        sort_pcbs_by_aging_score(numProcesses);  
        head = readyQueue[0];
    }

    return 0;
}

int scheduler(struct pcb *p1, struct pcb *p2, struct pcb *p3, struct pcb *p4, char *policy){

    initialize_ready_queue();

    if (strcmp("FCFS", policy) == 0){
        return scheduleFCFS(p1, p2, p3,p4);
    }
    else if (strcmp("SJF", policy) == 0){
        return scheduleSJF(p1, p2, p3,p4);
    }
    else if (strcmp("RR", policy) == 0){
        return scheduleRR(p1, p2, p3,p4);
    } 
    else if (strcmp("RR30", policy) == 0){
        return scheduleRR30(p1, p2, p3,p4);
    } 
    else if (strcmp("AGING", policy) == 0){
        return scheduleAGING(p1, p2, p3,p4);
    }
    else{
        return -1;
    }
}

void printAgingScore(char *filename){

	FILE *f = fopen(filename, "a");
	fprintf(f,"-------------start---------------\n");
    for(int i = 0 ; i < numProcesses ; i++){
        fprintf(f,"name : %s\t\t\t\t\t score : %d \n", (*readyQueue[i]).pid, (*readyQueue[i]).agingScore); 
    }
     fprintf(f,"-------------end---------------\n");

	fclose(f);
}

