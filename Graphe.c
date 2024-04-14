#include "Graphe.h"

Sommet* creerSommet(Noeud* N){

    Sommet* sommet_cree=(Sommet*)malloc(sizeof(Sommet));
    check_pointer(sommet_cree);
    sommet_cree->num=N->num;
    sommet_cree->x=N->x;
    sommet_cree->y=N->y;
    sommet_cree->L_voisin=NULL;
    return sommet_cree;
}

void ajout_arrete(Sommet* s1,Sommet* s2){

    check_pointer(s1);
    check_pointer(s2);
    // Arrete est cree
    Arete* arete_cree=(Arete*)malloc(sizeof(Arete));
    check_pointer(arete_cree);
    arete_cree->u=s1->num;
    arete_cree->v=s2->num;
    // Puis ajouter a  la premier sommet
    Cellule_arete* cell_arete=(Cellule_arete*)malloc(sizeof(Cellule_arete));
    cell_arete->a=arete_cree;
    cell_arete->suiv=s1->L_voisin;
    s1->L_voisin=cell_arete;
    // Puis la seconde
    cell_arete=(Cellule_arete*)malloc(sizeof(Cellule_arete));
    cell_arete->a=arete_cree;
    cell_arete->suiv=s2->L_voisin;
    s2->L_voisin=cell_arete;

}

Commod creerCommod(CellCommodite* C){

    Commod* commod_cree=malloc(sizeof(Commod));
    check_pointer(commod_cree);
    commod_cree->e1=C->extrA->num;
    commod_cree->e2=C->extrB->num;
    return *commod_cree;

} 

Graphe* creerGraphe(Reseau *R){
    check_pointer(R);

    // Cree et intialise les attribut du graphe
    Graphe* graphe_cree= (Graphe*)malloc(sizeof(Graphe));

    check_pointer(graphe_cree);

    graphe_cree->gamma=R->gamma;
    graphe_cree->nbsom=R->nbNoeuds;
    graphe_cree->nbcommod=nbCommodites(R);

    graphe_cree->T_som=(Sommet**)malloc(sizeof(Sommet*)*graphe_cree->nbsom);
    graphe_cree->T_commod=(Commod*)malloc(sizeof(Commod)*graphe_cree->nbcommod);

    for (int i=0; i<graphe_cree->nbsom;i++)
    {
        graphe_cree->T_som[i]=NULL;
    }
    
    //On commence a construire le graphe
    CellNoeud* cell_curr=R->noeuds,*cell_voisin=NULL;
    Noeud* noeud_curr=NULL,*noeud_voisin=NULL;
    CellCommodite* commod_curr=R->commodites;
    Sommet* sommet_cree=NULL,*sommet_voisin=NULL;
    Cellule_arete* cell_arete=NULL;

    // On ajoute les sommets
    while (cell_curr)
    {
        noeud_curr=cell_curr->nd;

        if (graphe_cree->T_som[noeud_curr->num]==NULL)
        {
            graphe_cree->T_som[noeud_curr->num]=creerSommet(noeud_curr);
        }
            
        sommet_cree=graphe_cree->T_som[noeud_curr->num];
        cell_voisin=noeud_curr->voisins;

        while (cell_voisin)
        {
            noeud_voisin=cell_voisin->nd;

            // Si cette sommet de noeud voisin existe alors l'arrete entre elle et sommet_cree existe deja
            if (graphe_cree->T_som[noeud_voisin->num]==NULL)
            {
                graphe_cree->T_som[noeud_voisin->num]=creerSommet(noeud_voisin);
                sommet_voisin=graphe_cree->T_som[noeud_voisin->num];
                ajout_arrete(sommet_cree,sommet_voisin);
            }
            cell_voisin=cell_voisin->suiv;
        }
            

        cell_curr=cell_curr->suiv;
        
        

    }
    
    // On ajoute les commodites
    for (int i=0;i<graphe_cree->nbcommod;i++)
    {
        graphe_cree->T_commod[i]=creerCommod(commod_curr);
        commod_curr=commod_curr->suiv;
    }
    
    return graphe_cree;
}