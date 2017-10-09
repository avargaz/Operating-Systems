edit : pi1.o pi2.o pi3.o

pi1.o : pi1.c
	gcc -o pi1 pi1.c
pi2.o : pi2.c
	gcc -o pi2 pi2.c
pi3.o : pi3.c
	gcc -o pi3 pi3.c

clean :
	rm edit pi1.o pi2.o pi3.o
