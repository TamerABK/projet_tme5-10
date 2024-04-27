#include "Reseau.h"
#include "Chaine.h"
#include "SVGwriter.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void ajoutevoisin(Noeud* v1,Noeud* v2){
    check_pointer(v1);
    check_pointer(v2);
    
    // Vérifie si les deux noeuds sont déjà voisins
    CellNoeud* cell_curr = v1->voisins;
    while (cell_curr) {
        if (cell_curr->nd->num == v2->num) {
            return;
        }
        cell_curr = cell_curr->suiv;
    }
    
    // Ajoute le voisin s'il n'existe pas déjà
    CellNoeud* cell_cree = (CellNoeud*)malloc(sizeof(CellNoeud));
    check_pointer(cell_cree);
    cell_cree->nd = v2;
    cell_cree->suiv = v1->voisins;
    v1->voisins = cell_cree;
}

Noeud* rechercheCreeNoeudListe(Reseau *R, double x, double y) {
    check_pointer(R);

    CellNoeud* Cn = R->noeuds;
    Noeud* N = NULL; // mettre à null

    while (Cn != NULL) {
        N = Cn->nd;
    // printf("Noeud %d : x = %f, y = %f\n", N->num, N->x, N->y); // printf pour debug
    if (N->x == x && N->y == y) {
        return N;
    }
        Cn = Cn->suiv;
    }

    // si noeud pas trouvé, alors le créer
    N = (Noeud*)malloc(sizeof(Noeud));
    if (N == NULL) {
        fprintf(stderr, "erreur de l'allocation mémoire pour le noeud\n");
        exit(EXIT_FAILURE);
    }
    N->x = x;
    N->y = y;
    //ici on incrémente direct le nb de noeuds au lieu d'utiliser un counter
    N->num = R->nbNoeuds + 1;
    R->nbNoeuds++;

    // création cellule noeud et insertion en tête
    CellNoeud* cell_noeud = (CellNoeud*)malloc(sizeof(CellNoeud));
    if (cell_noeud == NULL) {
        //cas échec de l'allocation mémoire
        fprintf(stderr, "erreur échec de l'allocation mémoire pour la cellule noeud\n");
        free(N);
        exit(EXIT_FAILURE);
    }
    cell_noeud->nd = N;
    cell_noeud->suiv = R->noeuds;
    R->noeuds = cell_noeud;

    return N;
}



Reseau* reconstitueReseauListe(Chaines *C) {
    check_pointer(C);

    // Création et initialisation du réseau à rendre
    Reseau *reseau = (Reseau*)malloc(sizeof(Reseau));
    if (reseau == NULL) {
        fprintf(stderr, "échec allocation mémoire\n");
        exit(EXIT_FAILURE);
    }
    reseau->gamma = C->gamma;

    CellChaine *Cell_curr = C->chaines;
    CellPoint *point_curr;
    reseau->nbNoeuds = 0;
    Noeud* noeud_a_relier = NULL;

    while (Cell_curr) {
        point_curr = Cell_curr->points;

        CellCommodite *commodite_cree = NULL;

        while (point_curr) {
            Noeud* noeud_cree = rechercheCreeNoeudListe(reseau, point_curr->x, point_curr->y);
            reseau->nbNoeuds++;
            
            if (commodite_cree == NULL) {
                commodite_cree = (CellCommodite*)malloc(sizeof(CellCommodite));
                if (commodite_cree == NULL) {
                    fprintf(stderr, "échec allocation mémoire pour la commodité\n");
                    liberer_reseau(reseau); // libérer absolument même s'il y a erreur d'allocation
                    exit(EXIT_FAILURE);
                }
                commodite_cree->extrA = noeud_cree;
            }

            if (noeud_a_relier) {
                ajoutevoisin(noeud_cree, noeud_a_relier);
                ajoutevoisin(noeud_a_relier, noeud_cree);
            }

            noeud_a_relier = noeud_cree;
            point_curr = point_curr->suiv;
        }

        // création d'une commodité si elle a été initialisée
        if (commodite_cree != NULL) {
            commodite_cree->extrB = noeud_a_relier;
            commodite_cree->suiv = reseau->commodites;
            reseau->commodites = commodite_cree;
        }

        Cell_curr = Cell_curr->suiv;
        noeud_a_relier = NULL;
    }

    return reseau;
}


void afficheReseauSVG(Reseau *R, char* nomInstance){
    CellNoeud *courN,*courv;
    SVGwriter svg;
    double maxx=0,maxy=0,minx=1e6,miny=1e6;

    courN=R->noeuds;
    while (courN!=NULL){
        if (maxx<courN->nd->x) maxx=courN->nd->x;
        if (maxy<courN->nd->y) maxy=courN->nd->y;
        if (minx>courN->nd->x) minx=courN->nd->x;
        if (miny>courN->nd->y) miny=courN->nd->y;
        courN=courN->suiv;
    }
    SVGinit(&svg,nomInstance,500,500);
    courN=R->noeuds;
    while (courN!=NULL){
        SVGpoint(&svg,500*(courN->nd->x-minx)/(maxx-minx),500*(courN->nd->y-miny)/(maxy-miny));
        courv=courN->nd->voisins;
        while (courv!=NULL){
            if (courv->nd->num<courN->nd->num)
                SVGline(&svg,500*(courv->nd->x-minx)/(maxx-minx),500*(courv->nd->y-miny)/(maxy-miny),500*(courN->nd->x-minx)/(maxx-minx),500*(courN->nd->y-miny)/(maxy-miny));
            courv=courv->suiv;
        }
        courN=courN->suiv;
    }
    SVGfinalize(&svg);
}

int nbCommodites(Reseau *R){
    check_pointer(R);
    int counter=0;
    CellCommodite* cell_commodite=R->commodites;

    while (cell_commodite)
    {
        counter++;
        cell_commodite=cell_commodite->suiv;
    }

    return counter;
}

int nbLiaisons(Reseau *R){
    check_pointer(R);

    int counter=0;
    CellNoeud  *cell_noeud_vois;
    CellNoeud *cell_noeud_curr=R->noeuds;

    while (cell_noeud_curr)
    {
        cell_noeud_vois=cell_noeud_curr->nd->voisins;

        while (cell_noeud_vois)
        {
            if (cell_noeud_curr->nd->num < cell_noeud_vois->nd->num)
            {
                counter++;
            }
            cell_noeud_vois=cell_noeud_vois->suiv;
        }

        cell_noeud_curr=cell_noeud_curr->suiv;
        
    }

    return counter;
}

void ecrireReseau(Reseau *R, FILE *F){
    check_pointer(R);
    check_pointer(F);

    fprintf(F,"NbNoeuds: %d\n",R->nbNoeuds);
    fprintf(F,"NbLiaisons: %d\n",nbLiaisons(R));
    fprintf(F,"NbCommodites: %d\n",nbCommodites(R));
    fprintf(F,"Gamma: %d\n\n",R->gamma);

    CellNoeud* cell_noeud_curr=R->noeuds;

    while (cell_noeud_curr)
    {
        fprintf(F,"v %d %f %f\n",cell_noeud_curr->nd->num,cell_noeud_curr->nd->x,cell_noeud_curr->nd->y);
        cell_noeud_curr=cell_noeud_curr->suiv;
    }
    
    fprintf(F,"\n");

    cell_noeud_curr=R->noeuds;
    CellNoeud  *cell_noeud_vois;

    while (cell_noeud_curr)
    {
        cell_noeud_vois=cell_noeud_curr->nd->voisins;

        while (cell_noeud_vois)
        {
            if (cell_noeud_curr->nd->num < cell_noeud_vois->nd->num)
            {
                fprintf(F,"l %d %d\n",cell_noeud_curr->nd->num,cell_noeud_vois->nd->num);
            }
            cell_noeud_vois=cell_noeud_vois->suiv;
        }

        cell_noeud_curr=cell_noeud_curr->suiv;
        
    }

    fprintf(F,"\n");

    CellCommodite* cell_commod_curr=R->commodites;

    while (cell_commod_curr)
    {
        fprintf(F,"k %d %d\n",cell_commod_curr->extrA->num,cell_commod_curr->extrB->num);
        cell_commod_curr=cell_commod_curr->suiv;
    }
}

void liberer_reseau(Reseau *R) {
    check_pointer(R);

    // liberer liste commodités
    CellCommodite *commod_curr = R->commodites;
    CellCommodite *commod_suiv;
    while (commod_curr) {
        commod_suiv = commod_curr->suiv;
        free(commod_curr);
        commod_curr = commod_suiv;
    }

    // liberer liste des noeuds
    CellNoeud *noeud_curr = R->noeuds;
    CellNoeud *noeud_suiv;
    while (noeud_curr) {
        // liberer la liste des voisins pour chaque noeud
        CellNoeud *voisin_curr = noeud_curr->nd->voisins;
        CellNoeud *voisin_suiv;
        while (voisin_curr) {
            voisin_suiv = voisin_curr->suiv;
            free(voisin_curr);
            voisin_curr = voisin_suiv;
        }
        // free le noeud lui-même
        free(noeud_curr->nd);

        noeud_suiv = noeud_curr->suiv;
        free(noeud_curr);
        noeud_curr = noeud_suiv;
    }

    free(R);
}
