#ifndef ready_queue_h
#define ready_queue_h

int scheduler(struct pcb *p1, struct pcb *p2, struct pcb *p3, struct pcb *background, int multithreaded,char *policy);
int add_pcb_to_ready_queue(struct pcb *p);
void join_pthreads();
void set_quit_flag(int value);

#endif