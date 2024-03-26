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
			
		return;
	}
	// Si a est une cellule interne
	if((*a)->noeud==NULL)
	{
		ArbreQuat** direction=trouve_dir(*a,n->x,n->y);
		insererNoeudArbre(n,direction,*a);
		return;
	}
	
}

Noeud* rechercheCreeNoeudArbre(Reseau* R, ArbreQuat** a, ArbreQuat* parent,double x, double y){

	if (!(*a))
	{
		Noeud* noeud_cree=(Noeud*)malloc(sizeof(Noeud));
		noeud_cree->x=x;
		noeud_cree->y=y;
		noeud_cree->num=R->nbNoeuds+1;
		insererNoeudArbre(noeud_cree,*a,parent);
		return noeud_cree;
	}
	// a est une feuille
	if((*a)->noeud!=NULL)
	{
		if((*a)->noeud->x==x &&(*a)->noeud->y==y)
		{
			return (*a)->noeud;
		}else{
			Noeud* noeud_cree=(Noeud*)malloc(sizeof(Noeud));
			noeud_cree->x=x;
			noeud_cree->y=y;
			noeud_cree->num=R->nbNoeuds+1;
			insererNoeudArbre(noeud_cree,*a,parent);
			return noeud_cree;
		}
	}
	// a est une cellue
	if((*a)->noeud==NULL)
	{
		ArbreQuat** direction=trouve_dir(*a,x,y);
		rechercheCreeNoeudArbre(R,direction,*a,x,y);
	}


}






