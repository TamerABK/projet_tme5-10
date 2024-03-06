#include "Hachage.h"
#include "Chaine.h"
#include "SVGwriter.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define A (sqrt(5)-1)/2

// Fonction generation clef
int fonctionClef(double x,double y){
  return (int)ceil((y+(x+y)*(x+y+1))/2);
}


int fonctionHachage(int clef,int m){

    return floor(m*(clef*A-floor(clef*A)));
}

// Fonction creation

TableHachage* cree_table_hachage(int m){

    TableHachage* table_H=(TableHachage*)malloc(sizeof(TableHachage));
    check_pointer(table_H);

    table_H->tailleMax=m;
    table_H->nbElement=0;
    table_H->T=(CellNoeud*)malloc(m*sizeof(CellNoeud*));

    for (int i=0;i<m;i++){
        table_H->T[i]=NULL;
    }
    return table_H;
}

Noeud* rechercheCreeNoeudHachage(Reseau *R,TableHachage *H, double x, double y){

    check_pointer(R);
    check_pointer(H);

    int pos=fonctionHachage(fonctionClef(x,y),H->tailleMax);
    CellNoeud* Cn=H->T[pos];
    Noeud* N;

    while (Cn)
    {
        if (Cn->nd->x==x && Cn->nd->y==y)
        {
            return Cn->nd;
        }
        Cn=Cn->suiv;
    }
    
    if (!Cn)
    {
        // Ajouter au reseau
        N=(Noeud*)malloc(sizeof(Noeud));
        N->x=x;N->y=y;N->num=R->nbNoeuds+1;
        R->nbNoeuds++;
        Cn=(CellNoeud*)malloc(sizeof(CellNoeud));
        Cn->nd=N;Cn->suiv=R->noeuds;
        R->noeuds=Cn;
        
        // Ajouter a la table de hachage
        Cn=(CellNoeud*)malloc(sizeof(CellNoeud));
        Cn->nd=N;Cn->suiv=H->T[pos];
        H->T[pos]=Cn;
        H->nbElement++;
        
        return N;
    }
}
