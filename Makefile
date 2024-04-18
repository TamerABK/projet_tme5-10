CFLAGS = -g -Wno-unused-parameter -ggdb
CC = gcc

LIST= ChaineMain ReconstitueReseau GenereAlea

all: ChaineMain ReconstitueReseau GenereAlea

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

Struct_File: Struct_File.o Struct_File.h
	$(CC) -o $@ $(CFLAGS) $^ -lm

Graphe: Graphe.o Struct_File.o Reseau.o Chaine.o Struct_File.h
	$(CC) -o $@ $(CFLAGS) $^ -lm

ReconstitueReseau: ReconstitueReseau.o Chaine.o SVGwriter.o Reseau.o Hachage.o ArbreQuat.o Graphe.o Struct_File.o
	$(CC) -o $@ $(CFLAGS) $^ -lm

GenereAlea: GenereAlea.o Chaine.o SVGwriter.o
	$(CC) -o $@ $(CFLAGS) $^ -lm	

clean:
	rm -f *.o *~ *.html $(LIST)
