threads: main.o module.o bathroom.h
	gcc -g0 -Wall -pthread main.o module.o bathroom.h -o threads
main.o: main.c bathroom.h
	gcc -g0 -Wall -c main.c
module.o: module.c bathroom.h
	gcc -g0 -Wall -c module.c
clean:
	rm -f threads main.o module.o
all: threads main.o module.o
