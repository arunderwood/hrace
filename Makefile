CC=gcc
CFLAGS=-Wall -lpthread -lrt 

hRace: hRace.c oterm.c
	$(CC) -o hRace hRace.c oterm.c $(CFLAGS)

clean: 
	  $(RM) hRace
