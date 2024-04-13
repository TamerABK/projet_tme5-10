#include "Chaine.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

Chaines* generationAleatoire(int nbChaines, int nbPointsChaine, int xmax, int ymax) {
    Chaines* C = (Chaines*)malloc(sizeof(Chaines));
    if (C == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire pour les chaines.\n");
        exit(EXIT_FAILURE);
    }
    
    C->nbChaines = nbChaines;
    C->gamma = 1; // Valeur par défaut pour gamma
    
    srand(time(NULL));

    // première chaine
    C->chaines = (CellChaine*)malloc(sizeof(CellChaine));
    if (C->chaines == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire pour la première chaîne.\n");
        exit(EXIT_FAILURE);
    }

    CellChaine* currChaine = C->chaines;
    for (int i = 0; i < nbChaines; i++) {
        currChaine->points = (CellPoint*)malloc(sizeof(CellPoint));
        if (currChaine->points == NULL) {
            fprintf(stderr, "Erreur d'allocation de mémoire pour les points de la chaîne.\n");
            exit(EXIT_FAILURE);
        }

        CellPoint* currPoint = currChaine->points;
        for (int j = 0; j < nbPointsChaine; j++) {
            currPoint->x = (double)(rand() % (xmax + 1)); // Coordonnée x aléatoire
            currPoint->y = (double)(rand() % (ymax + 1)); // Coordonnée y aléatoire
            if (j < nbPointsChaine - 1) {
                currPoint->suiv = (CellPoint*)malloc(sizeof(CellPoint));
                if (currPoint->suiv == NULL) {
                    fprintf(stderr, "Erreur d'allocation de mémoire pour un point de la chaîne.\n");
                    exit(EXIT_FAILURE);
                }
                currPoint = currPoint->suiv;
            } else {
                currPoint->suiv = NULL;
            }
        }

        if (i < nbChaines - 1) {
            currChaine->suiv = (CellChaine*)malloc(sizeof(CellChaine));
            if (currChaine->suiv == NULL) {
                fprintf(stderr, "Erreur d'allocation de mémoire pour une nouvelle chaîne.\n");
                exit(EXIT_FAILURE);
            }
            currChaine = currChaine->suiv;
        } else {
            currChaine->suiv = NULL;
        }
    }

    return C;
}


int main() {
    int nbChaines = 5; // nb chaînes à générer
    int nbPointsChaine = 4; // nb points par chaîne
    int xmax = 100; 
    int ymax = 100; 

    
    Chaines* chainesAleatoires = generationAleatoire(nbChaines, nbPointsChaine, xmax, ymax);
    afficheChainesSVG(chainesAleatoires, "chaines_aleatoires.svg");

    liberer_chaine(chainesAleatoires);

    return 0;
}
