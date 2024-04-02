#include <stdio.h>
#include "ArbreQuat.h"
#include "Chaine.h"
#include "SVGwriter.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void chaineCoordMinMax(Chaines* C, double* xmin, double*
ymin, double* xmax, double* ymax){
	*xmin=C->chaines[0].points->x;
	*xmax=C->chaines[0].points->x;
	*ymin=C->chaines[0].points->y;
	*ymax=C->chaines[0].points->y;
	
	for(int i=0; i< C->nbChaines ; i++){
		CellChaine* chaine= &(C->chaines[i]);
		CellPoint* p=chaine->points;
		while(p!=NULL){
			if (p->x < *xmin) *xmin=p->x;
			if (p->x > *xmax) *xmax=p->x;
			if (p->y < *ymin) *ymin=p->y;
			if (p->y > *ymax) *ymax=p->y;
			
			p=p->suiv;
		}
	}
}

ArbreQuat* creerArbreQuat(double xc, double yc, double coteX,double coteY){
	ArbreQuat* ab=(ArbreQuat*)malloc(sizeof(ArbreQuat));
	
	ab->xc=xc;
	ab->yc=yc;
	ab->coteX=coteX;
	ab->coteY=coteY;
	ab->noeud=NULL;
	ab->so=NULL;
	ab->se=NULL;
	ab->no=NULL;
	ab->ne=NULL;
	
	return ab;
}

ArbreQuat** trouve_dir(ArbreQuat* parent,int x, int y){
	
	check_pointer(parent);

	if(parent->xc<=x)
	{
		if (parent->yc<=y)
		{
			return &(parent->ne);
		}else{
			return &(parent->se);
		}
		
	}else{
		if (parent->yc<=y)
		{
			return &(parent->no);
		}else{
			return &(parent->so);
		}
	}
}

void insererNoeudArbre(Noeud* n, ArbreQuat** a,ArbreQuat* parent){

	check_pointer(parent);
	check_pointer(n);
	// Si a est vide
	if (!(*a))
	{
		int coteX= abs(parent->xc-n->x);
		int coteY=abs(parent->yc-n->y);
		*a=creerArbreQuat(n->x,n->y,coteX,coteY);
		(*a)->noeud=n;
		printf("J'ai finit l'insertion cas 1\n");
		return;

	}
	// Si a est une feuille
	if((*a)->noeud!=NULL)
	{
		Noeud* noeud_deplace=(*a)->noeud;
		(*a)->noeud=NULL;

		ArbreQuat** direction=trouve_dir(*a,noeud_deplace->x,noeud_deplace->y);
		insererNoeudArbre(noeud_deplace,direction,*a);

		direction=trouve_dir(*a,n->x,n->y);
		insererNoeudArbre(n,direction,*a);
		
		printf("J'ai finit l'insertion cas 2\n");
		return;
	}
	// Si a est une cellule interne
	if((*a !=NULL)&&(*a)->noeud==NULL )
	{
		ArbreQuat** direction=trouve_dir(*a,n->x,n->y);
		insererNoeudArbre(n,direction,*a);
		printf("J'ai finit l'insertion cas 3\n");
		return;
	}
	
}

Noeud* rechercheCreeNoeudArbre(Reseau* R, ArbreQuat** a, ArbreQuat* parent,double x, double y){

	if (!(*a))
	{	
		printf("a est null\n");
		Noeud* noeud_cree=(Noeud*)malloc(sizeof(Noeud));
		noeud_cree->x=x;
		noeud_cree->y=y;
		noeud_cree->num=R->nbNoeuds+1;
		R->nbNoeuds++;
		CellNoeud* cell_cree= (CellNoeud*)malloc(sizeof(CellNoeud));
		cell_cree->nd=noeud_cree;
		cell_cree->suiv=R->noeuds;
		R->noeuds=cell_cree;
		insererNoeudArbre(noeud_cree,a,parent);
		return noeud_cree;
	}
	// a est une feuille
	if((*a)->noeud!=NULL)
	{
		printf("a est une feuille\n");
		if((*a)->noeud->x==x &&(*a)->noeud->y==y)
		{
			return (*a)->noeud;
		}else{
			Noeud* noeud_cree=(Noeud*)malloc(sizeof(Noeud));
			noeud_cree->x=x;
			noeud_cree->y=y;
			noeud_cree->num=R->nbNoeuds+1;
			insererNoeudArbre(noeud_cree,a,parent);
			return noeud_cree;
		}
	}
	// a est une cellule
	if((*a !=NULL)&&(*a)->noeud==NULL)
	{
		printf("a est une cellule\n");
		ArbreQuat** direction=trouve_dir(*a,x,y);
		rechercheCreeNoeudArbre(R,direction,*a,x,y);
	}


}

void creerListeCommodites(Chaines* C, Reseau* R) {
    CellCommodite* commodites = NULL;
    
    for (int i = 0; i < C->nbChaines; i++) {
        CellChaine* chaine = C->chaines;
        while (chaine != NULL) {
            CellPoint* p1 = chaine->points;
            CellPoint* p2 = chaine->points->suiv;
            
            Noeud* extrA = rechercheCreeNoeudListe(R, p1->x, p1->y,NULL);
            Noeud* extrB = rechercheCreeNoeudListe(R, p2->x, p2->y,NULL);
            
            CellCommodite* nouvelle_commodite = (CellCommodite*)malloc(sizeof(CellCommodite));
            nouvelle_commodite->extrA = extrA;
            nouvelle_commodite->extrB = extrB;
            nouvelle_commodite->suiv = commodites;
            
            commodites = nouvelle_commodite;
            
            chaine = chaine->suiv;
        }
    }
    
    R->commodites = commodites;
}


void libererArbreQuat(ArbreQuat* a) {
    if (a == NULL) {
        return;
    }

    libererArbreQuat(a->so);
    libererArbreQuat(a->se);
    libererArbreQuat(a->no);
    libererArbreQuat(a->ne);

    free(a);
}

Reseau* reconstitueReseauArbre(Chaines* C) {
    
	ArbreQuat* arbre = creerArbreQuat(0, 0, 1000, 1000); 
    Reseau* reseau = (Reseau*)malloc(sizeof(Reseau));
	CellChaine* Cell_curr = C->chaines;
	CellPoint* pt;
    CellCommodite *commodite_cree;
    Noeud* nouveau,*noeud_a_relier; 
    
	while (Cell_curr)
    {
        printf("1_%p\n",Cell_curr);
        pt = Cell_curr->points;

        commodite_cree=(CellCommodite*)malloc(sizeof(CellCommodite));

        while(pt != NULL){
			printf("2_%p\n",Cell_curr);
            Noeud* nouveau = rechercheCreeNoeudArbre(reseau,&arbre,arbre,pt->x,pt->y);
            printf("3\n");
            if (!commodite_cree->extrA) commodite_cree->extrA=nouveau;

            if (noeud_a_relier){
                ajoutevoisin(nouveau,noeud_a_relier);
                ajoutevoisin(noeud_a_relier,nouveau);
            } 

            noeud_a_relier=nouveau;
            pt = pt->suiv;
        }

        commodite_cree->extrB=noeud_a_relier;
        commodite_cree->suiv=reseau->commodites;
        reseau->commodites=commodite_cree;
        noeud_a_relier=NULL;

        Cell_curr=Cell_curr->suiv;

    }

	
    libererArbreQuat(arbre);

    return reseau;
}










