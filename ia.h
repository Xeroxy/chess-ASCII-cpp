#ifndef IA_H_INCLUDED
#define IA_H_INCLUDED

#include "jouer.h"

//! Valeur des pièces : https://chessprogramming.wikispaces.com/Encoding+Moves

#define VICTOIRE_VALEUR  50000	//! Gagne le jeu
#define PION_VALEUR    30		//! 8x
#define TOUR_VALEUR    90		//! 2x
#define CAVALIER_VALEUR  85		//! 2x
#define FOU_VALEUR  84			//! 2x
#define DAME_VALEUR  300		//! 1x
#define ROI_VALEUR 	 ((PION_VALEUR * 8) + (TOUR_VALEUR * 2) \
					+ (CAVALIER_VALEUR * 2) + (FOU_VALEUR * 2) + DAME_VALEUR + VICTOIRE_VALEUR)

class Plateau;

class IAJoueur: public JoueurEchec {

	public:
	
		IAJoueur(int couleur, int profondeurRecherche); // Constructeur

		~IAJoueur();

		//! Get le prochain déplacement
		bool getDeplacement(Plateau & plateau, Deplacement & deplacement) const;

		//! Recheche par MinMax/AlphaBeta la meilleur issue de déplacement possible
		int evaluationAlphaBeta(Plateau & plateau, int couleur, int depth, int alpha, int beta, bool checkBool) const;

		//! Boucle sur chaque case du plateau pour l'évaluer
		int evaluationPlateau(const Plateau & plateau) const;
	
	protected:

		//! Profondeur de recherche : parametre de l'évaluation AlphaBeta
		int profondeurRecherche;
};

#endif
