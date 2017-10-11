all: semaphores semaphores2 trenes mirapendejo trenes1 trenes2
semaphores:
	gcc -c semaphore.c
semaphores2: 
	gcc -c semaphore2.c
trenes:
	gcc -c trenes.c
mirapendejo:
	gcc -c trenes2.c
trenes1:
	gcc -o trenes trenes.o semaphore.o -lpthread
trenes2:
	gcc -o trenes2 trenes2.o semaphore2.o -lpthread
