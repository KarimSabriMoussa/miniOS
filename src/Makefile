CC=gcc
CFLAGS=

mysh: shell.c interpreter.c shellmemory.c
	$(CC) $(CFLAGS) -c shell.c interpreter.c shellmemory.c pcb.c ready_queue.c
	$(CC) $(CFLAGS) -o mysh shell.o interpreter.o shellmemory.o pcb.o ready_queue.o

clean: 
	rm mysh; rm *.o
