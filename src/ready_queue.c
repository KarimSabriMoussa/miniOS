#include <stdio.h>
#include "pcb.h"
#include "ready_queue.h"
#include "shellmemory.h"

#define QUEUE_LENGTH 10
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
            *readyQueue[i] = *p;
            break;
        }
    }
}

add_pcbs_to_ready_queue(struct pcb *p1, struct pcb *p2, struct pcb *p3, char *policy){

    if (strcmp("FCFS", policy) == 0  || strcmp("RR", policy) == 0){
        if (p1 != NULL){
            add_pcb_to_ready_queue(p1);
        }
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

void execute_script_lines(struct pcb* process, int numLines){
    for(int i = 0; i < numLines; i++){
        
        char * line;
        int index = (*process).startPos + (*process).pc;

        parseInput(mem_get_value_from_index(index));
        (*process).pc++;
    }

    if((*process).pc == ((*process).length - 1)){
        head = (*process).nextPCB;
    }
}

int scheduleFCFS(){
    while(head != NULL){
        execute_script_lines(head,(*head).length);
    }
}


int scheduler(struct pcb *p1, struct pcb *p2, struct pcb *p3, char *policy){

    initialize_ready_queue();
    add_pcbs_to_ready_queue(p1,p2,p3,policy);

    if (strcmp("FCFS", policy) == 0){
        return scheduleFCFS();
    }
    else if (strcmp("SJF", policy) == 0){
        return scheduleSJF();
    }
    else if (strcmp("RR", policy) == 0){
        return scheduleRR();
    }
    else if (strcmp("AGING", policy) == 0){
        return scheduleAGING();
    }
    else{
        return -1;
    }
}