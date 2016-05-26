/**
 * \file jouer.cpp
 * \brief Point d'entré du programme.
 * \author m1_76
 * \version 1
 *
 * Jeux d'echec en C++ avec algo alpha/beta.
 *
 */
#include <cstdlib>
#include <cstdio>
#include <list>

#include "plateau.h"
#include "ia.h"

using namespace std;

int main(void) {
	//! Déclaration des variables utiles au main
	Plateau plateau; //! Creation d'un plateau
	list<Deplacement> regulier, nulls; //! Liste temporaires de déplacements
	int tour = BLANC; //! Le premier tour sera effectué par les blancs : 0x00
	Deplacement deplacement; //! Stoque le déplacement en cours
	bool deplacementBool;

	//! Initialisation des deux joueurs IA
	IAJoueur noir(NOIR, 3);
	IAJoueur blanc(BLANC, 1);

	//! Iniialisation du plateau par defaut en le remplissant
	plateau.initialisationPlateau();

	printf("###################################################\n");
	printf("Jeux d'echec, computer vs. computeur. - m1_76\n"
			"Basé sur un algorithme d'elagage alpha-beta.\n"
			"(optimisation de l'algorithme MinMax)\n"
			"Profondeur de recherche (modifiable en dur) :\n"
			"--- NOIR : 4\n"
			"--- BLANC : 1\n"
			"ATTENTION : Etre patient, certains déplacements peuvent etre longs !\n");
	printf("###################################################\n");
	printf("##### Appuyez sur une touche pour commencer ! #####\n");
	getchar();
	for(;;) {

		//! Affichage du plateau
		plateau.affichage();

		//! Tour par tour
		if(tour)
			deplacementBool = noir.getDeplacement(plateau, deplacement);
		else
			deplacementBool = blanc.getDeplacement(plateau, deplacement);

		if(!deplacementBool) //! Si aucun déplacement n'est possible:deplacementBool=0
			break;

		//! Si un déplacement est trouvé alors on le get
		regulier.clear();
		nulls.clear();
		plateau.getDeplacements(tour, regulier, regulier, nulls);

		//Execution des mouvements nulls di de maintenance
		for(list<Deplacement>::iterator i=nulls.begin(); i!=nulls.end(); ++i){ //! On parcours la liste de déplacement nulls
			plateau.deplacement(*i);
		}

		//! On vient executer le déplacement sur le plateau
		plateau.deplacement(deplacement);

		//! On affiche le déplacement au dessu du plateau
		deplacement.affichage();

		//! On vient passer le tour à l'adverssair : 0x10 ^ tour
		tour = CHANGE_COULEUR(tour);
	}

	//! Get le Status du tour pour voir si il y a echec ou pat
	JoueurEchec::Status status = plateau.getJoueurStatus(tour);

	switch(status) //! En fonction du status du joueur en cour
	{
		case JoueurEchec::EchecMat:
			printf("EchecMat -- Les Noirs ont gagné\n");
			break;
		case JoueurEchec::Pat:
			printf("Pat -- Aucun vainqueur\n");
			break;
	}
}
