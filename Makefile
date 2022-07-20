CC=gcc
CFLAGS=-Wall -Wextra -std=c99

build: main

main: main.c
	$(CC) $(CFLAGS) main.c -o main

pack: 
	zip -FSr 314CA_MaciucaALexandruPetru_Tema1.zip README Makefile *.c *.h

clean:
	rm *.o
	rm main

.PHONY: pack clean
