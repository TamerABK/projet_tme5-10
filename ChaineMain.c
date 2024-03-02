#include "Chaine.h"
#include "SVGwriter.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


int main(int argc,char** argv){

    if(argc!=3)
    {
        printf("Chaine main prend 2 fichiers en argument\n");
        exit(EXIT_FAILURE);
    }

    
    FILE *fLecture=fopen(argv[1],"r");
    check_pointer(fLecture);
    FILE *fEcriture=fopen(argv[2],"w");
    check_pointer(fEcriture);

    Chaines * Ctest=lectureChaines(fLecture);
    
    ecrireChaines(Ctest,fEcriture);

    afficheChainesSVG(Ctest,"Svgtest");

    printf("%f\n",longueurTotale(Ctest));
    printf("%d\n",comptePointsTotal(Ctest));

    fclose(fLecture);
    fclose(fEcriture);

    liberer_chaine(Ctest);


}