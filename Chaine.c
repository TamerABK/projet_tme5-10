#include "Chaine.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// FONCTION TOKEN CHECK

void check_token(char *token ){
     if(!token)
       {
            printf("Format ligne invalide");
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

    fgets(buffer,256,f);
    assert(strncmp(buffer,"NbChain:",8)==0);
    sscanf(buffer,"NbChain: %d",chaine->nbChaines);

    fgets(buffer,256,f);
    assert(strncmp(buffer,"Gamma:",6)==0);
    sscanf(buffer,"Gamma: %d",chaine->gamma);

    int i,n;
    char *token;
    CellChaine* cellChaine;
    CellPoint* point;

    while (buffer[0]!='\0')
    {
       fgets(buffer,256,f); 
       char *token=strtok(buffer," ");
       check_token(token);
       i=0;

       //CREE UNE CellChaine, L'INSERT, PUIS INITIALISE SES ATRIBUTS 
        cellChaine=(CellChaine*)malloc(sizeof(CellChaine));
        check_pointer(cellChaine);

        cellChaine->suiv=chaine->chaines;
        chaine->chaines=cellChaine;

        cellChaine->numero=atoi(token);

        token=strtok(NULL," ");
        check_token(token);
        n=atoi(token);
        token=strtok(NULL," ");
        check_token(token);

        while (!token && i<n)
        {
            // CREE LES POINTS ET LES INSERTS
            point=(CellPoint*)malloc(sizeof(CellPoint));
            check_pointer(point);

            point->suiv=cellChaine->points;
            cellChaine->points=point;

            point->x=atof(token);
            token=strtok(NULL," ");
            check_token(token);
            point->y=atof(token);
            token=strtok(NULL," ");

            i++;
        }
        
    }
    
    return chaine;
    
}

void ecrireChaine(Chaines *C,FILE *f){

    check_pointer(C);

    fprintf(f,"NbChain: %d\n",C->nbChaines);
    fprintf(f,"Gamma: %d\n",C->gamma);

    CellChaine *Ch=C->chaines;
    

    while (!Ch)
    {
        
    }
    


}