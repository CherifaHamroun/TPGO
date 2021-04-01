/*********************************************************************/
/*********************************************************************/
/***							      	   						******/
/***         	TP MinMax / Module TPGO / ESI 2016-2017            ***/
/***							      	   						******/
/*** Il est demandé d'améliorer la fonction estim(...) ci-dessous. ***/
/*** Son rôle est d'évaluer la qualité d'une configuration donnée. ***/
/*** Elle doit retourner une valeur dans ]-100 , +100[		 	*  ***/
/***							      	  						** ***/
/*********************************************************************/
/*********************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <limits.h>  // pour INT_MAX


#define MAX +1	// Niveau Maximisant
#define MIN -1  // Niveau Minimisant
#define INFINI INT_MAX


// Type d'une configuration formant l'espace de recherche
struct config {
	char mat[8][8];			// l'échiquier
	int val;			// estimation de la config
	char xrN, yrN, xrB, yrB;	// positions des rois Noir et Blanc
	char roqueN, roqueB;		// indicateurs de roque pour N et B :
						// 'g'rand roque non réalisable
						// 'p'etit roque non réalisable
						// 'n'on réalisable des 2 cotés
						// 'r'éalisable (valeur initiale)
						// 'e'ffectué
};

// vecteurs des déplacements par type de piece ...
// cavalier
int dC[8][2] = { {-2,+1} , {-1,+2} , {+1,+2} , {+2,+1} , {+2,-1} , {+1,-2} , {-1,-2} , {-2,-1} };
// fou (indices impairs), tour (indices pairs), reine et roi (indices pairs et impairs) ...
int D[8][2] = { {+1,0} , {+1,+1} , {0,+1} , {-1,+1} , {-1,0} , {-1,-1} , {0,-1} , {+1,-1} };

//BLANC
// MATRICE POIDS PION
int pionB_poids[8][8] = {{0,  0,  0,  0,  0,  0,  0,  0},
                        {50, 50, 50, 50, 50, 50, 50, 50},
                        {10, 10, 20, 30, 30, 20, 10, 10},
                        {5,  5, 10, 25, 25, 10,  5,  5},
                        {0,  0,  0, 20, 20,  0,  0,  0},
                        {5, -5,-10,  0,  0,-10, -5,  5},
                        {5, 10, 10,-20,-20, 10, 10,  5},
                        {0,  0,  0,  0,  0,  0,  0,  0}};

// MATRICE POIDS CHEVALIER
int chevalierB_poids[8][8] = {{-50,-40,-30,-30,-30,-30,-40,-50},
                            {-40,-20,  0,  0,  0,  0,-20,-40},
                            {-30,  0, 10, 15, 15, 10,  0,-30},
                            {-30,  5, 15, 20, 20, 15,  5,-30},
                            {-30,  0, 15, 20, 20, 15,  0,-30},
                            {-30,  5, 10, 15, 15, 10,  5,-30},
                            {-40,-20,  0,  5,  5,  0,-20,-40},
                            {-50,-40,-30,-30,-30,-30,-40,-50}};

// MATRICE POIDS FOU
int fouB_poids[8][8] = {{-20,-10,-10,-10,-10,-10,-10,-20},
                        {-10,  0,  0,  0,  0,  0,  0,-10},
                        {-10,  0,  5, 10, 10,  5,  0,-10},
                        {-10,  5,  5, 10, 10,  5,  5,-10},
                        {-10,  0, 10, 10, 10, 10,  0,-10},
                        {-10, 10, 10, 10, 10, 10, 10,-10},
                        {-10,  5,  0,  0,  0,  0,  5,-10},
                        {-20,-10,-10,-10,-10,-10,-10,-20}};

// MATRICE POIDS TOUT
int tourB_poids[8][8] = { {0,  0,  0,  0,  0,  0,  0,  0},
                         {5, 10, 10, 10, 10, 10, 10,  5},
                         {-5,  0,  0,  0,  0,  0,  0, -5},
                         {-5,  0,  0,  0,  0,  0,  0, -5},
                         {-5,  0,  0,  0,  0,  0,  0, -5},
                         {-5,  0,  0,  0,  0,  0,  0, -5},
                         {-5,  0,  0,  0,  0,  0,  0, -5},
                          {0,  0,  0,  5,  5,  0,  0,  0}};

// MATRICE POIDS REINE
int reineB_poids[8][8] = { {-20,-10,-10, -5, -5,-10,-10,-20},
                            {-10,  0,  0,  0,  0,  0,  0,-10},
                            {-10,  0,  5,  5,  5,  5,  0,-10},
                            {-5,  0,  5,  5,  5,  5,  0, -5},
                            {0,  0,  5,  5,  5,  5,  0, -5},
                            {-10,  5,  5,  5,  5,  5,  0,-10},
                            {-10,  0,  5,  0,  0,  0,  0,-10},
                            {-20,-10,-10, -5, -5,-10,-10,-20}};

// MATRICE POIDS ROI
int roiB_poids[8][8] = {{-30,-40,-40,-50,-50,-40,-40,-30},
                        {-30,-40,-40,-50,-50,-40,-40,-30},
                        {-30,-40,-40,-50,-50,-40,-40,-30},
                        {-30,-40,-40,-50,-50,-40,-40,-30},
                        {-20,-30,-30,-40,-40,-30,-30,-20},
                        {-10,-20,-20,-20,-20,-20,-20,-10},
                        {20, 20,  0,  0,  0,  0, 20, 20},
                         {20, 30, 10,  0,  0, 10, 30, 20}};

// MATRICE POIDS ROI FIN
int fin_roiB_poids[8][8] = {{-50,-40,-30,-20,-20,-30,-40,-50},
                            {-30,-20,-10,  0,  0,-10,-20,-30},
                            {-30,-10, 20, 30, 30, 20,-10,-30},
                            {-30,-10, 30, 40, 40, 30,-10,-30},
                            {-30,-10, 30, 40, 40, 30,-10,-30},
                            {-30,-10, 20, 30, 30, 20,-10,-30},
                            {-30,-30,  0,  0,  0,  0,-30,-30},
                            {-50,-30,-30,-30,-30,-30,-30,-50}};

//NOIR
// MATRICE POIDS PION
int pionN_poids[8][8] = {{0,  0,  0,  0,  0,  0,  0,  0},
                        {5, 10, 10,-20,-20, 10, 10,  5},
                        {5, -5,-10,  0,  0,-10, -5,  5},
                        {0,  0,  0, 20, 20,  0,  0,  0},
                        {5,  5, 10, 25, 25, 10,  5,  5},
                        {10, 10, 20, 30, 30, 20, 10, 10},
                        {50, 50, 50, 50, 50, 50, 50, 50},
                        {0,  0,  0,  0,  0,  0,  0,  0}};

// MATRICE POIDS CHEVALIER
int chevalierN_poids[8][8] = {{-50,-40,-30,-30,-30,-30,-40,-50},
                            {-40,-20,  0,  5,  5,  0,-20,-40},
                            {-30,  5, 10, 15, 15, 10,  5,-30},
                            {-30,  0, 15, 20, 20, 15,  0,-30},
                            {-30,  5, 15, 20, 20, 15,  5,-30},
                            {-30,  0, 10, 15, 15, 10,  0,-30},
                            {-40,-20,  0,  0,  0,  0,-20,-40},
                            {-50,-40,-30,-30,-30,-30,-40,-50}};

// MATRICE POIDS FOU
int fouN_poids[8][8] = {{-20,-10,-10,-10,-10,-10,-10,-20},
                        {-10,  5,  0,  0,  0,  0,  5,-10},
                        {-10, 10, 10, 10, 10, 10, 10,-10},
                         {-10,  0, 10, 10, 10, 10,  0,-10},
                        {-10,  5,  5, 10, 10,  5,  5,-10},
                        {-10,  0,  5, 10, 10,  5,  0,-10},
                        {-10,  0,  0,  0,  0,  0,  0,-10},
                        {-20,-10,-10,-10,-10,-10,-10,-20}};

// MATRICE POIDS TOUT
int tourN_poids[8][8] = { {0,  0,  0,  5,  5,  0,  0,  0},
                         {-5,  0,  0,  0,  0,  0,  0, -5},
                          {-5,  0,  0,  0,  0,  0,  0, -5},
                         {-5,  0,  0,  0,  0,  0,  0, -5},
                         {-5,  0,  0,  0,  0,  0,  0, -5},
                         {-5,  0,  0,  0,  0,  0,  0, -5},
                          {5, 10, 10, 10, 10, 10, 10,  5},
                         {0,  0,  0,  0,  0,  0,  0,  0}};

// MATRICE POIDS REINE
int reineN_poids[8][8] = { {-20,-10,-10, -5, -5,-10,-10,-20},
                            {-10,  0,  5,  0,  0,  0,  0,-10},
                            {-10,  0,  5,  5,  5,  5,  0,-10},
                            {0,  0,  5,  5,  5,  5,  0, -5},
                            {-5,  0,  5,  5,  5,  5,  0, -5},
                            {-10,  5,  5,  5,  5,  5,  0,-10},
                           {-10,  0,  0,  0,  0,  0,  0,-10},
                            {-20,-10,-10, -5, -5,-10,-10,-20}};

// MATRICE POIDS ROI
int roiN_poids[8][8] = { {20, 30, 10,  0,  0, 10, 30, 20},
                         {20, 20,  0,  0,  0,  0, 20, 20},
                        {-10,-20,-20,-20,-20,-20,-20,-10},
                        {-20,-30,-30,-40,-40,-30,-30,-20},
                        {-30,-40,-40,-50,-50,-40,-40,-30},
                        {-30,-40,-40,-50,-50,-40,-40,-30},
                       {-30,-40,-40,-50,-50,-40,-40,-30},
                        {-30,-40,-40,-50,-50,-40,-40,-30} };

// MATRICE POIDS ROI FIN
int fin_roiN_poids[8][8] = { {-50,-30,-30,-30,-30,-30,-30,-50},
                            {-30,-30,  0,  0,  0,  0,-30,-30},
                            {-30,-10, 20, 30, 30, 20,-10,-30},
                            {-30,-10, 30, 40, 40, 30,-10,-30},
                            {-30,-10, 30, 40, 40, 30,-10,-30},
                            {-30,-10, 20, 30, 30, 20,-10,-30},
                              {-30,-20,-10,  0,  0,-10,-20,-30},
                            {-50,-40,-30,-20,-20,-30,-40,-50}};
// evalue avec alpha beta la configuration 'conf' du joueur 'mode' en descendant de 'niv' niveaux
int minmax_ab( struct config conf, int mode, int niv, int min, int max );


/* Copie la configuration c1 dans c2  */
void copier( struct config *c1, struct config *c2 )
{
	int i, j;

	for (i=0; i<8; i++)
		for (j=0; j<8; j++)
			c2->mat[i][j] = c1->mat[i][j];

	c2->val = c1->val;
	c2->xrB = c1->xrB;
	c2->yrB = c1->yrB;
	c2->xrN = c1->xrN;
	c2->yrN = c1->yrN;

	c2->roqueB = c1->roqueB;
	c2->roqueN = c1->roqueN;
} // copier


/* Teste si les conf c1 et c2 sont égales */
int egal(char c1[8][8], char c2[8][8] )
{
	int i, j;

	for (i=0; i<8; i++)
		for (j=0; j<8; j++)
			if (c1[i][j] != c2[i][j]) return 0;
	return 1;
} // egal




/***********************************************************/
/*********** Partie:  Evaluations et Estimations ***********/
/***********************************************************/


/* Teste s'il n'y a aucun coup possible dans la configuration conf */
int AucunCoupPossible( struct config conf )
{
      	// ... A completer pour les matchs nuls
	// ... vérifier que generer_succ retourne 0 configurations filles ...
	return 0;

} // AucunCoupPossible


/* Teste si conf représente une fin de partie et retourne dans 'cout' la valeur associée */
int feuille( struct config conf, int *cout )
{
	//int i, j, rbx, rnx, rby, rny;

	*cout = 0;

	// Si victoire pour les Noirs cout = -100
	if ( conf.xrB == -1 ) {
	   *cout = -100;
	   return 1;
	}

	// Si victoire pour les Blancs cout = +100
	if ( conf.xrN == -1 ) {
	   *cout = +100;
	   return 1;
	}

	// Si Match nul cout = 0
	if (  conf.xrB != -1 &&  conf.xrN != -1 && AucunCoupPossible( conf ) )
	   return 1;

	// Sinon ce n'est pas une config feuille
	return 0;

}  // feuille


/* Retourne une estimation de la configuration conf */

/*
int estim( struct config conf )
{

// ********************************************************
// Remplacer ce code par une estimation de meilleur qualité
// ********************************************************

	int i, j, ScrQte;
	int pionB = 0, pionN = 0, cB = 0, cN = 0, tB = 0, tN = 0, nB = 0, nN = 0 , fB=0,fN=0 ,rN=0,rB=0;
	int pionB_mob = 0, pionN_mob = 0, cB_mob = 0, cN_mob = 0, tB_mob = 0, tN_mob = 0, nB_mob = 0, nN_mob = 0 ,rB_mob = 0, rN_mob = 0, fB_mob=0,fN_mob=0;
	int cpt_blockedN=0 , cpt_blockedB=0;
	int blocked_pawns=0;
    int cpt;
	struct config T[100];

	int material =0;
	int mobility =0;
    //p : pion : pawn
    //c : cavalier : knight
    //f : fou : bishop
    //t : tour : rook
    //n : reine : Queen
    //r : roi : king

	// parties : nombre de pièces et occupation du centre
	for (i=0; i<8; i++)
	   for (j=0; j<8; j++) {

		switch (conf.mat[i][j]) {

		//   case 'p' : pionB++;   ;     cpt=0;   cpt=mobilityB(conf,i, j);  pionB_mob+=cpt;  if (cpt==0) {cpt_blockedB++;}    break;
		 //  case 'c' : cB++;              cpt=0; cpt=mobilityB(conf,i, j); cB_mob+=cpt;   break;
		 //  case 'f' : fB++;              cpt=0;  cpt=mobilityB(conf,i, j); fB_mob+=cpt;    break;
		  // case 't' : tB++;              cpt=0;  cpt=mobilityB(conf,i, j);  tB_mob+=cpt;     break;
		  // case 'n' : nB++;             cpt=0;  cpt=mobilityB(conf,i, j);  nB_mob+=cpt;   break;
          // case 'r' :  ;                 cpt=0;  cpt=mobilityB(conf,i, j);   rB_mob+=cpt; break;


           case 'p' : pionB++;   ;     cpt=0;   deplacementsB(conf, i, j, T,&cpt);  pionB_mob+=cpt;  if (cpt==0) {cpt_blockedB++;}    break;
		   case 'c' : cB++;              cpt=0; deplacementsB(conf, i, j, T,&cpt); cB_mob+=cpt;   break;
		   case 'f' : fB++;              cpt=0;  deplacementsB(conf, i, j, T,&cpt); fB_mob+=cpt;    break;
		   case 't' : tB++;              cpt=0;  deplacementsB(conf, i, j, T,&cpt);  tB_mob+=cpt;     break;
		   case 'n' : nB++;             cpt=0;  deplacementsB(conf, i, j, T,&cpt); nB_mob+=cpt;   break;
           case 'r' : rB++ ;                 cpt=0;  deplacementsB(conf, i, j, T,&cpt);  rB_mob+=cpt; break;

		   case -'p' : pionN++;     cpt=0;   deplacementsN(conf, i, j, T,&cpt);  pionN_mob+=cpt;  if (cpt==0) {cpt_blockedN++;} break;
		   case -'c' : cN++;         cpt=0;    deplacementsN(conf, i, j, T,&cpt); cN_mob+=cpt;   break;
		   case -'f' : fN++;         cpt=0;    deplacementsN(conf, i, j, T,&cpt);  fN_mob+=cpt;  break;
		   case -'t' : tN++;        cpt=0;     deplacementsN(conf, i, j, T,&cpt); tN_mob+=cpt;   break;
		   case -'n' : nN++;           cpt=0;    deplacementsN(conf, i, j, T,&cpt);  nN_mob+=cpt;  break;
           case -'r' :  rN++;              cpt=0;  deplacementsN(conf, i, j, T,&cpt); rN_mob+=cpt;  break;
		}
	   }

	   //printf("fin estimation");

	//materials
	material = (pionB*1 + cB*3 +fB*3 + tB*5 + nB*9+ 200*rB) - (pionN*1 + cN*3 + fN*3 + tN*5 + nN*9+ 200*rN) ;

    //mobility
    mobility = ( (pionB_mob + cB_mob +fB_mob + tB_mob + nB_mob +rB_mob ) - (pionN_mob + cN_mob + fN_mob + tN_mob + nN_mob + rN_mob ) )*0.1;

    //blocked_pawns
    blocked_pawns = (cpt_blockedB- cpt_blockedN) * (-0.5);

    ScrQte = (material + mobility + blocked_pawns )*1.4; //je ne sais si on enleve le 1.4 ou pas pour ne pas sortir de l'intervalle [-100,100] comme il a dit

	if (ScrQte > 95) ScrQte = 95;		// pour rétrécir l'intervalle à
	if (ScrQte < -95) ScrQte = -95;		// ]-95 , +95[ car ce n'est qu'une estimation

	return ScrQte;

} //

*/


int ending_game(struct config conf)
{
    int continu = 1;
    int reineB =0, reineN=0;
    int i=0, j=0;
    while(continu == 1)
        {
            if(conf.mat[i][j]==(1)*'n')
            {
                reineB++;
            }
            if(conf.mat[i][j]==(-1)*'n')
            {
                reineN++;
            }
            if (reineB+reineN==2) // ou ==1
            {
                continu=0;
            }
            else
            {
                j++;
                if(j==8)
                {
                    i++; j=0;
                    if(i==8) continu=0;
                }
            }

        }
    if (reineB+reineN==0) return 1; //Aucun des deux n'a de reine ie ending game
    else return 0; // Il y a une ou deux reine ie middle game

}  /* "- Every side which has a queen has additionally no other pieces or one minorpiece maximum." pas prise en considérantion */


/* Retourne une estimation de la configuration conf */
int estim( struct config conf )
{

// ********************************************************
// Remplacer ce code par une estimation de meilleur qualité
// ********************************************************

	int i, j, ScrQte;
	int pionB = 0, pionN = 0, cB = 0, cN = 0, tB = 0, tN = 0, nB = 0, nN = 0 , fB=0,fN=0 , rB=0,rN=0;
	int pionB_mob = 0, pionN_mob = 0, cB_mob = 0, cN_mob = 0, tB_mob = 0, tN_mob = 0, nB_mob = 0, nN_mob = 0 ,rB_mob = 0, rN_mob = 0, fB_mob=0,fN_mob=0;
	int cpt_blockedN=0 , cpt_blockedB=0;
	int blocked_pawns, doubled_pawns, isolated_pawns;
    int cpt;
	struct config T[100];

	int doubledB=0;
	int doubledN=0;

	int isolatedB=0;
	int isolatedN=0;

	int material =0;
	int mobility =0;

	int poids_totalB = 0;
	int poids_totalN = 0;
	int poids =0;

    //p : pion : pawn
    //c : cavalier : knight
    //f : fou : bishop
    //t : tour : rook
    //n : reine : Queen
    //r : roi : king

	// parties : nombre de pièces et occupation du centre
	for (i=0; i<8; i++)
	   for (j=0; j<8; j++) {

		switch (conf.mat[i][j]) {
		   case 'p' : pionB++;      cpt=0;      deplacementsB(conf, i, j, T,&cpt); pionB_mob+=cpt;  if (cpt==0) {cpt_blockedB++;}
                                    if(doubledpawn(i,j,1,conf)==1) doubledB++;
                                    if(isolatedpawn(i,j,1,conf)==1) isolatedB++;
                                    poids_totalB+=pionB_poids[i][j]; break;

		   case 'c' : cB++;              cpt=0; deplacementsB(conf, i, j, T,&cpt); cB_mob+=cpt; poids_totalB+=chevalierB_poids[i][j]; break;
		   case 'f' : fB++;             cpt=0;  deplacementsB(conf, i, j, T,&cpt); fB_mob+=cpt; poids_totalB+=fouB_poids[i][j];    break;
		   case 't' : tB++;             cpt=0;  deplacementsB(conf, i, j, T,&cpt);  tB_mob+=cpt;   poids_totalB+=tourB_poids[i][j];  break;
		   case 'n' : nB++;          cpt=0;  deplacementsB(conf, i, j, T,&cpt);  nB_mob+=cpt;  poids_totalB+=reineB_poids[i][j];  break;
            case 'r' :                cpt=0;  deplacementsB(conf, i, j, T,&cpt); rB_mob+=cpt;
                                    if(ending_game(conf)==0) poids_totalB+=roiB_poids[i][j];
                                     else poids_totalB+=fin_roiB_poids[i][j]; break;

		   case -'p' : pionN++;        cpt=0;   deplacementsN(conf, i, j, T,&cpt);  pionN_mob+=cpt;
                                        if (cpt==0) {cpt_blockedN++;}
                                         if(doubledpawn(i,j,-1,conf)==1) doubledN++;
                                         if(isolatedpawn(i,j,-1,conf)==1) isolatedN++;
                                         poids_totalN+=pionN_poids[i][j]; break;

		   case -'c' : cN++;           cpt=0;    deplacementsN(conf, i, j, T,&cpt); cN_mob+=cpt;
                                       poids_totalN+=chevalierN_poids[i][j];  break;

		   case -'f' : fN++;         cpt=0;    deplacementsN(conf, i, j, T,&cpt);  fN_mob+=cpt;
                                    poids_totalN+=fouN_poids[i][j]; break;

		   case -'t' : tN++;        cpt=0;     deplacementsN(conf, i, j, T,&cpt); tN_mob+=cpt;
                                         poids_totalN+=tourN_poids[i][j]; break;

		   case -'n' : nN++;          cpt=0;    deplacementsN(conf, i, j, T,&cpt);  nN_mob+=cpt;
                                   poids_totalN+=reineN_poids[i][j]; break;

           case -'r' :               cpt=0;    deplacementsN(conf, i, j, T,&cpt); rN_mob+=cpt;
                                     if(ending_game(conf)==0) poids_totalN+=roiN_poids[i][j];
                                     else poids_totalN+=fin_roiN_poids[i][j]; break;
		}
	   }



	//materials
	material = (pionB*1 + cB*3 +fB*3 + tB*5 + nB*9+ 200*rB) - (pionN*1 + cN*3 + fN*3 + tN*5 + nN*9+ 200*rN) ;

    //mobility
    mobility = ( (pionB_mob + cB_mob +fB_mob + tB_mob + nB_mob +rB_mob ) - (pionN_mob + cN_mob + fN_mob + tN_mob + nN_mob + rN_mob ) )*0.1;

    //blocked_pawns
    blocked_pawns = (cpt_blockedB- cpt_blockedN) * (-0.5);

    //Doubled_pawns
    doubled_pawns = (doubledB - doubledN) * (-0.5);

    //Isolated_pawns
    isolated_pawns = (isolatedB - isolatedN) * (-0.5);

    //Weight_pawns
    poids = (poids_totalB - poids_totalN) * (1);



    ScrQte = (material + mobility + blocked_pawns + doubled_pawns + isolated_pawns )*1.4; //je ne sais si on enleve le 1.4 ou pas pour ne pas sortir de l'intervalle [-100,100] comme il a dit

	if (ScrQte > 95) ScrQte = 95;		// pour rétrécir l'intervalle à
	if (ScrQte < -95) ScrQte = -95;		// ]-95 , +95[ car ce n'est qu'une estimation

	return ScrQte;

} //


int doubledpawn(int x, int y, int color, struct config conf)
{
    int i=0;
    int continu=1;
    int doubled=0;

   // char pawn = color * 'p';
    // if(color==1)  strcpy (pawn,"p"); strcpy (pawn,"-p");

    while(continu==1)
    {


        if(conf.mat[i][y]==color * 'p')
        {

                continu=0; doubled=1;
        }
        else
        {

            i++; if(i==8) continu=0;
        }


    }

    return doubled;
}


int isolatedpawn(int x, int y, int color, struct config conf)
{
    int i=0;
    int continu=1;
    int isolated=1;

    if ((y>0)&&(y<7))
    {
        while(continu==1)
        {


            if((conf.mat[i][y-1]==color * 'p')||(conf.mat[i][y+1]==color * 'p'))
            {

                continu=0; isolated=0;
            }
            else
            {

                i++; if(i==8) continu=0;
            }
        }
    }
    else
    {
        if(y==0)
        {
            while(continu==1)
            {


                if(conf.mat[i][y+1]==color * 'p')
                {

                    continu=0; isolated=0;
                }
                else
                {

                    i++; if(i==8) continu=0;
                }
            }
        }
        else // y==7
        {
            while(continu==1)
            {


                if(conf.mat[i][y-1]==color * 'p')
                {

                    continu=0; isolated=0;
                }
                else
                {

                    i++; if(i==8) continu=0;
                }
            }
        }
    }

    return isolated;
}


/***********************************************************/
/*********** Partie:  Génération des Successeurs ***********/
/***********************************************************/


/* Génère dans T les configurations obtenues à partir de conf lorsqu'un pion atteint la limite de l'échiq */
void transformPion( struct config conf, int a, int b, int x, int y, struct config T[], int *n )
{
	int signe = +1;
	if (conf.mat[a][b] < 0 ) signe = -1;
	copier(&conf, &T[*n]);
	T[*n].mat[a][b] = 0;
	T[*n].mat[x][y] = signe *'n';
	(*n)++;
	copier(&conf, &T[*n]);
	T[*n].mat[a][b] = 0;
	T[*n].mat[x][y] = signe *'c';
	(*n)++;
	copier(&conf, &T[*n]);
	T[*n].mat[a][b] = 0;
	T[*n].mat[x][y] = signe *'f';
	(*n)++;
	copier(&conf, &T[*n]);
	T[*n].mat[a][b] = 0;
	T[*n].mat[x][y] = signe *'t';
	(*n)++;

} // transformPion


// Vérifie si la case (x,y) est menacée par une des pièces du joueur 'mode'
int caseMenaceePar( int mode, int x, int y, struct config conf )
{
	int i, j, a, b, stop;

	// menace par le roi ...
	for (i=0; i<8; i += 1) {
	   // traitement des 8 directions paires et impaires
	   a = x + D[i][0];
	   b = y + D[i][1];
	   if ( a >= 0 && a <= 7 && b >= 0 && b <= 7 )
		if ( conf.mat[a][b]*mode == 'r' ) return 1;
	} // for

	// menace par cavalier ...
	for (i=0; i<8; i++)
	   if ( x+dC[i][0] <= 7 && x+dC[i][0] >= 0 && y+dC[i][1] <= 7 && y+dC[i][1] >= 0 )
		if ( conf.mat[ x+dC[i][0] ] [ y+dC[i][1] ] * mode == 'c' )
		   return 1;

	// menace par pion ...
	if ( (x-mode) >= 0 && (x-mode) <= 7 && y > 0 && conf.mat[x-mode][y-1]*mode == 'p' )
	   return 1;
	if ( (x-mode) >= 0 && (x-mode) <= 7 && y < 7 && conf.mat[x-mode][y+1]*mode == 'p' )
	   return 1;

	// menace par fou, tour ou reine ...
	for (i=0; i<8; i += 1) {
	   // traitement des 8 directions paires et impaires
	   stop = 0;
	   a = x + D[i][0];
	   b = y + D[i][1];
	   while ( !stop && a >= 0 && a <= 7 && b >= 0 && b <= 7 )
		if ( conf.mat[a][b] != 0 )  stop = 1;
		else {
		    a = a + D[i][0];
		    b = b + D[i][1];
		}
	   if ( stop )  {
		if ( conf.mat[a][b]*mode == 'f' && i % 2 != 0 ) return 1;
		if ( conf.mat[a][b]*mode == 't' && i % 2 == 0 ) return 1;
		if ( conf.mat[a][b]*mode == 'n' ) return 1;
	   }
	} // for

	// sinon, aucune menace ...
	return 0;

} // caseMenaceePar


/* Génere dans T tous les coups possibles de la pièce (de couleur N) se trouvant à la pos x,y */
void deplacementsN(struct config conf, int x, int y, struct config T[], int *n )
{
	int i, j, a, b, stop;

	switch(conf.mat[x][y]) {
	// mvmt PION ...
	case -'p' :
		//***printf("PION N à la pos (%d,%d) \n", x,y);
		if ( x > 0 && conf.mat[x-1][y] == 0 ) {				// avance d'une case
			copier(&conf, &T[*n]);
			T[*n].mat[x][y] = 0;
			T[*n].mat[x-1][y] = -'p';
			(*n)++;
			if ( x == 1 ) transformPion( conf, x, y, x-1, y, T, n );
		}
		if ( x == 6 && conf.mat[5][y] == 0 && conf.mat[4][y] == 0) {	// avance de 2 cases
			copier(&conf, &T[*n]);
			T[*n].mat[6][y] = 0;
			T[*n].mat[4][y] = -'p';
			(*n)++;
		}
		if ( x > 0 && y >0 && conf.mat[x-1][y-1] > 0 ) {		// mange à droite (en descendant)
			copier(&conf, &T[*n]);
			T[*n].mat[x][y] = 0;
			T[*n].mat[x-1][y-1] = -'p';
			// cas où le roi adverse est pris...
			if (T[*n].xrB == x-1 && T[*n].yrB == y-1) {
				T[*n].xrB = -1; T[*n].yrB = -1;
			}

			(*n)++;
			if ( x == 1 ) transformPion( conf, x, y, x-1, y-1, T, n );
		}
		if ( x > 0 && y < 7 && conf.mat[x-1][y+1] > 0 ) {		// mange à gauche (en descendant)
			copier(&conf, &T[*n]);
			T[*n].mat[x][y] = 0;
			T[*n].mat[x-1][y+1] = -'p';
			// cas où le roi adverse est pris...
			if (T[*n].xrB == x-1 && T[*n].yrB == y+1) {
				T[*n].xrB = -1; T[*n].yrB = -1;
			}

			(*n)++;
			if ( x == 1 ) transformPion( conf, x, y, x-1, y+1, T, n );
		}
		break;

	// mvmt CAVALIER ...
	case -'c' :
		for (i=0; i<8; i++)
		   if ( x+dC[i][0] <= 7 && x+dC[i][0] >= 0 && y+dC[i][1] <= 7 && y+dC[i][1] >= 0 )
			if ( conf.mat[ x+dC[i][0] ] [ y+dC[i][1] ] >= 0 )  {
			   copier(&conf, &T[*n]);
			   T[*n].mat[x][y] = 0;
			   T[*n].mat[ x+dC[i][0] ][ y+dC[i][1] ] = -'c';
			   // cas où le roi adverse est pris...
			   if (T[*n].xrB == x+dC[i][0] && T[*n].yrB == y+dC[i][1]) {
				T[*n].xrB = -1; T[*n].yrB = -1;
			   }

			   (*n)++;
			}
		break;

	// mvmt FOU ...
	case -'f' :
		for (i=1; i<8; i += 2) {
		   // traitement des directions impaires (1, 3, 5 et 7)
		   stop = 0;
		   a = x + D[i][0];
		   b = y + D[i][1];
		   while ( !stop && a >= 0 && a <= 7 && b >= 0 && b <= 7 ) {
			if ( conf.mat[ a ] [ b ] < 0 )  stop = 1;
			else {
			   copier(&conf, &T[*n]);
			   T[*n].mat[x][y] = 0;
			   if ( T[*n].mat[a][b] > 0 ) stop = 1;
			   T[*n].mat[a][b] = -'f';
			   // cas où le roi adverse est pris...
			   if (T[*n].xrB == a && T[*n].yrB == b) { T[*n].xrB = -1; T[*n].yrB = -1; }

			   (*n)++;
		   	   a = a + D[i][0];
		   	   b = b + D[i][1];
			}
		   } // while
		} // for
		break;

	// mvmt TOUR ...
	case -'t' :
		for (i=0; i<8; i += 2) {
		   // traitement des directions paires (0, 2, 4 et 6)
		   stop = 0;
		   a = x + D[i][0];
		   b = y + D[i][1];
		   while ( !stop && a >= 0 && a <= 7 && b >= 0 && b <= 7 ) {
			if ( conf.mat[ a ] [ b ] < 0 )  stop = 1;
			else {
			   copier(&conf, &T[*n]);
			   T[*n].mat[x][y] = 0;
			   if ( T[*n].mat[a][b] > 0 ) stop = 1;
			   T[*n].mat[a][b] = -'t';
			   // cas où le roi adverse est pris...
			   if (T[*n].xrB == a && T[*n].yrB == b) { T[*n].xrB = -1; T[*n].yrB = -1; }

			   if ( conf.roqueN != 'e' && conf.roqueN != 'n' ) {
			      if ( x == 7 && y == 0 && conf.roqueN != 'p')
			   	T[*n].roqueN = 'g'; // le grand roque ne sera plus possible
			      else if ( x == 7 && y == 0 )
			   	   T[*n].roqueN = 'n'; // ni le grand roque ni le petit roque ne seront possibles
			      if ( x == 7 && y == 7 && conf.roqueN != 'g' )
			   	T[*n].roqueN = 'p'; // le petit roque ne sera plus possible
			      else if ( x == 7 && y == 7 )
			   	   T[*n].roqueN = 'n'; // ni le grand roque ni le petit roque ne seront possibles
			   }

			   (*n)++;
		   	   a = a + D[i][0];
		   	   b = b + D[i][1];
			}
		   } // while
		} // for
		break;

	// mvmt REINE ...
	case -'n' :
		for (i=0; i<8; i += 1) {
		   // traitement des 8 directions paires et impaires
		   stop = 0;
		   a = x + D[i][0];
		   b = y + D[i][1];
		   while ( !stop && a >= 0 && a <= 7 && b >= 0 && b <= 7 ) {
			if ( conf.mat[ a ] [ b ] < 0 )  stop = 1;
			else {
			   copier(&conf, &T[*n]);
			   T[*n].mat[x][y] = 0;
			   if ( T[*n].mat[a][b] > 0 ) stop = 1;
			   T[*n].mat[a][b] = -'n';
			   // cas où le roi adverse est pris...
			   if (T[*n].xrB == a && T[*n].yrB == b) { T[*n].xrB = -1; T[*n].yrB = -1; }

			   (*n)++;
		   	   a = a + D[i][0];
		   	   b = b + D[i][1];
			}
		   } // while
		} // for
		break;

	// mvmt ROI ...
	case -'r' :
		// vérifier possibilité de faire un roque ...
		if ( conf.roqueN != 'n' && conf.roqueN != 'e' ) {
		   if ( conf.roqueN != 'g' && conf.mat[7][1] == 0 && conf.mat[7][2] == 0 && conf.mat[7][3] == 0 )
		      if ( !caseMenaceePar( MAX, 7, 1, conf ) && !caseMenaceePar( MAX, 7, 2, conf ) && \
			   !caseMenaceePar( MAX, 7, 3, conf ) && !caseMenaceePar( MAX, 7, 4, conf ) )  {
			// Faire un grand roque ...
			copier(&conf, &T[*n]);
			T[*n].mat[7][4] = 0;
			T[*n].mat[7][0] = 0;
			T[*n].mat[7][2] = -'r'; T[*n].xrN = 7; T[*n].yrN = 2;
			T[*n].mat[7][3] = -'t';
			T[*n].roqueN = 'e'; // aucun roque ne sera plus possible à partir de cette config
			(*n)++;
		      }
		   if ( conf.roqueN != 'p' && conf.mat[7][5] == 0 && conf.mat[7][6] == 0 )
		      if ( !caseMenaceePar( MAX, 7, 4, conf ) && !caseMenaceePar( MAX, 7, 5, conf ) && \
			   !caseMenaceePar( MAX, 7, 6, conf ) )  {
			// Faire un petit roque ...
			copier(&conf, &T[*n]);
			T[*n].mat[7][4] = 0;
			T[*n].mat[7][7] = 0;
			T[*n].mat[7][6] = -'r'; T[*n].xrN = 7; T[*n].yrN = 6;
			T[*n].mat[7][5] = -'t';
			T[*n].roqueN = 'e'; // aucun roque ne sera plus possible à partir de cette config
			(*n)++;

		      }
		}

		// vérifier les autres mouvements du roi ...
		for (i=0; i<8; i += 1) {
		   // traitement des 8 directions paires et impaires
		   a = x + D[i][0];
		   b = y + D[i][1];
		   if ( a >= 0 && a <= 7 && b >= 0 && b <= 7 )
			if ( conf.mat[a][b] >= 0 ) {
			   copier(&conf, &T[*n]);
			   T[*n].mat[x][y] = 0;
			   T[*n].mat[a][b] = -'r'; T[*n].xrN = a; T[*n].yrN = b;
			   // cas où le roi adverse est pris...
			   if (T[*n].xrB == a && T[*n].yrB == b) { T[*n].xrB = -1; T[*n].yrB = -1; }

			   T[*n].roqueN = 'n'; // aucun roque ne sera plus possible à partir de cette config
			   (*n)++;
			}
		} // for
		break;

	}

} // deplacementsN


/* Génere dans T tous les coups possibles de la pièce (de couleur B) se trouvant à la pos x,y */
void deplacementsB(struct config conf, int x, int y, struct config T[], int *n )
{
	int i, j, a, b, stop;

	switch(conf.mat[x][y]) {
	// mvmt PION ...
	case 'p' :
		if ( x <7 && conf.mat[x+1][y] == 0 ) {				// avance d'une case
			copier(&conf, &T[*n]);
			T[*n].mat[x][y] = 0;
			T[*n].mat[x+1][y] = 'p';
			(*n)++;
			if ( x == 6 ) transformPion( conf, x, y, x+1, y, T, n );
		}
		if ( x == 1 && conf.mat[2][y] == 0 && conf.mat[3][y] == 0) {	// avance de 2 cases
			copier(&conf, &T[*n]);
			T[*n].mat[1][y] = 0;
			T[*n].mat[3][y] = 'p';
			(*n)++;
		}
		if ( x < 7 && y > 0 && conf.mat[x+1][y-1] < 0 ) {		// mange à gauche (en montant)
			copier(&conf, &T[*n]);
			T[*n].mat[x][y] = 0;
			T[*n].mat[x+1][y-1] = 'p';
			// cas où le roi adverse est pris...
			if (T[*n].xrN == x+1 && T[*n].yrN == y-1) {
				T[*n].xrN = -1; T[*n].yrN = -1;
			}

			(*n)++;
			if ( x == 6 ) transformPion( conf, x, y, x+1, y-1, T, n );
		}
		if ( x < 7 && y < 7 && conf.mat[x+1][y+1] < 0 ) {		// mange à droite (en montant)
			copier(&conf, &T[*n]);
			T[*n].mat[x][y] = 0;
			T[*n].mat[x+1][y+1] = 'p';
			// cas où le roi adverse est pris...
			if (T[*n].xrN == x+1 && T[*n].yrN == y+1) {
				T[*n].xrN = -1; T[*n].yrN = -1;
			}

			(*n)++;
			if ( x == 6 ) transformPion( conf, x, y, x+1, y+1, T, n );
		}
		break;

	// mvmt CAVALIER ...
	case 'c' :
		for (i=0; i<8; i++)
		   if ( x+dC[i][0] <= 7 && x+dC[i][0] >= 0 && y+dC[i][1] <= 7 && y+dC[i][1] >= 0 )
			if ( conf.mat[ x+dC[i][0] ] [ y+dC[i][1] ] <= 0 )  {
			   copier(&conf, &T[*n]);
			   T[*n].mat[x][y] = 0;
			   T[*n].mat[ x+dC[i][0] ][ y+dC[i][1] ] = 'c';
			   // cas où le roi adverse est pris...
			   if (T[*n].xrN == x+dC[i][0] && T[*n].yrN == y+dC[i][1]) {
				T[*n].xrN = -1; T[*n].yrN = -1;
			   }

			   (*n)++;
			}
		break;

	// mvmt FOU ...
	case 'f' :
		for (i=1; i<8; i += 2) {
		   // traitement des directions impaires (1, 3, 5 et 7)
		   stop = 0;
		   a = x + D[i][0];
		   b = y + D[i][1];
		   while ( !stop && a >= 0 && a <= 7 && b >= 0 && b <= 7 ) {
			if ( conf.mat[ a ] [ b ] > 0 )  stop = 1;
			else {
			   copier(&conf, &T[*n]);
			   T[*n].mat[x][y] = 0;
			   if ( T[*n].mat[a][b] < 0 ) stop = 1;
			   T[*n].mat[a][b] = 'f';
			   // cas où le roi adverse est pris...
			   if (T[*n].xrN == a && T[*n].yrN == b) { T[*n].xrN = -1; T[*n].yrN = -1; }

			   (*n)++;
		   	   a = a + D[i][0];
		   	   b = b + D[i][1];
			}
		   } // while
		} // for
		break;

	// mvmt TOUR ...
	case 't' :
		for (i=0; i<8; i += 2) {
		   // traitement des directions paires (0, 2, 4 et 6)
		   stop = 0;
		   a = x + D[i][0];
		   b = y + D[i][1];
		   while ( !stop && a >= 0 && a <= 7 && b >= 0 && b <= 7 ) {
			if ( conf.mat[ a ] [ b ] > 0 )  stop = 1;
			else {
			   copier(&conf, &T[*n]);
			   T[*n].mat[x][y] = 0;
			   if ( T[*n].mat[a][b] < 0 ) stop = 1;
			   T[*n].mat[a][b] = 't';
			   // cas où le roi adverse est pris...
			   if (T[*n].xrN == a && T[*n].yrN == b) { T[*n].xrN = -1; T[*n].yrN = -1; }

			   if ( conf.roqueB != 'e' && conf.roqueB != 'n' ) {
			     if ( x == 0 && y == 0 && conf.roqueB != 'p')
			   	T[*n].roqueB = 'g'; // le grand roque ne sera plus possible
			     else if ( x == 0 && y == 0 )
			   	   T[*n].roqueB = 'n'; // ni le grand roque ni le petit roque ne seront possibles
			     if ( x == 0 && y == 7 && conf.roqueB != 'g' )
			   	T[*n].roqueB = 'p'; // le petit roque ne sera plus possible
			     else if ( x == 0 && y == 7 )
			   	   T[*n].roqueB = 'n'; // ni le grand roque ni le petit roque ne seront possibles
			   }

			   (*n)++;
		   	   a = a + D[i][0];
		   	   b = b + D[i][1];
			}
		   } // while
		} // for
		break;

	// mvmt REINE ...
	case 'n' :
		for (i=0; i<8; i += 1) {
		   // traitement des 8 directions paires et impaires
		   stop = 0;
		   a = x + D[i][0];
		   b = y + D[i][1];
		   while ( !stop && a >= 0 && a <= 7 && b >= 0 && b <= 7 ) {
			if ( conf.mat[ a ] [ b ] > 0 )  stop = 1;
			else {
			   copier(&conf, &T[*n]);
			   T[*n].mat[x][y] = 0;
			   if ( T[*n].mat[a][b] < 0 ) stop = 1;
			   T[*n].mat[a][b] = 'n';
			   // cas où le roi adverse est pris...
			   if (T[*n].xrN == a && T[*n].yrN == b) { T[*n].xrN = -1; T[*n].yrN = -1; }

			   (*n)++;
		   	   a = a + D[i][0];
		   	   b = b + D[i][1];
			}
		   } // while
		} // for
		break;

	// mvmt ROI ...
	case 'r' :
		// vérifier possibilité de faire un roque ...
		if ( conf.roqueB != 'n' && conf.roqueB != 'e' ) {
		   if ( conf.roqueB != 'g' && conf.mat[0][1] == 0 && conf.mat[0][2] == 0 && conf.mat[0][3] == 0 )
		      if ( !caseMenaceePar( MIN, 0, 1, conf ) && !caseMenaceePar( MIN, 0, 2, conf ) && \
			   !caseMenaceePar( MIN, 0, 3, conf ) && !caseMenaceePar( MIN, 0, 4, conf ) )  {
			// Faire un grand roque ...
			copier(&conf, &T[*n]);
			T[*n].mat[0][4] = 0;
			T[*n].mat[0][0] = 0;
			T[*n].mat[0][2] = 'r'; T[*n].xrB = 0; T[*n].yrB = 2;
			T[*n].mat[0][3] = 't';
			T[*n].roqueB = 'e'; // aucun roque ne sera plus possible à partir de cette config
			(*n)++;
		      }
		   if ( conf.roqueB != 'p' && conf.mat[0][5] == 0 && conf.mat[0][6] == 0 )
		      if ( !caseMenaceePar( MIN, 0, 4, conf ) && !caseMenaceePar( MIN, 0, 5, conf ) && \
			   !caseMenaceePar( MIN, 0, 6, conf ) )  {
			// Faire un petit roque ...
			copier(&conf, &T[*n]);
			T[*n].mat[0][4] = 0;
			T[*n].mat[0][7] = 0;
			T[*n].mat[0][6] = 'r'; T[*n].xrB = 0; T[*n].yrB = 6;
			T[*n].mat[0][5] = 't';
			T[*n].roqueB = 'e'; // aucun roque ne sera plus possible à partir de cette config
			(*n)++;

		      }
		}

		// vérifier les autres mouvements du roi ...
		for (i=0; i<8; i += 1) {
		   // traitement des 8 directions paires et impaires
		   a = x + D[i][0];
		   b = y + D[i][1];
		   if ( a >= 0 && a <= 7 && b >= 0 && b <= 7 )
			if ( conf.mat[a][b] <= 0 ) {
			   copier(&conf, &T[*n]);
			   T[*n].mat[x][y] = 0;
			   T[*n].mat[a][b] = 'r'; T[*n].xrB = a; T[*n].yrB = b;
			   // cas où le roi adverse est pris...
			   if (T[*n].xrN == a && T[*n].yrN == b) { T[*n].xrN = -1; T[*n].yrN = -1; }

			   T[*n].roqueB = 'n'; // aucun roque ne sera plus possible à partir de cette config
			   (*n)++;
			}
		} // for
		break;

	}

} // deplacementsB


/* Génère les successeurs de la configuration conf dans le tableau T,
   retourne aussi dans n le nb de configurations filles générées */
void generer_succ( struct config conf, int mode, struct config T[], int *n )
{
	int i, j, k, stop;

	*n = 0;

	if ( mode == MAX ) {		// mode == MAX
	   for (i=0; i<8; i++)
	      for (j=0; j<8; j++)
		 if ( conf.mat[i][j] > 0 )
		    deplacementsB(conf, i, j, T, n );

	   // vérifier si le roi est en echec, auquel cas on ne garde que les succ évitants l'échec...
	   for (k=0; k < *n; k++) {
	      	i = T[k].xrB; j = T[k].yrB;  // pos du roi B dans T[k]
		// vérifier s'il est menacé dans la config T[k] ...
		if ( caseMenaceePar( MIN, i, j, T[k] ) ) {
		    T[k] = T[(*n)-1];	// alors supprimer T[k] de la liste des succ...
		    (*n)--;
		    k--;
		}
	    } // for k
	}

	else { 				// mode == MIN
	   for (i=0; i<8; i++)
	      for (j=0; j<8; j++)
		 if ( conf.mat[i][j] < 0 )
		    deplacementsN(conf, i, j, T, n );

	   // vérifier si le roi est en echec, auquel cas on ne garde que les succ évitants l'échec...
	   for (k=0; k < *n; k++) {
		i = T[k].xrN; j = T[k].yrN;
		// vérifier s'il est menacé dans la config T[k] ...
		if ( caseMenaceePar( MAX, i, j, T[k] ) ) {
		    T[k] = T[(*n)-1];	// alors supprimer T[k] de la liste des succ...
		    (*n)--;
		    k--;
		}
	   } // for k
	} // if (mode == MAX) ... else ...

} // generer_succ



/***********************************************************************/
/*********** Partie:  AlphaBeta, Initialisation et affichahe ***********/
/***********************************************************************/


/* MinMax avec elagage alpha-beta */
int minmax_ab( struct config conf, int mode, int niv, int alpha, int beta )
{
 	int n, i, score, score2;
 	struct config T[100];

   	if ( feuille(conf, &score) )
		return score;

   	if ( niv == 0 )
		return estim( conf );

   	if ( mode == MAX ) {

	   generer_succ( conf, MAX, T, &n );

	   score = alpha;
	   for ( i=0; i<n; i++ ) {
   	    	score2 = minmax_ab( T[i], MIN, niv-1, score, beta );
		if (score2 > score) score = score2;
		if (score > beta) {
			// Coupe Beta
   	      		return beta;
	    	}
	   }
	}
	else  { // mode == MIN

	   generer_succ( conf, MIN, T, &n );

	   score = beta;
	   for ( i=0; i<n; i++ ) {
   	    	score2 = minmax_ab( T[i], MAX, niv-1, alpha, score );
		if (score2 < score) score = score2;
		if (score < alpha) {
			// Coupe Alpha
   	      		return alpha;
	    	}
	   }
	}

        if ( score == +INFINI ) score = +100;
        if ( score == -INFINI ) score = -100;

	return score;

} // minmax_ab


/* Intialise la disposition des pieces dans la configuration initiale conf */
/*
void init( struct config *conf )
{
   	int i, j;

    	for (i=0; i<8; i++)
		for (j=0; j<8; j++)
			conf->mat[i][j] = 0;	// Les cases vides sont initialisées avec 0

	conf->mat[0][0] =  't'; conf->mat[0][1] =  'c'; conf->mat[0][2] = 'f'; conf->mat[0][3] =  'n';
	conf->mat[0][4] =  'r'; conf->mat[0][5] =  'f'; conf->mat[0][6] = 'c'; conf->mat[0][7] =  't';

	for (j=0; j<8; j++) {
		conf->mat[1][j] = 'p';
 		conf->mat[6][j] = -'p';
		conf->mat[7][j] = -conf->mat[0][j];
	}

	conf->xrB = 0; conf->yrB = 4;
	conf->xrN = 7; conf->yrN = 4;

	conf->roqueB = 'r';
	conf->roqueN = 'r';

	conf->val = 0;

} // init
*/


void init( struct config *conf )
{
   	int i, j;

    	for (i=0; i<8; i++)
		for (j=0; j<8; j++)
			conf->mat[i][j] = 0;	// Les cases vides sont initialisées avec 0

	 conf->mat[0][2] = 'f'; conf->mat[0][3] =  'r';
	conf->mat[0][4] =  'n'; conf->mat[0][5] =  't';

    for (j=0; j<8; j++) {
		conf->mat[7][j] = -conf->mat[0][j];
	}


	conf->xrB = 0; conf->yrB = 3;
	conf->xrN = 7; conf->yrN = 3;

	conf->roqueB = 'r';
	conf->roqueN = 'r';

	conf->val = 0;

}




/* Affiche la configuration conf */
void affich( struct config conf )
{
	int i, j, k;
	for (i=0;  i<8; i++)
		printf("\t  %c", i+'a');
   	printf("\n");

	for (i=0;  i<8; i++)
		printf("\t----- ");
   	printf("\n");

	for(i=8; i>0; i--)  {
		printf("    %d", i);
		for (j=0; j<8; j++)
			if ( conf.mat[i-1][j] < 0 ) printf("\t -%c", -conf.mat[i-1][j]);
			else if ( conf.mat[i-1][j] > 0 ) printf("\t +%c", conf.mat[i-1][j]);
				  else printf("\t  ");
		printf("\n");

		for (k=0;  k<8; k++)
			printf("\t----- ");
   		printf("\n");

	}
	printf("\n");

} // affich





/*******************************************/
/*********** Programme princiapl ***********/
/*******************************************/



int main( int argc, char *argv[] )
{
   char sy, dy, ch[10];
   int sx, dx, n, i, j, score, stop, cout, cout2, legal, hauteur, sauter;
   int cmin, cmax;

   struct config T[100], conf, conf1;

   if ( argc == 1 )
	hauteur = 4;  // par défaut on fixe la profondeur d'évaluation à 4
   else
	hauteur = atoi( argv[1] ); // sinon elle est récupérée depuis la ligne de commande

   printf("\n\nProfondeur d'exploration = %d\n\n", hauteur);

   // Initialise la configuration de départ
   init( &conf );

   printf("\n\nVous êtes les + (Blancs) et je suis les - (Noirs)\n\n");

   // Boucle principale du dérouleùment d'une partie ...
   stop = 0;
   while ( !stop ) {

	affich( conf );

	// récupérer le coup du joueur ...
	printf("Coup (sy sx dy dx : depl normal / 0 0 0 0 : pt roq / 1 1 1 1 : grd roq / 2 niv 2 2 : estim profonde ) : ");
	scanf(" %c %d %c %d", &sy, &sx, &dy, &dx );

	copier(&conf, &conf1);

	// Traitement du coup du joueur ...
	sauter = 0;
	if (sy == '0') { // petit roque ...
	   conf1.mat[0][4] = 0;
	   conf1.mat[0][7] = 0;
	   conf1.mat[0][6] = 'r'; conf1.xrB = 0; conf1.yrB = 6;
	   conf1.mat[0][5] = 't';
	   conf1.roqueB = 'e';
	}
	else
	   if (sy == '1') {  // grand roque ...
		conf1.mat[0][4] = 0;
		conf1.mat[0][0] = 0;
		conf1.mat[0][2] = 'r'; conf1.xrB = 0; conf1.yrB = 2;
		conf1.mat[0][3] = 't';
		conf1.roqueB = 'e';
	   }
	   else
	      if ( sy == '2' ) { // Estimation à la profondeur spécifiée dans sx
		  cout = minmax_ab( conf, MAX, sx, -INFINI, +INFINI );
		  printf("Estimation a %d niveaux = %d\n", sx, cout);
		  sauter = 1;
	      }
	      else
		{  // deplacement normal (les autres coups) ...
		conf1.mat[dx-1][dy-'a'] = conf1.mat[sx-1][sy-'a'];
		conf1.mat[sx-1][sy-'a'] = 0;
		// vérifier possibilité de transformation d'un pion arrivé en fin d'échiquier ...
		if (dx == 8 && conf1.mat[dx-1][dy-'a'] == 'p') {
		   printf("Pion arrivé en ligne 8, transformer en (p/c/f/t/n) : ");
		   scanf(" %s", ch);
		   switch (ch[0]) {
			case 'c' : conf1.mat[dx-1][dy-'a'] = 'c'; break;
			case 'f' : conf1.mat[dx-1][dy-'a'] = 'f'; break;
			case 't' : conf1.mat[dx-1][dy-'a'] = 't'; break;
			case 'p' : conf1.mat[dx-1][dy-'a'] = 'p'; break;
			default  : conf1.mat[dx-1][dy-'a'] = 'n';
		   }
		}
		// vérifier si victoire (le roi N n'existe plus) ...
		if ( conf1.xrN == dx-1 && conf1.yrN == dy-'a' ) {
			conf1.xrN = -1;
			conf1.yrN = -1;
		}
	   }

	if ( sauter == 0) {

	   // vérification de la légalité du coup effectué par le joueur ...
    	   generer_succ(  conf, MAX, T, &n );

	   legal = 0;
	   for (i=0; i<n && !legal; i++)
	    	if ( egal(T[i].mat, conf1.mat) )  legal = 1;

	   if ( legal && !feuille(conf1,&cout) ) {
	    	printf("OK\n\n");
	    	i--;
	    	copier( &T[i], &conf );
	    	affich( conf );

		// L'ordinateur joue son coup ...
	    	printf("A mon tour maintenant ...\n");

	    	generer_succ(  conf, MIN, T, &n );

	    	score = +INFINI;
	    	j = -1;

	    	for (i=0; i<n; i++) {
		   cout = minmax_ab( T[i], MAX, hauteur, -INFINI, +INFINI );
		   if ( cout < score ) {  // Choisir le meilleur coup (c-a-d le plus petit score)
		   	score = cout;
		   	j = i;
		   }
	    	}
	    	if ( j != -1 ) { // jouer le coup et aller à la prochaine itération ...
	    	   copier( &T[j], &conf );
		   conf.val = score;
	    	}
	    	else { // S'il n'y a pas de successeur possible, l'ordinateur à perdu
		   printf(" *** J'ai perdu ***\n");
		   stop = 1;
	    	}
	   }
	   else
	    	if ( !legal )
	   	   printf("Coup illégal -- réessayer\n");
	    	else
		   stop = 1;
	}  // if (sauter == 0)
   } // while

}



/*

int mobility_pion(struct config conf, int x , int y,int BN)
{ int cpt=0 ;

  switch(BN) {

  case 1 :

    if ( x <7 && conf.mat[x+1][y] == 0 ) {				// avance d'une case
			cpt++;
		}
    if ( x == 1 && conf.mat[2][y] == 0 && conf.mat[3][y] == 0) {	// avance de 2 cases
			cpt++;
		}
    if ( x < 7 && y > 0 && conf.mat[x+1][y-1] < 0 ) {		// mange à gauche (en montant)
        cpt++;
        }
    if ( x < 7 && y < 7 && conf.mat[x+1][y+1] < 0 ) {		// mange à droite (en montant)
		cpt++;
        }

   break;

  case -1 :

   if ( x > 0 && conf.mat[x-1][y] == 0 ) {				// avance d'une case
     cpt++;
      }
    if ( x == 6 && conf.mat[5][y] == 0 && conf.mat[4][y] == 0) {	// avance de 2 cases
     cpt++;
		}
    if ( x > 0 && y >0 && conf.mat[x-1][y-1] > 0 ) {		// mange à droite (en descendant)
        cpt++;
        }
    if ( x > 0 && y < 7 && conf.mat[x-1][y+1] > 0 ) {		// mange à gauche (en descendant)
        cpt++;
		}

  break;

}

 return cpt;
}






int mobilityB(struct config conf, int x, int y)
{
	int i , j,stop,cpt=0,a,b;

	switch(conf.mat[x][y]) {
	// mvmt PION ...
	case 'p' :
		if ( x <7 && conf.mat[x+1][y] == 0 ) {				// avance d'une case
			cpt++;
			}
		if ( x == 1 && conf.mat[2][y] == 0 && conf.mat[3][y] == 0) {	// avance de 2 cases
			cpt++;
		}
		if ( x < 7 && y > 0 && conf.mat[x+1][y-1] < 0 ) {		// mange à gauche (en montant)
			cpt++;

		}
		if ( x < 7 && y < 7 && conf.mat[x+1][y+1] < 0 ) {		// mange à droite (en montant)
			cpt++;
		}
		break;

	// mvmt CAVALIER ...
	case 'c' :
		for (i=0; i<8; i++)
		   if ( x+dC[i][0] <= 7 && x+dC[i][0] >= 0 && y+dC[i][1] <= 7 && y+dC[i][1] >= 0 )
			if ( conf.mat[ x+dC[i][0] ] [ y+dC[i][1] ] <= 0 )  {
			   cpt++;
			}
		break;

	// mvmt FOU ...
	case 'f' :
		for (i=1; i<8; i += 2) {
		   // traitement des directions impaires (1, 3, 5 et 7)
		   stop = 0;
		   a = x + D[i][0];
		   b = y + D[i][1];
		   while ( !stop && a >= 0 && a <= 7 && b >= 0 && b <= 7 ) {
			if ( conf.mat[ a ] [ b ] > 0 )  stop = 1;
			else {
			   cpt++;
		   	   a = a + D[i][0];
		   	   b = b + D[i][1];
			}
		   } // while
		} // for
		break;

	// mvmt TOUR ...
	case 't' :
		for (i=0; i<8; i += 2) {
		   // traitement des directions paires (0, 2, 4 et 6)
		   stop = 0;
		   a = x + D[i][0];
		   b = y + D[i][1];
		   while ( !stop && a >= 0 && a <= 7 && b >= 0 && b <= 7 ) {
			if ( conf.mat[ a ] [ b ] > 0 )  stop = 1;
			else {
			   cpt++;
		   	   a = a + D[i][0];
		   	   b = b + D[i][1];
			}
		   } // while
		} // for
		break;

	// mvmt REINE ...
	case 'n' :
		for (i=0; i<8; i += 1) {
		   // traitement des 8 directions paires et impaires
		   stop = 0;
		   a = x + D[i][0];
		   b = y + D[i][1];
		   while ( !stop && a >= 0 && a <= 7 && b >= 0 && b <= 7 ) {
			if ( conf.mat[ a ] [ b ] > 0 )  stop = 1;
			else {
			   cpt++;
		   	   a = a + D[i][0];
		   	   b = b + D[i][1];
			}
		   } // while
		} // for
		break;

	// mvmt ROI ...
	case 'r' :
		// vérifier possibilité de faire un roque ...
		if ( conf.roqueB != 'n' && conf.roqueB != 'e' ) {
		   if ( conf.roqueB != 'g' && conf.mat[0][1] == 0 && conf.mat[0][2] == 0 && conf.mat[0][3] == 0 )
		      if ( !caseMenaceePar( MIN, 0, 1, conf ) && !caseMenaceePar( MIN, 0, 2, conf ) && \
			   !caseMenaceePar( MIN, 0, 3, conf ) && !caseMenaceePar( MIN, 0, 4, conf ) )  {
			// Faire un grand roque ...
			cpt++;
		      }
		   if ( conf.roqueB != 'p' && conf.mat[0][5] == 0 && conf.mat[0][6] == 0 )
		      if ( !caseMenaceePar( MIN, 0, 4, conf ) && !caseMenaceePar( MIN, 0, 5, conf ) && \
			   !caseMenaceePar( MIN, 0, 6, conf ) )  {
			// Faire un petit roque ...
			cpt++;

		      }
		}

		// vérifier les autres mouvements du roi ...
		for (i=0; i<8; i += 1) {
		   // traitement des 8 directions paires et impaires
		   a = x + D[i][0];
		   b = y + D[i][1];
		   if ( a >= 0 && a <= 7 && b >= 0 && b <= 7 )
			if ( conf.mat[a][b] <= 0 ) {
			   cpt++;
			}
		} // for
		break;

	}

	return cpt;

} // deplacementsB


*/












