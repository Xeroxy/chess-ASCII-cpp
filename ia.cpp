#include <cstdlib>
#include <ctime>
#include <list>
#include <vector>
#include "ia.h"
#include "plateau.h"

using namespace std;

IAJoueur::IAJoueur(int couleur, int profondeurRecherche)
 : JoueurEchec(couleur),
   profondeurRecherche(profondeurRecherche)
{
	srand(time(NULL));
}

IAJoueur::~IAJoueur()
{}

bool IAJoueur::getDeplacement(Plateau & plateau, Deplacement & deplacement) const
{
	list<Deplacement> reguliers, nulls;
	vector<Deplacement> deplacementsCandidats;
    bool checkBool = false;
	int meilleur, tmp;

	//! Supposons que nous perdons
	meilleur = -ROI_VALEUR;

	//! On get tous les déplacements
	plateau.getDeplacements(this->couleur, reguliers, reguliers, nulls);

	//! On execute les mouvement dit de maintenance
	for(list<Deplacement>::iterator i = nulls.begin(); i != nulls.end(); ++i){
		plateau.deplacement(*i);
	}

	//! On boucle sur tous les déplacements reguliers possibles
	for(list<Deplacement>::iterator i = reguliers.begin(); i != reguliers.end(); ++i){
		//! On execute chaque deplacement
		plateau.deplacement(*i);

		//! On check si le roi est vulnérable
		if(!plateau.estVulnerable((this->couleur ? plateau.roi_noir_position : plateau.roi_blanc_position), this->couleur)) {

			if((*i).capture != VIDE) {
				checkBool = true;
			}

			//! Evaluation AlphaBeta récursive
			tmp = -evaluationAlphaBeta(plateau, CHANGE_COULEUR(this->couleur), this->profondeurRecherche - 1, -VICTOIRE_VALEUR, -meilleur, checkBool);
			if(tmp > meilleur) {
				meilleur = tmp;
				deplacementsCandidats.clear();
				deplacementsCandidats.push_back(*i);
			}
			else if(tmp == meilleur) {
				deplacementsCandidats.push_back(*i);
			}
		}

		//! Annule le déplacement et increpmente la variable d'itération i 
		plateau.annulerDeplacement(*i);
	}

	//! Annule les mouvements dit de maintenance/temporaires
	for(list<Deplacement>::iterator i = nulls.begin(); i != nulls.end(); ++i){
		plateau.annulerDeplacement(*i);
	}

	//! Verifie si on perd le jeu
	if(meilleur < -VICTOIRE_VALEUR) {
		return false;
	}
	else {
		//! Sinon on sélectione aléatoirement un mouvement parmis les déplacements candidats trouvés
		deplacement = deplacementsCandidats[rand() % deplacementsCandidats.size()];
		return true;
	}
}

int IAJoueur::evaluationAlphaBeta(Plateau & plateau, int couleur, int profondeurRecherche, int alpha, int beta, bool checkBool) const
{
	list<Deplacement> reguliers, nulls;
	int meilleur, tmp;

	if(profondeurRecherche <= 0 && !checkBool) {
		if(couleur)
			return -evaluationPlateau(plateau);
		else
			return +evaluationPlateau(plateau);
	}

	//! Supposons que nous perdons
	meilleur = -VICTOIRE_VALEUR;

	//! On get tous les déplacements
	plateau.getDeplacements(couleur, reguliers, reguliers, nulls);
	
	//! On execute les mouvement dit de maintenance
	for(list<Deplacement>::iterator i = nulls.begin(); i != nulls.end(); ++i){
		plateau.deplacement(*i);
	}
	
	//! On boucle sur tous les déplacements reguliers possibles
	for(list<Deplacement>::iterator i = reguliers.begin(); alpha <= beta && i != reguliers.end(); ++i){
		//! On execute chaque deplacement
		plateau.deplacement(*i);

		//! On check si le roi est vulnérable
		if(!plateau.estVulnerable((couleur ? plateau.roi_noir_position : plateau.roi_blanc_position), couleur)) {

			if((*i).capture == VIDE)
				checkBool = false;
            else
                checkBool = true;

			//! Appel recursif et élagage
			tmp = -evaluationAlphaBeta(plateau, CHANGE_COULEUR(couleur), profondeurRecherche - 1, -beta, -alpha, checkBool);
			if(tmp > meilleur) {
				meilleur = tmp;
				if(tmp > alpha) {
					alpha = tmp;
				}
			}
		}
		//! Annule le déplacement et increpmente la variable d'itération i 
		plateau.annulerDeplacement(*i);
	}
	
	//! Annule les mouvements dit de maintenance/temporaires
	for(list<Deplacement>::iterator i = nulls.begin(); i != nulls.end(); ++i){
		plateau.annulerDeplacement(*i);
	}
	//! On retourne le meilleur résultat
	return meilleur; 
}

int IAJoueur::evaluationPlateau(const Plateau & plateau) const
{
	int typePiece, position, somme = 0, tmp;

	for(position = 0; position < 64; position++)
	{
		typePiece = plateau.plateau88[position];
		switch(TYPE_PIECE(typePiece))
		{
			case PION:
				tmp = PION_VALEUR;
				break;
			case TOUR:
				tmp = TOUR_VALEUR;
				break;
			case CAVALIER:
				tmp = CAVALIER_VALEUR;
				break;
			case FOU:
				tmp = FOU_VALEUR;
				break;
			case DAME:
				tmp = DAME_VALEUR;
				break;
			case ROI:
				tmp = ROI_VALEUR;
				break;
			default:
				tmp = 0;
				break;
		}
		somme += EST_NOIR(typePiece) ? -tmp : tmp;
	}
	
	return somme;
}

