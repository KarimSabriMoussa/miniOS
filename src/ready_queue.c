#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "pcb.h"
#include "ready_queue.h"
#include "shellmemory.h"
#include "shell.h"
#include "pthread.h"


#define QUEUE_LENGTH 10
struct pcb *readyQueue[QUEUE_LENGTH];
struct pcb *head;
struct pcb *background_pcb;
int num_of_processes;
char *scheduler_policy;
pthread_mutex_t pcb_lock;

/*
    method signatures
*/
int scheduler(struct pcb *p1, struct pcb *p2, struct pcb *p3, struct pcb *background, int multithreading,char *policy);
int scheduleFCFS(struct pcb *p1, struct pcb *p2, struct pcb *p3);
int scheduleSJF(struct pcb *p1, struct pcb *p2, struct pcb *p3);
int scheduleRR(struct pcb *p1, struct pcb *p2, struct pcb *p3);
int scheduleRR30(struct pcb *p1, struct pcb *p2, struct pcb *p3);
int scheduleAGING(struct pcb *p1, struct pcb *p2, struct pcb *p3);

void execute_script(struct pcb *process);
void execute_script_lines_AGING(struct pcb *process);
void execute_script_lines_RR(struct pcb *process, int numLinesToExecute);

void add_pcbs_to_ready_queue_FCFS(struct pcb *p1, struct pcb *p2, struct pcb *p3);
void add_pcbs_to_ready_queue_SJF(struct pcb *p1, struct pcb *p2, struct pcb *p3);
void add_pcbs_to_ready_queue_RR(struct pcb *p1, struct pcb *p2, struct pcb *p3);
void add_pcbs_to_ready_queue_AGING(struct pcb *p1, struct pcb *p2, struct pcb *p3);

int add_pcb_to_ready_queue(struct pcb *p);
int add_pcb_to_ready_queue_FCFS(struct pcb *p);
int add_pcb_to_ready_queue_SJF(struct pcb *p);
int add_pcb_to_ready_queue_RR(struct pcb *p);
int add_pcb_to_ready_queue_AGING(struct pcb *p);

void printAgingScore(char *filename);
void printReadyQueue(char *filename);
struct pcb *find_previous_PCB(struct pcb *p);
struct pcb *find_longest_process_less_than(int length);
void decrement_aging_score();
void shift_pcbs_left_from_index(int index);
void sort_pcbs_by_aging_score(int num_of_processes);
void initialize_ready_queue();
int get_index_of_process(struct pcb *p);

void multithreading();
void *exec_thread();




// initialises the queue to all NULL pointers
void initialize_ready_queue(){
    for (int i = 0; i < QUEUE_LENGTH; i++){
        readyQueue[i] = NULL;
    }

    return;
}

/*
    adds pcb p to the tail of the queue
    the element before it in the queue will point to p
    p points to null
    if p is the first element in the queue it becomes the head
*/ 
int add_pcb_to_ready_queue_FCFS(struct pcb *p){

    for (int i = 0; i < QUEUE_LENGTH; i++){
        if (readyQueue[i] == NULL){
            readyQueue[i] = p;
            num_of_processes++;

            if (i > 0){
                (*readyQueue[i - 1]).nextPCB = p;
            }else{
                head = p;
            }

            return 0;
        }
    }

    return -1;
}

/*
    adds pcb p to the tail of the queue
    let pcb k be the pcb with the largest length that is smaller than p's length 
        p points to k's nextPCB
        k points to p
    if there is no background pcb and there is no shorter process than p
        then p becomes the head
*/  
int add_pcb_to_ready_queue_SJF(struct pcb *p){

    struct pcb* prevPCB;
    struct pcb* temp;
    
     for (int i = 0; i < QUEUE_LENGTH; i++){
        if (readyQueue[i] == NULL){

            prevPCB = find_longest_process_less_than((*p).length);

            readyQueue[i] = p;
            num_of_processes++;

            if(prevPCB == NULL){
                if(background_pcb != NULL && head == background_pcb){
                    (*p).nextPCB = (*head).nextPCB;
                    (*head).nextPCB = p;
                }else{
                    (*p).nextPCB = head;
                    head = p;
                }
            }else{
                (*p).nextPCB = (*prevPCB).nextPCB;
                (*prevPCB).nextPCB = p;
            }

            return 0;
        }
    }

    return -1;
}

/*
    adds pcb p to the tail of the queue
    if p isn't the only pcb in the queue 
        then p will point to k's nextPCB
        and the pcb k before it will point to it 
    if p is the only pcb in the queue
        then it points to itself and it becomes the head
*/  
int add_pcb_to_ready_queue_RR(struct pcb *p){
    
    for (int i = 0; i < QUEUE_LENGTH; i++){
        if (readyQueue[i] == NULL){

            readyQueue[i] = p;
            num_of_processes++;

            if (i > 0){
                (*p).nextPCB = (*readyQueue[i - 1]).nextPCB;
                (*readyQueue[i - 1]).nextPCB = p;
            }else{
                (*p).nextPCB = p;
                head = p;
            }

            return 0;
        }
    }

    return -1;
}

/*
    adds pcb p to the tail of the queue
    the head is determined later by sorting the readyQueue according to the agingScore if elements
*/  
int add_pcb_to_ready_queue_AGING(struct pcb *p){
    
    for (int i = 0; i < QUEUE_LENGTH; i++){
        if (readyQueue[i] == NULL){

            readyQueue[i] = p;
            num_of_processes++;

            return 0;
        }
    }

    return -1;
}

int add_pcb_to_ready_queue(struct pcb *p){

    if (strcmp("FCFS", scheduler_policy) == 0){
        return add_pcb_to_ready_queue_FCFS(p);
    }
    else if (strcmp("SJF", scheduler_policy) == 0){
        return add_pcb_to_ready_queue_SJF(p);
    }
    else if (strcmp("RR", scheduler_policy) == 0){
        return add_pcb_to_ready_queue_RR(p);
    }
    else if (strcmp("RR30", scheduler_policy) == 0){
        return add_pcb_to_ready_queue_RR(p);
    }
    else if (strcmp("AGING", scheduler_policy) == 0){
        return add_pcb_to_ready_queue_AGING(p);
    }
    else{
        return -2;
    }
}

void add_pcbs_to_ready_queue_FCFS(struct pcb *p1, struct pcb *p2, struct pcb *p3){

    num_of_processes = 0;
    head = NULL;

    if (background_pcb != NULL){
        add_pcb_to_ready_queue_FCFS(background_pcb);
    }
    if (p1 != NULL){
        add_pcb_to_ready_queue_FCFS(p1);
    }
    if (p2 != NULL){
        add_pcb_to_ready_queue_FCFS(p2);
    }
    if (p3 != NULL){
        add_pcb_to_ready_queue_FCFS(p3);
    }

    return;
}

void add_pcbs_to_ready_queue_SJF(struct pcb *p1, struct pcb *p2, struct pcb *p3){

    num_of_processes = 0;
    head = NULL;

    if (background_pcb != NULL){
        head = background_pcb;
    }
    if (p1 != NULL){
        add_pcb_to_ready_queue_SJF(p1);
    }
    if (p2 != NULL){
        add_pcb_to_ready_queue_SJF(p2);
    }
    if (p3 != NULL){
        add_pcb_to_ready_queue_SJF(p3);
    }

    return;
}

void add_pcbs_to_ready_queue_RR(struct pcb *p1, struct pcb *p2, struct pcb *p3){

    num_of_processes = 0;
    head = NULL;

    if (background_pcb != NULL){
        add_pcb_to_ready_queue_RR(background_pcb);
    }
    if (p1 != NULL){
        add_pcb_to_ready_queue_RR(p1);
    }
    if (p2 != NULL){
        add_pcb_to_ready_queue_RR(p2);
    }
    if (p3 != NULL){
        add_pcb_to_ready_queue_RR(p3);
    }

    return;
}

void add_pcbs_to_ready_queue_AGING(struct pcb *p1, struct pcb *p2, struct pcb *p3){

    num_of_processes = 0;
    head = NULL;

    if (background_pcb != NULL){
        add_pcb_to_ready_queue_AGING(background_pcb);
    }
    if (p1 != NULL){
        add_pcb_to_ready_queue_AGING(p1);
    }
    if (p2 != NULL){
        add_pcb_to_ready_queue_AGING(p2);
    }
    if (p3 != NULL){
        add_pcb_to_ready_queue_AGING(p3);
    }

    return;
}

void execute_script(struct pcb *process){

    int numLines = (*process).length;

    for (int i = 0; i < numLines; i++){

        char *line;
        int index = (*process).startPos + (*process).pc;

        // executing command at index in the shellmemory
        parseInput(mem_get_value_from_index(index));
        (*process).pc++;
    }

    if ((*process).pc == ((*process).length)){

        int i = get_index_of_process(process);

        head = (*process).nextPCB;
        free(process);
        process = NULL;
        num_of_processes--;

        if(i >= 0 && i < QUEUE_LENGTH){
            shift_pcbs_left_from_index(i);
        } 
    }

    return;
}

void execute_script_lines_RR(struct pcb *process, int numLinesToExecute){

    for (int i = 0; i < numLinesToExecute; i++) {

        char *line;
        int index = (*process).startPos + (*process).pc;

        // executing command at index in the shellmemory
        parseInput(mem_get_value_from_index(index));
        (*process).pc++;

        if ((*process).pc == ((*process).length)){

            // if it is the last procces in the queue, the head is set to null
            if ((*process).nextPCB == process){
                head = NULL;
                free(process);
                process = NULL;
                num_of_processes--;
                return;
            }

            /*
            if there are more processes in the queue, the current process is removed.
            the pcb pointing to the current proccess will instead point to what the current process's nextPCB
            all pcbs are shifted left in the readyqueue from where the current process was removed to fill in any gaps
            */
            int i = get_index_of_process(process);

            struct pcb *previousPCB = find_previous_PCB(process);
            struct pcb *nextPCB = (*process).nextPCB;
            (*previousPCB).nextPCB = nextPCB;
            head = nextPCB;
            free(process);
            process = NULL;
            num_of_processes--;

            if(i >= 0 && i < QUEUE_LENGTH){
                shift_pcbs_left_from_index(i);
            } 

            return;
        }
    }

    head = (*process).nextPCB;

    return;
}

void execute_script_lines_AGING(struct pcb *process){

    char *line;
    int index = (*process).startPos + (*process).pc;

    // executing command at index in the shellmemory
    parseInput(mem_get_value_from_index(index));

    (*process).pc++;

    if ((*process).pc == ((*process).length)) {
        int i = get_index_of_process(process);
        free(process);
        process = NULL;
        num_of_processes--;
        if(i >= 0 && i < QUEUE_LENGTH){
            shift_pcbs_left_from_index(i);
        } 
    }
}

int scheduleFCFS(struct pcb *p1, struct pcb *p2, struct pcb *p3){

    add_pcbs_to_ready_queue_FCFS(p1, p2, p3);

    while (head != NULL){
        execute_script(head);
    }
    return 0;
}

int scheduleSJF(struct pcb *p1, struct pcb *p2, struct pcb *p3){

    add_pcbs_to_ready_queue_SJF(p1, p2, p3);

    while (head != NULL){
        execute_script(head);
    }

    return 0;
}

int scheduleRR(struct pcb *p1, struct pcb *p2, struct pcb *p3){

    add_pcbs_to_ready_queue_RR(p1, p2, p3);

    while (head != NULL){
        execute_script_lines_RR(head, 2);
    }

    return 0;
}

int scheduleRR30(struct pcb *p1, struct pcb *p2, struct pcb *p3){

    add_pcbs_to_ready_queue_RR(p1, p2, p3);

    while (head != NULL){
        execute_script_lines_RR(head, 30);
    }

    return 0;
}

int scheduleAGING(struct pcb *p1, struct pcb *p2, struct pcb *p3){

    add_pcbs_to_ready_queue_AGING(p1, p2, p3);

    sort_pcbs_by_aging_score(num_of_processes);
    head = readyQueue[0];

    while (head != NULL){
        execute_script_lines_AGING(head);
        decrement_aging_score();
        sort_pcbs_by_aging_score(num_of_processes);
        head = readyQueue[0];
    }

    return 0;
}

int scheduler(struct pcb *p1, struct pcb *p2, struct pcb *p3, struct pcb *background, int multithreading, char *policy){

    initialize_ready_queue();
    scheduler_policy = policy;
    background_pcb = background;

    if(multithreading == 1){
        add_pcbs_to_ready_queue_RR(p1,p2,p3);
        multithreading();
    }else{
        if (strcmp("FCFS", policy) == 0){
            return scheduleFCFS(p1, p2, p3);
        }
        else if (strcmp("SJF", policy) == 0){
            return scheduleSJF(p1, p2, p3);
        }
        else if (strcmp("RR", policy) == 0){
            return scheduleRR(p1, p2, p3);
        }
        else if (strcmp("RR30", policy) == 0){
            return scheduleRR30(p1, p2, p3);
        }
        else if (strcmp("AGING", policy) == 0){
            return scheduleAGING(p1, p2, p3);
        }
        else{
            return -1;
        }
    }

    return 0;
}

void printAgingScore(char *filename){

    FILE *f = fopen(filename, "a");
    fprintf(f, "-------------start---------------\n");
    for (int i = 0; i < num_of_processes; i++){
        fprintf(f, "name : %s\t\t\t\t\t score : %d \n", (*readyQueue[i]).pid, (*readyQueue[i]).agingScore);
    }
    fprintf(f, "-------------end---------------\n");

    fclose(f);
}

void printReadyQueue(char *filename){

    FILE *f = fopen(filename, "w");
    
    for (int i = 0; i < num_of_processes; i++){
        char *nextPCB = "null";
        if(((*readyQueue[i]).nextPCB) != NULL){
            nextPCB = (*((*readyQueue[i]).nextPCB)).pid;
        }
        fprintf(f, "name : %s\t\t\t length: %d\t\t\t nextPCB : %s \n", (*readyQueue[i]).pid,(*readyQueue[i]).length , nextPCB);
    }

    fclose(f);
}

// This function finds which pcb in the ready queue that is pointing to p.
struct pcb *find_previous_PCB(struct pcb *p){
    for (int i = 0; i < QUEUE_LENGTH; i++){
        if (readyQueue[i] != NULL && (*readyQueue[i]).nextPCB == p){
            return readyQueue[i];
        }
    }

    return NULL;
}

void decrement_aging_score(){

    for (int i = 0; i < QUEUE_LENGTH; i++){
        if (readyQueue[i] != NULL && readyQueue[i] != head && (*readyQueue[i]).agingScore > 0){
            (*readyQueue[i]).agingScore--;
        }
    }
    return;
}

void shift_pcbs_left_from_index(int index){
    
    for (int i = index; i < QUEUE_LENGTH - 1; i++){
        readyQueue[i] = readyQueue[i + 1];
        if (readyQueue[i] == NULL){
            break;
        }
    }

    readyQueue[QUEUE_LENGTH - 1] = NULL;

    return;
}

struct pcb *find_longest_process_less_than(int length){

    struct pcb *result = NULL;

    for(int  i = 0 ; i < num_of_processes; i++){
        if((*readyQueue[i]).length <= length && (result == NULL || (*readyQueue[i]).length >= (*result).length)){
            result = readyQueue[i];
        }
    }

    return result;
}

void sort_pcbs_by_aging_score(int num_of_processes){

    struct pcb *temp = NULL;

    for (int i = 0; i < num_of_processes; i++){
        for (int j = i + 1; j < num_of_processes; j++){
            if ((*readyQueue[i]).agingScore > (*readyQueue[j]).agingScore) {
                temp = readyQueue[i];
                readyQueue[i] = readyQueue[j];
                readyQueue[j] = temp;
            }
        }
    }

    return;
}

int get_index_of_process(struct pcb *p){

    
    for(int i = 0 ; i < num_of_processes; i++){
        if(readyQueue[i] != NULL && p == readyQueue[i]){
            return i;
        }
    }

    return -1;
}

void multithreading(){

pthread_t thread1, thread2;

pthread_create(&thread1, NULL, exec_thread, NULL);
pthread_create(&thread2, NULL, exec_thread, NULL);
pthread_join(thread1, NULL);
pthread_join(thread2, NULL);

}

void *exec_thread(){

    int num_of_lines;

    if(strcmp("RR", scheduler_policy) == 0){
        num_of_lines = 2;
    }else if(strcmp("RR30", scheduler_policy) == 0){
        num_of_lines = 30;
    }


    while(head != NULL){

        struct pcb *pcb_to_execute = NULL;

        pthread_mutex_lock(&pcb_lock);
        if((*head).taken == 0){
            (*head).taken = 1;
            pcb_to_execute = head;
        }else{
            if((*head).nextPCB != head && (*((*head).nextPCB)).taken == 0){
                pcb_to_execute = (*head).nextPCB;
            }
        }
        pthread_mutex_unlock(&pcb_lock);

        if(pcb_to_execute != NULL){
            execute_script_lines_RR(pcb_to_execute, num_of_lines);
        }

    }

    return NULL;
}