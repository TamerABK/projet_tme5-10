#include "Reseau.h"
#include "Chaine.h"
#include "SVGwriter.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


Noeud* rechercheCreeNoeudListe(Reseau *R,double x, double y,int* counter,int* signal)
{
    check_pointer(R);

    CellNoeud* Cn=R->noeuds;
    Noeud* N;

    while (Cn)
    {
        N=Cn->nd;
        if(N->x==x && N->y==y)
        {
            if(signal) *signal=0;
            return N;
        }
        Cn=Cn->suiv;
    }

    
        N=(Noeud*)malloc(sizeof(Noeud));
        N->x=x;N->y=y;N->num=R->nbNoeuds+1;
        R->nbNoeuds++;
        Cn=(CellNoeud*)malloc(sizeof(CellNoeud));
        Cn->nd=N;Cn->suiv=R->noeuds;
        R->noeuds=Cn;
        if(counter) (*counter)++;
        if(signal) *signal=1;
    

    return N;
    
}

Reseau* reconstitueReseauListe(Chaines *C){

    check_pointer(C);

    //Cree et intialise gamma du reseau a rendre    

    Reseau *reseau=(Reseau*)malloc(sizeof(Reseau));
    reseau->gamma=C->gamma;
    // Variable pour parcourir la chaine,compter les noeuds et cree les noeuds et commodite
    CellChaine *Cell_curr=C->chaines;
    CellPoint* point_curr;
    int signal,counter=0;
    Noeud* noeud_cree,* noeud_a_relier;
    CellNoeud* cellnoeud_cree;
    CellCommodite* commodite_cree;

    while (Cell_curr)
    {
        point_curr=Cell_curr->points;

        do
        {
            noeud_cree= rechercheCreeNoeudListe(reseau,point_curr->x,point_curr->y,&counter,&signal);

            // Si un noeud a ete cree rechercheCreeNoeudListe met le signal a un et une CellNoeud est cree et ajoute au reseau
            if (signal)
            {
                cellnoeud_cree=(CellNoeud*)malloc(sizeof(CellNoeud));
                cellnoeud_cree->nd=noeud_cree;
                cellnoeud_cree->suiv=reseau->noeuds;
                reseau->noeuds=cellnoeud_cree;
            }
            // Cree une commodite entre le noeud cree ou courrant et le noeud precedant dans la chaine
            if (noeud_a_relier)
            {
                commodite_cree=(CellCommodite*)malloc(sizeof(CellCommodite));
                commodite_cree->extrA=noeud_a_relier;
                commodite_cree->extrB=noeud_cree;
                commodite_cree->suiv=reseau->commodites;
                reseau->commodites=commodite_cree;
                noeud_a_relier=noeud_cree;
            }

            point_curr=point_curr->suiv;
        
        }while (point_curr);
        
        Cell_curr=Cell_curr->suiv;

    }

    reseau->nbNoeuds=counter;

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
