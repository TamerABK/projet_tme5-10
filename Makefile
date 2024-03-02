CFLAGS = -g -Wno-unused-parameter -ggdb
CC = gcc

LIST= ChaineMain

all: ChaineMain

Chaine: Chaine.o Chaine.h SVGwriter.h
	$(CC) -o $@ $(CFLAGS) $^ -lm

SVGwriter: SVGwriter.o SVGwriter.h
	$(CC) -o $@ $(CFLAGS) $^

ChaineMain: Chaine.o SVGwriter.o ChaineMain.o
	$(CC) -o $@ $(CFLAGS) $^ -lm

clean:
	rm -f *.o *~ *.html $(LIST)