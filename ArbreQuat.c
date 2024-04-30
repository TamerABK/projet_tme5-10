#include <stdio.h>
#include "ArbreQuat.h"
#include "Chaine.h"
#include "SVGwriter.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void chaineCoordMinMax(Chaines* C, double* xmin, double* ymin, double* xmax, double* ymax){
    if(C==NULL){
        printf("Impossible de determiner coordonnees min et max de chaines qui n'existent pas\n");
        return;
    }

    CellChaine *c = C->chaines;
    if(c==NULL){
        printf("Il n'y a pas de chaines, il est impossible de determiner les coordonnees min et max\n");
        return;
    }
    CellPoint *p = c->points;
    if(p==NULL){
        printf("La chaine n'a pas de points, il est impossible de determiner les coordonnees min et max\n");
        return;
    }
    (*xmin) = p->x;
    (*ymin) = p->y;
    (*xmax) = p->x;
    (*ymax) = p->y;
    /*Boucle passant par toutes les chaines (Boucle for est plus sûre)*/
    for(int i=0; i<(C->nbChaines); i++){
        if(c==NULL){
            printf("Il y a moins de chaines que prévu, il est impossible de determiner les coordonnees min et max\n");
            return;
        }
        p = c->points;
        /*Boucle passant par tous les points de la chaine*/
        while(p){
            if(p->x < (*xmin)){ (*xmin) = p->x; }
            if(p->y < (*ymin)){ (*ymin) = p->y; }
            if(p->x > (*xmax)){ (*xmax) = p->x; }
            if(p->y > (*ymax)){ (*ymax) = p->y; }

            p = p->suiv;
        }

        c = c->suiv;
    }
}

ArbreQuat* creerArbreQuat(double xc, double yc, double coteX,double coteY){
	ArbreQuat* ab=(ArbreQuat*)malloc(sizeof(ArbreQuat));
	check_pointer(ab);
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

ArbreQuat** trouve_dir(ArbreQuat* parent,double x, double y){
	
	check_pointer(parent);

	if(x>=parent->xc)
	{
		if (y>=parent->yc)
		{	
			// printf("Nord est\n");
			return &(parent->ne);
		}else{
			// printf("Sud est\n");
			return &(parent->se);
		}
		
	}else{
		if (y>=parent->yc)
		{
			// printf("Nord ouest\n");
			return &(parent->no);
		}else{
			// printf("Sud ouest\n");
			return &(parent->so);
		}
	}
}


void calcul_centre(ArbreQuat* parent,Noeud *n, double *xnew, double *ynew){

	check_pointer(parent);
	check_pointer(n);
	if((parent->xc)<=(n->x))
	{
		*xnew=parent->xc +((parent->coteX*1.0)/4.0);
		
	}else{
		*xnew=parent->xc -((parent->coteX*1.0)/4.0);
	}

	if((parent->yc)<=(n->y))
	{
		*ynew=parent->yc+((parent->coteY*1.0)/4.0);
	}else{
		*ynew=parent->yc-((parent->coteY*1.0)/4.0);
	}

}


void insererNoeudArbre(Noeud* n, ArbreQuat** a,ArbreQuat* parent){
	check_pointer(parent);
	check_pointer(n);
	// Si a est vide

	if ((*a)==NULL)
	{
		// printf("a est Null\n");
		double coteX=(parent->xc*1.0)/2.0;
		double coteY=(parent->yc*1.0)/2.0;
		double x=__INT_MAX__,y=__INT_MAX__;
		calcul_centre(parent,n,&x,&y);
		// printf("%f %f\n",x,y);
		*a=creerArbreQuat(x,y,coteX,coteY);
		(*a)->noeud=n;
		return;
    }

	
	// Si a est une feuille
	if((*a)->noeud!=NULL)
	{
		// printf("a est une feuille\n");
		Noeud* noeud_deplace=(*a)->noeud;
		(*a)->noeud=NULL;

		insererNoeudArbre(noeud_deplace,trouve_dir(*a,noeud_deplace->x,noeud_deplace->y),*a);

		insererNoeudArbre(n,trouve_dir(*a,n->x,n->y),*a);
	
		
		return;
	}

	// Si a est une cellule interne
	if(((*a) !=NULL)&&(*a)->noeud==NULL )
	{	
		
		insererNoeudArbre(n,trouve_dir(*a,n->x,n->y),*a);
		
		return;
	}
	
}

Noeud* ajouteCellReseau(Reseau *R,double x, double y){
		Noeud* noeud_cree=(Noeud*)malloc(sizeof(Noeud));
		noeud_cree->x=x;
		noeud_cree->y=y;
		noeud_cree->num=R->nbNoeuds+1;
		R->nbNoeuds++;
		CellNoeud* cell_cree= (CellNoeud*)malloc(sizeof(CellNoeud));
		cell_cree->nd=noeud_cree;
		cell_cree->suiv=R->noeuds;
		R->noeuds=cell_cree;
		return noeud_cree;
}

Noeud* rechercheCreeNoeudArbre(Reseau* R, ArbreQuat** a, ArbreQuat* parent,double x, double y){
	check_pointer(R);
	check_pointer(parent);
	
	// a est vide
	// printf("rechercheCreeNoeudArbre\n");
	if ((*a)==NULL)
	{	
		// printf("a est Null\n");
		Noeud* noeud_cree= ajouteCellReseau(R,x,y);
		insererNoeudArbre(noeud_cree,a,parent);
		return noeud_cree;
	}
	// a est une feuille
	if((*a)->noeud!=NULL)
	{
		// printf("a est une feuille\n");
		if(((*a)->noeud->x==x) &&((*a)->noeud->y==y))
		{	
			
			return (*a)->noeud;
		}else{
			// Noeud* noeud_cree= ajouteCellReseau(R,x,y);

			Noeud* noeud_cree=ajouteCellReseau(R,x,y);
			insererNoeudArbre(noeud_cree,a,parent);
			return noeud_cree;
		}
	}
	// a est une cellule
	if(((*a) !=NULL)&&(*a)->noeud==NULL)
	{
		ArbreQuat** direction=trouve_dir(*a,x,y);
		rechercheCreeNoeudArbre(R,direction,*a,x,y);
	}


}


void libererArbreQuat(ArbreQuat* a) {
    if (a == NULL) {
        return;
    }
	a->noeud=NULL;
    libererArbreQuat(a->so);
    libererArbreQuat(a->se);
    libererArbreQuat(a->no);
    libererArbreQuat(a->ne);

    free(a);
}

void print_arbre(ArbreQuat* A){

	if(!A) return;
	if (A->noeud){ printf("%d\n",A->noeud->num); return;}

	print_arbre(A->ne);
	print_arbre(A->no);
	print_arbre(A->se);
	print_arbre(A->so);

}

Reseau* reconstitueReseauArbre(Chaines* C) {
    
    Reseau* reseau = (Reseau*)malloc(sizeof(Reseau));
	CellChaine* Cell_curr = C->chaines;
	CellPoint* pt=NULL;
    CellCommodite *commodite_cree=NULL;
    Noeud* nouveau=NULL,*noeud_a_relier=NULL;
	ArbreQuat** fils=NULL; 

	reseau->nbNoeuds=0;
	reseau->gamma=C->gamma;


	double xmin, ymin, xmax, ymax;
    chaineCoordMinMax(C, &xmin, &ymin, &xmax, &ymax);
    double coteX = xmax - xmin;
    double coteY = ymax - ymin;
    double xc = xmax - (coteX*1.0)/2.0;
    double yc = ymax - (coteY*1.0)/2.0;
    
	ArbreQuat* arbre = creerArbreQuat(xc, yc, coteX,coteY); 
	while (Cell_curr)
    {
        pt = Cell_curr->points;

        commodite_cree=(CellCommodite*)malloc(sizeof(CellCommodite));

        while(pt != NULL){
			fils=trouve_dir(arbre,pt->x,pt->y);
            nouveau = rechercheCreeNoeudArbre(reseau,fils,arbre,pt->x,pt->y);
            if (!commodite_cree->extrA) commodite_cree->extrA=nouveau;
            if (noeud_a_relier){
                ajoutevoisin(nouveau,noeud_a_relier);
                ajoutevoisin(noeud_a_relier,nouveau);
            } 
            noeud_a_relier=nouveau;
            pt = pt->suiv;
        }
		// printf("--------------------\n");
		// print_arbre(arbre);
        commodite_cree->extrB=noeud_a_relier;
        commodite_cree->suiv=reseau->commodites;
        reseau->commodites=commodite_cree;
        noeud_a_relier=NULL;

        Cell_curr=Cell_curr->suiv;

    }

	
    libererArbreQuat(arbre);
	// printf("%d\n", reseau->nbNoeuds);
    return reseau;
}