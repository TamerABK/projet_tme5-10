#ifndef __GRAPHE_H__
#define __GRAPHE_H__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "Reseau.h"
#include "Struct_File.h"

typedef struct 
{
    int u,v;
} Arete;

typedef struct cellule_arete
{
    Arete *a;
    struct cellule_arete* suiv;    
} Cellule_arete;

typedef struct 
{
    int num;
    double x,y;
    Cellule_arete* L_voisin;
} Sommet;

typedef struct 
{
    int e1,e2;
} Commod;

typedef struct 
{
    int nbsom;
    Sommet** T_som;
    int gamma;
    int nbcommod;
    Commod* T_commod;
} Graphe;


Sommet* creerSommet(Noeud* N);
void ajout_arrete(Sommet* s1,Sommet* s2);
Commod creerCommod(CellCommodite* C);
Graphe* creerGraphe(Reseau* r);
int determine_voisin(Cellule_arete* C,int num);
int* PP_chemin(Graphe* G,int debut);
void affiche_graphe(Graphe* G);

#endif 


