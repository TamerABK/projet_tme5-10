#include "Reseau.h"
#include "Chaine.h"
#include "Hachage.h"
#include"ArbreQuat.h"
#include "SVGwriter.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


int main(int argc,char** argv){

    if (argc!=3)
    {
        printf("Mauvais nombre d'argument\n");
        exit(EXIT_FAILURE);
    }

    FILE* fLecture=fopen(argv[1],"r");
    check_pointer(fLecture);
    FILE* fEcriture=fopen("test2.res","w");
    check_pointer(fEcriture);

    int methode_choisie=atoi(argv[2]);

    Chaines* chaine=lectureChaines(fLecture);

    if(methode_choisie==1)
    {
        Reseau* reseau_listeCh=reconstitueReseauListe(chaine);
        afficheReseauSVG(reseau_listeCh,"testreseauLCH");
        ecrireReseau(reseau_listeCh,fEcriture);
        liberer_reseau(reseau_listeCh);
    }
    
    if(methode_choisie==2)
    {
        Reseau* reseau_tableH=reconstitueReseauHachage(chaine,50);
        afficheReseauSVG(reseau_tableH,"testreseauH");
        ecrireReseau(reseau_tableH,fEcriture);
        liberer_reseau(reseau_tableH);
    }

    if(methode_choisie==3)
    {
        Reseau* reseau_arbreQ=reconstitueReseauArbre(chaine);
        // afficheReseauSVG(reseau_arbreQ,"testreseauAQ");
        // ecrireReseau(reseau_arbreQ,fEcriture);
        // liberer_reseau(reseau_arbreQ);
    }

    liberer_chaine(chaine);
    fclose(fLecture);
    fclose(fEcriture);

    return 0;
}