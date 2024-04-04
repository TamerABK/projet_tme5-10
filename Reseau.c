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

    printf("Ajv1-%p\n",v1->voisins);
   CellNoeud* cell_curr=v1->voisins;
   
    //Verifie si les deux noeuds sont deja voisins    
   while (cell_curr)
   {
        printf("Ajv2-%p\n",cell_curr->nd);
        
        if(cell_curr->nd->num==v2->num){ printf("On sort\n"); return ;}
        cell_curr=cell_curr->suiv;
   }
    // Ajoute le voisin s'il n'existe pas deja
    
    CellNoeud* cell_cree=(CellNoeud*)malloc(sizeof(CellNoeud));
    check_pointer(cell_cree);
    printf("Ajv3\n");
    cell_cree->nd=v2;
    printf("Ajv4\n");
    cell_cree->suiv=v1->voisins;
    printf("Ajv5\n");
    v1->voisins=cell_cree;
    printf("Ajv6\n");

}


Noeud* rechercheCreeNoeudListe(Reseau *R,double x, double y,int* counter)
{
    check_pointer(R);

    CellNoeud* Cn=R->noeuds;
    Noeud* N;

    while (Cn)
    {
        N=Cn->nd;
        if(N->x==x && N->y==y)
        {
            return N;
        }
        Cn=Cn->suiv;
    }

    if(!Cn)
    {
        N=(Noeud*)malloc(sizeof(Noeud));
        N->x=x;N->y=y;N->num=R->nbNoeuds+1;
        R->nbNoeuds++;
        Cn=(CellNoeud*)malloc(sizeof(CellNoeud));
        Cn->nd=N;Cn->suiv=R->noeuds;
        R->noeuds=Cn;
        if(counter) (*counter)++;
        return N;
    }

    
    
}

Reseau* reconstitueReseauListe(Chaines *C){

    check_pointer(C);

    //Cree et intialise gamma du reseau a rendre    

    Reseau *reseau=(Reseau*)malloc(sizeof(Reseau));
    reseau->gamma=C->gamma;
    // Variable pour parcourir la chaine,compter les noeuds et cree les noeuds et commodite
    CellChaine *Cell_curr=C->chaines;
    CellPoint* point_curr;
    int counter=0;
    Noeud* noeud_cree,* noeud_a_relier=NULL;
    CellNoeud* cellnoeud_cree;
    CellCommodite* commodite_cree;

    while (Cell_curr)
    {
        point_curr=Cell_curr->points;

        CellCommodite *commodite_cree=(CellCommodite*)malloc(sizeof(CellCommodite));

        while (point_curr)
        {
            noeud_cree= rechercheCreeNoeudListe(reseau,point_curr->x,point_curr->y,&counter);
            
            if (!commodite_cree->extrA) commodite_cree->extrA=noeud_cree;

            
            if (noeud_a_relier){
                ajoutevoisin(noeud_cree,noeud_a_relier);
                ajoutevoisin(noeud_a_relier,noeud_cree);
            } 
                
            
            noeud_a_relier=noeud_cree;
            point_curr=point_curr->suiv;

        }
        // Cree une commodite
        commodite_cree->extrB=noeud_a_relier;
        commodite_cree->suiv=reseau->commodites;
        reseau->commodites=commodite_cree;

        Cell_curr=Cell_curr->suiv;
        noeud_a_relier=NULL;

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

    return counter/*(int)(ceil((float)counter/2))*/;
    
}


// FONCTION D'ECRITURE

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
        printf("%p\n",cell_noeud_curr);
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


void liberer_reseau(Reseau *R){
    check_pointer(R);

    CellCommodite* commod_suiv,* commod_curr= R->commodites;

    while (commod_curr)
    {
        commod_suiv=commod_curr->suiv;
        free(commod_curr);
        commod_curr=commod_suiv;
    }

    CellNoeud* cell_suiv,* cell_curr=R->noeuds;

    while (cell_curr)
    {
        cell_suiv=cell_curr->suiv;
        free(cell_curr->nd);
        free(cell_curr);
        cell_curr=cell_suiv;
    }

    free(R);
    
}