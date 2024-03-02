#include "Chaine.h"
#include "SVGwriter.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


// FONCTION TOKEN CHECK

void check_token(char *token ){
     if(!token)
       {
            printf("Format ligne invalide\n");
            exit(EXIT_FAILURE);
       }
}

void check_pointer(void *pointer){
    if(!pointer)
    {
        printf("Erreur d'allocation de memoire\n");
        exit(EXIT_FAILURE);
    }
}

// FONCTION LECTURE

Chaines* lectureChaines(FILE *f){

    if(!f)
    {
        printf("Fichier Invalide\n");
        exit(EXIT_FAILURE);
    }
    
    Chaines* chaine= (Chaines*) malloc(sizeof(Chaines));
    
    check_pointer(chaine);

    char buffer[256];

    // LIT LES DEUX PREMIERES LIGNES ET INITIALISE "chaine"
    int temp;

    fgets(buffer,256,f);
    assert(strncmp(buffer,"NbChain:",8)==0);
    sscanf(buffer,"NbChain: %d",&temp);
    chaine->nbChaines=temp;

    fgets(buffer,256,f);
    assert(strncmp(buffer,"Gamma:",6)==0);
    sscanf(buffer,"Gamma: %d",&temp);
    chaine->gamma=temp;

    int i,n;
    char *token;
    CellChaine* cellChaine;
    CellPoint* point;

    while (fgets(buffer,256,f))
    {
        
       char *token=strtok(buffer," ");
       printf("1-%s\n",token);
       check_token(token);
       i=0;

       //CREE UNE CellChaine, L'INSERT, PUIS INITIALISE SES ATRIBUTS 
        cellChaine=(CellChaine*)malloc(sizeof(CellChaine));
        check_pointer(cellChaine);

        cellChaine->suiv=chaine->chaines;
        chaine->chaines=cellChaine;

        cellChaine->numero=atoi(token);

        token=strtok(NULL," ");
        printf("2-%s\n",token);
        check_token(token);
        n=atoi(token);
        token=strtok(NULL," ");
        printf("3-%s\n",token);
        check_token(token);

        while (i<n)
        {
            // CREE LES POINTS ET LES INSERTS
            point=(CellPoint*)malloc(sizeof(CellPoint));
            check_pointer(point);

            point->suiv=cellChaine->points;
            cellChaine->points=point;

            point->x=atof(token);
            token=strtok(NULL," ");
            printf("4-%s\n",token);
            check_token(token);
            point->y=atof(token);
            token=strtok(NULL," ");

            i++;
        }
        
    }
    
    return chaine;
    
}

void ecrireChaines(Chaines *C,FILE *f){

    check_pointer(C);

    fprintf(f,"NbChain: %d\n",C->nbChaines);
    fprintf(f,"Gamma: %d\n",C->gamma);

    CellChaine *Ch=C->chaines;
    CellPoint* p;
    double buffer[256];
    char cat[256],ftos[10];
    int i,counter,j;

    printf("%p\n",Ch);

    while (Ch)
    {
        printf("1\n");
        p=Ch->points;
        i=0;
        counter=0;

        while (p)
        {
           buffer[i]=p->x;
           i++; 
           buffer[i]=p->y;
           i++;
           counter++;
            p=p->suiv;
        }
        
        fprintf(f,"%d %d ",Ch->numero,counter);

        for(j=0;j<i-1;j++)
        {
            fprintf(f,"%.2f ",buffer[j]);
        }

        fprintf(f,"%.2f\n",buffer[i-1]);


        Ch=Ch->suiv;

    }

    
}

void afficheChainesSVG(Chaines *C, char* nomInstance){
    int i;
    double maxx=0,maxy=0,minx=1e6,miny=1e6;
    CellChaine *ccour;
    CellPoint *pcour;
    double precx,precy;
    SVGwriter svg;
    ccour=C->chaines;
    while (ccour!=NULL){
        pcour=ccour->points;
        while (pcour!=NULL){
            if (maxx<pcour->x) maxx=pcour->x;
            if (maxy<pcour->y) maxy=pcour->y;
            if (minx>pcour->x) minx=pcour->x;
            if (miny>pcour->y) miny=pcour->y;  
            pcour=pcour->suiv;
        }
    ccour=ccour->suiv;
    }
    SVGinit(&svg,nomInstance,500,500);
    ccour=C->chaines;
    while (ccour!=NULL){
        pcour=ccour->points;
        SVGlineRandColor(&svg);
        SVGpoint(&svg,500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny)); 
        precx=pcour->x;
        precy=pcour->y;  
        pcour=pcour->suiv;
        while (pcour!=NULL){
            SVGline(&svg,500*(precx-minx)/(maxx-minx),500*(precy-miny)/(maxy-miny),500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny));
            SVGpoint(&svg,500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny));
            precx=pcour->x;
            precy=pcour->y;    
            pcour=pcour->suiv;
        }
        ccour=ccour->suiv;
    }
    SVGfinalize(&svg);
}


double longueurChaine(CellChaine *c) {
    if (c == NULL || c->points == NULL) {
        return 0.0; 
    }
    double longueur = 0.0;
    CellPoint *cur = c->points;
    CellPoint *next = cur->suiv;
    while (next != NULL) {
        double distance = sqrt(pow(next->x - cur->x, 2) + pow(next->y - cur->y, 2));
        longueur += distance;
        cur = next;
        next = cur->suiv;
    }
    return longueur;
}

double longueurTotale(Chaines *C) {
    if (C == NULL || C->chaines == NULL) {
        return 0.0;
    }
    double longueurTotale = 0.0;
    CellChaine *curChaine = C->chaines;
    while (curChaine != NULL) {
        longueurTotale += longueurChaine(curChaine); 
        curChaine = curChaine->suiv;
    }

    return longueurTotale;
}


int comptePointsTotal(Chaines *C){
	int total=0;
	CellChaine* curChaine = C->chaines;
	while(curChaine !=NULL){
		CellPoint* curPoint = C->chaines->points;
		while(curPoint != NULL){
			total+=1;
			curPoint=curPoint->suiv;
		}
		curChaine=curChaine->suiv;
	}
	return total;
}


// FONCTIONS DE LIBERATION

void liberer_chaine(Chaines* C)
{
    check_pointer(C);

    CellChaine* Chsuiv,*Ch=C->chaines;
    CellPoint* p,*psuiv;

    while (!Ch)
    {
        Chsuiv=Ch->suiv;
        p=Ch->points;

        while (!p)
        {
            psuiv=p->suiv;
            free(p);
            p=psuiv;
        }
        
        free(Ch);
        Ch=Chsuiv;
    }
    
    free(C);

}
	
