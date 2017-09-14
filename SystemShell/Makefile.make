edit : init.o getty.o sh.o 

init.o : init.c 
	gcc -o init init.c
getty.o : getty.c 
	gcc -o getty getty.c
sh.o : sh.c 
	gcc -o sh sh.c

clean :
	rm edit init.o getty.o sh.o

