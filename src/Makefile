CC=gcc
CFLAGS= -lpthread -g -D FRAMESIZE=$(framesize) -D VARMEMSIZE=$(varmemsize)

mysh: shell.c interpreter.c shellmemory.c
	$(CC) $(CFLAGS) -c shell.c interpreter.c shellmemory.c pcb.c ready_queue.c page_table.c
	$(CC) $(CFLAGS) -o testing/mysh shell.o interpreter.o shellmemory.o pcb.o ready_queue.o page_table.o

clean: 
	rm testing/mysh; rm *.o; 
