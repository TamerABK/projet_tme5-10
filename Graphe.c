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

    graphe_cree->T_som=(Sommet**)malloc(sizeof(Sommet*)*(graphe_cree->nbsom+1));
    graphe_cree->T_commod=(Commod*)malloc(sizeof(Commod)*graphe_cree->nbcommod);

    for (int i=0; i<graphe_cree->nbsom+1;i++)
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
            if (graphe_cree->T_som[noeud_voisin->num]!=NULL)
            {
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

// Lit les arete et determine le voisin 
int determine_voisin(Cellule_arete* C,int num){
    int u=C->a->u;
    int v=C->a->v;
    if (u==num){ 
        return v;
    }
    return u;
}

int* parcour_en_largeur(Graphe* G,int debut){
    
    // Initialisation du tableau distance et file
    
    if(debut==0) return NULL;

    int* distance=(int*)malloc(sizeof(int)*G->nbsom);
    for(int i=0;i<G->nbsom+1;i++)
    {
        distance[i]=__INT_MAX__;
    
    }

    // Initialisation: Distance debut a lui meme
    distance[debut]=0;
    
    S_file* File=malloc(sizeof(S_file));
    enfile(File,debut);
    Sommet* somm_cur=NULL;
    Cellule_arete* cell_cur=NULL;
    int num,voisin;
    

    while (!estFileVide(File))
    {
        num=defile(File);
        somm_cur=G->T_som[num];
        cell_cur=somm_cur->L_voisin;
        while (cell_cur)
        {   
            
            voisin=determine_voisin(cell_cur,num);


            if (distance[voisin]>distance[num]+1)
            {
                enfile(File,voisin);
                distance[voisin]=distance[num]+1;
            }

            cell_cur=cell_cur->suiv;
        }
    }
    
    free(File);
    return distance;
}

S_file** PP_chemins(Graphe* G,int debut){

    if(debut==0) return NULL;

    S_file** Chemins=(S_file**)malloc(sizeof(S_file*)*G->nbsom+1);
    for (int i=0;i<G->nbsom+1;i++)
    {
        Chemins[i]=(S_file*)malloc(sizeof(S_file));
        Init_file(Chemins[i]);
    }

    // Initialisation: Chemin du debut a lui meme
    enfile(Chemins[debut],debut);

    S_file* File=malloc(sizeof(S_file));
    enfile(File,debut);
    Sommet* somm_cur=NULL;
    Cellule_arete* cell_cur=NULL;
    int num,voisin;

    while (!estFileVide(File))
    {
        num=defile(File);
        somm_cur=G->T_som[num];
        cell_cur=somm_cur->L_voisin; 

         while (cell_cur)
        {   
            
            voisin=determine_voisin(cell_cur,num);


            if (estFileVide(Chemins[voisin]))
            {
                enfile(File,voisin);
                // La file de pere + le fils en fin
                clone_file(Chemins[num],Chemins[voisin]);
                enfile(Chemins[voisin],voisin);
            }

            cell_cur=cell_cur->suiv;
        }

    }
    
    free(File);
    return Chemins;


}


void affiche_graphe(Graphe* G){

    printf("Num de sommes: %d\n",G->nbsom);
    printf("Num de commodites: %d\n ",G->nbcommod);
    Cellule_arete* cell_cur=NULL;
    for (int i=1;i<G->nbsom+1;i++)
    {
        printf("Sommet %i: ",i);
        cell_cur=G->T_som[i]->L_voisin;
        while (cell_cur)
        {
            printf("(%d,%d) ",cell_cur->a->u,cell_cur->a->v);
            cell_cur=cell_cur->suiv;
        }
        printf("\n");

    }
}