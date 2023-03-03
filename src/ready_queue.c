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

void initialize_ready_queue(){
    for (int i = 0; i < QUEUE_LENGTH; i++){
        readyQueue[i] = NULL;
    }
}

void add_pcb_to_ready_queue(struct pcb *p){
 
    for (int i = 0; i < QUEUE_LENGTH; i++){
        if (readyQueue[i] == NULL){
            readyQueue[i] = p;
            break;
        }
    }
}

void sort_pcbs_by_shortest_length(struct pcb *arr[],int numProcesses){

    struct pcb *temp = NULL;

    for (int i = 0; i < numProcesses; i++) {     
            for (int j = i+1; j < numProcesses; j++) {     
                if((*arr[i]).length > (*arr[j]).length) {    
                    temp = arr[i];    
                    arr[i] = arr[j];    
                    arr[j] = temp;    
                }     
            }     
        } 

}

void add_pcbs_to_ready_queue_FCFS(struct pcb *p1, struct pcb *p2, struct pcb *p3){

    head = NULL; 
    if (p1 != NULL){
        add_pcb_to_ready_queue(p1);
        if (p2 != NULL){
            (*p1).nextPCB = p2;
            add_pcb_to_ready_queue(p2);
        }
        if (p3 != NULL){
            (*p2).nextPCB = p3;
            add_pcb_to_ready_queue(p3);
        }
        head = p1;
    }
}

void add_pcbs_to_ready_queue_SJF(struct pcb *p1, struct pcb *p2, struct pcb *p3){
    
    int numProcesses = 0;
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

        sort_pcbs_by_shortest_length(readyQueue,numProcesses);

        for(int i = 0 ; i < numProcesses; i++){
            if(i != 0){
                (*readyQueue[i-1]).nextPCB = readyQueue[i];
            }
        }

        head = readyQueue[0];
    }

 
}

void execute_script_lines(struct pcb *process, int numLines){

    for(int i = 0; i < numLines; i++){
        
        char* line;
        int index = (*process).startPos + (*process).pc;

        parseInput(mem_get_value_from_index(index));
        (*process).pc++;
    }

    if((*process).pc == ((*process).length)){
        head = (*process).nextPCB;
        free(process);
    }
}

int scheduleFCFS(struct pcb *p1, struct pcb *p2, struct pcb *p3){

    add_pcbs_to_ready_queue_FCFS(p1, p2, p3);
       
    while(head != NULL){
        execute_script_lines(head,(*head).length);
    }
    return 0;
}

int scheduleSJF(struct pcb *p1, struct pcb *p2, struct pcb *p3){

    add_pcbs_to_ready_queue_SJF(p1, p2, p3);

    while(head != NULL){
        execute_script_lines(head,(*head).length);
    }

    return 0;
}

int scheduleRR(struct pcb *p1, struct pcb *p2, struct pcb *p3){

}

int scheduleAGING(struct pcb *p1, struct pcb *p2, struct pcb *p3){

}

int scheduler(struct pcb *p1, struct pcb *p2, struct pcb *p3, char *policy){

    initialize_ready_queue();

    if (strcmp("FCFS", policy) == 0){
        return scheduleFCFS(p1, p2, p3);
    }
    else if (strcmp("SJF", policy) == 0){
        return scheduleSJF(p1, p2, p3);
    }
    else if (strcmp("RR", policy) == 0){
        return scheduleRR(p1, p2, p3);
    }
    else if (strcmp("AGING", policy) == 0){
        return scheduleAGING(p1, p2, p3);
    }
    else{
        return -1;
    }
}

