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

ArbreQuat* trouve_dir(ArbreQuat* parent,int x, int y){
	
	check_pointer(parent);

	if(parent->xc<=x)
	{
		if (parent->yc<=y)
		{
			return (parent->ne);
		}else{
			return (parent->se);
		}
		
	}else{
		if (parent->yc<=y)
		{
			return (parent->no);
		}else{
			return (parent->so);
		}
	}
}


void calcul_centre(ArbreQuat* parent,Noeud *n, double *xnew, double *ynew){

	check_pointer(parent);
	check_pointer(n);
	if(parent->xc<=n->x)
	{
		*xnew=parent->xc +(parent->coteX/4);
		
	}else{
		*xnew=parent->xc -(parent->coteX/4);
	}

	if(parent->yc<=n->y)
	{
		*ynew=parent->yc+(parent->coteY/4);
	}else{
		*ynew=parent->yc-(parent->coteY/4);
	}

}


void insererNoeudArbre(Noeud* n, ArbreQuat** a,ArbreQuat* parent){

	check_pointer(parent);
	check_pointer(n);
	// Si a est vide
	if (!(*a))
	{
		int coteX=parent->xc/2;
		int coteY=parent->yc/2;
		double x,y;
		calcul_centre(parent,n,&x,&y);
		*a=creerArbreQuat(x,y,coteX,coteY);
		(*a)->noeud=n;
		return;
    }

	
	// Si a est une feuille
	if((*a)->noeud!=NULL)
	{
		Noeud* noeud_deplace=(*a)->noeud;
		(*a)->noeud=NULL;
		ArbreQuat* direction=trouve_dir(*a,noeud_deplace->x,noeud_deplace->y);
		insererNoeudArbre(noeud_deplace,&direction,*a);

		direction=trouve_dir(*a,n->x,n->y);
		insererNoeudArbre(n,&direction,*a);
		
		
		return;
	}
	// Si a est une cellule interne
	if((*a !=NULL)&&(*a)->noeud==NULL )
	{	
		
		ArbreQuat* direction=trouve_dir(*a,n->x,n->y);
		insererNoeudArbre(n,&direction,*a);
		
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
	// a est vide
	if (!(*a))
	{	
		Noeud* noeud_cree= ajouteCellReseau(R,x,y);
		insererNoeudArbre(noeud_cree,a,parent);
		return noeud_cree;
	}
	// a est une feuille
	if((*a)->noeud!=NULL)
	{
		if((*a)->noeud->x==x &&(*a)->noeud->y==y)
		{	
			return (*a)->noeud;
		}else{
			Noeud* noeud_cree= ajouteCellReseau(R,x,y);
			insererNoeudArbre(noeud_cree,a,parent);
			return noeud_cree;
		}
	}
	// a est une cellule
	if((*a !=NULL)&&(*a)->noeud==NULL)
	{
		ArbreQuat* direction=trouve_dir(*a,x,y);
		rechercheCreeNoeudArbre(R,&direction,*a,x,y);
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
    
    Reseau* reseau = (Reseau*)malloc(sizeof(Reseau));
	CellChaine* Cell_curr = C->chaines;
	CellPoint* pt=NULL;
    CellCommodite *commodite_cree=NULL;
    Noeud* nouveau=NULL,*noeud_a_relier=NULL;
	ArbreQuat* fils=NULL; 

	reseau->nbNoeuds=0;
	reseau->gamma=C->gamma;


	double xmin, ymin, xmax, ymax;
    chaineCoordMinMax(C, &xmin, &ymin, &xmax, &ymax);
    double coteX = xmax - xmin;
    double coteY = ymax - ymin;
    double xc = xmax - coteX/2;
    double yc = ymax - coteY/2;
    
	ArbreQuat* arbre = creerArbreQuat(xc, yc, coteX,coteY); 
	while (Cell_curr)
    {
        pt = Cell_curr->points;

        commodite_cree=(CellCommodite*)malloc(sizeof(CellCommodite));

        while(pt != NULL){
			fils=trouve_dir(arbre,pt->x,pt->y);
            nouveau = rechercheCreeNoeudArbre(reseau,&fils,arbre,pt->x,pt->y);
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










