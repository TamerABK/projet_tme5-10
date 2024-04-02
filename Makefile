CFLAGS = -g -Wno-unused-parameter -ggdb
CC = gcc

LIST= ChaineMain ReconstitueReseau

all: ChaineMain ReconstitueReseau

Chaine: Chaine.o Chaine.h SVGwriter.h
	$(CC) -o $@ $(CFLAGS) $^ -lm

Reseau: Reseau.o Chaine.h SVGwriter.h
	$(CC) -o $@ $(CFLAGS) $^ -lm

Hachage: Hachage.o Chaine.o Reseau.o 
	$(CC) -o $@ $(CFLAGS) $^ -lm

ArbreQuat:	Chaine.o Reseau.o ArbreQuat.o
	$(CC) -o $@ $(CFLAGS) $^ -lm

SVGwriter: SVGwriter.o SVGwriter.h
	$(CC) -o $@ $(CFLAGS) $^

ChaineMain: Chaine.o SVGwriter.o ChaineMain.o
	$(CC) -o $@ $(CFLAGS) $^ -lm

ReconstitueReseau: ReconstitueReseau.o Chaine.o SVGwriter.o Reseau.o Hachage.o ArbreQuat.o
	$(CC) -o $@ $(CFLAGS) $^ -lm

clean:
	rm -f *.o *~ *.html $(LIST)