#ifndef PLATEAU_H_INCLUDED
#define PLATEAU_H_INCLUDED

#include "jouer.h"

//! Les pièces sont définis par les 4 bits de poids faible
#define VIDE		0x00	
#define PION		0x01	
#define TOUR		0x02	
#define CAVALIER  	0x03	
#define FOU  		0x04	
#define DAME   		0x05	
#define ROI			0x06	

//! Extrait le type de la piece
#define TYPE_PIECE(x) (0x0F & x)

//! Quelques macros de verification 
#define SET_NOIR(x) (x | 0x10)
#define EST_NOIR(x)  (0x10 & x)

#define SET_DEPLACE(x) (x | 0x20)
#define EST_DEPLACE(x)  (0x20 & x)

//! Pour les pions se retrouvant dans la situation d'une prise en passant
#define SET_PASSANT(x)   (x | 0x40)
#define SUPR_PASSANT(x) (x & 0xbf)
#define EST_PASSANT(x)    (0x40 & x)

//! Pour la promotion des pions
#define SET_PROMOTION(x)   (x | 0x80)
#define EST_PROMOTION(x)    (0x80 & x)
#define DEL_PROMOTED(x) (x & 0x7f)

//! Constantes a comparé avec les macro : definition des couleurs
#define BLANC 0x00
#define NOIR 0x10
#define CHANGE_COULEUR(x) (0x10 ^ x)

struct Deplacement{
	
	//! Methode qui affiche le déplacement effectué au dessus du plateau
	void affichage(void) const;
	
	//! Renvoi vrai si deux deplacements sont identiques
	bool operator==(const Deplacement & b) const;

	char typePiece;	//! Type de la pièce qui est déplacé typePiece which is moved
	char origine, destination;	//! Coordonnées sur le plateau
	char capture;	//! Piece qui est sur la case destination
};

struct Plateau{
	enum Position {
		A1 = 0, B1, C1, D1, E1, F1, G1, H1,
		A2, B2, C2, D2, E2, F2, G2, H2,
		A3, B3, C3, D3, E3, F3, G3, H3,
		A4, B4, C4, D4, E4, F4, G4, H4,
		A5, B5, C5, D5, E5, F5, G5, H5,
		A6, B6, C6, D6, E6, F6, G6, H6,
		A7, B7, C7, D7, E7, F7, G7, H7,
		A8, B8, C8, D8, E8, F8, G8, H8
	};

	Plateau();

	//! Affichage ASCII du plateau
	void affichage(void) const;

	//! Permet de retourné une representation ASCII de chaque type de pièce
	char typePieceEnASCII(int typePiece) const;

	//! Initialise le plateau
	void initialisationPlateau(void);

	//! Genere tous les mouvement pour chaque coté (noir ou blanc)
	void getDeplacements(int couleur, std::list<Deplacement> & moves,
						std::list<Deplacement> & captures, std::list<Deplacement> & deplacementsNull);

	//! Tous les déplacements possibles pour un PION
	void getDeplacementsPions(int typePiece, int position, std::list<Deplacement> & moves,
						std::list<Deplacement> & captures, std::list<Deplacement> & deplacementsNull) const;
	
	//! Tous les déplacements possibles pour une TOUR
	void getDeplacementsTours(int typePiece, int position, std::list<Deplacement> & moves,
						std::list<Deplacement> & captures) const;

	//! Tous les déplacements possibles pour un CAVALIER
	void getDeplacementsCavaliers(int typePiece, int position, std::list<Deplacement> & moves,
						std::list<Deplacement> & captures) const;
	
	//! Tous les déplacements possibles pour un FOU
	void getDeplacementsFous(int typePiece, int position, std::list<Deplacement> & moves,
		std::list<Deplacement> & captures) const;
	
	//! Tous les déplacements possibles pour une DAME
	void getDeplacementsDames(int typePiece, int position, std::list<Deplacement> & moves,
		std::list<Deplacement> & captures) const;

	//! Tous les déplacements possibles pour un ROI
	void getDeplacementsRois(int typePiece, int position, std::list<Deplacement> & moves,
		std::list<Deplacement> & captures);

	//! Retourne vrai si la position est vulnérable
	//! Soit le roque est legal soit le roi est en echec. En passant n'est pas ici pris en compte.
	bool estVulnerable(int position, int couleur) const;

	//! Vrai si deplacement est un deplacement valable pour un joueru d'une couleur donnée. 
	//! Un deplacement qui met le propre roi du joueur en échec, est également considérée comme invalide.
	bool estDeplacementValide(int couleur, Deplacement & deplacement);

	//! Retourne le statut de joueur de couleur donnée. 
	//! Cette méthode n'est pas déclarée const : 
	//! On a besoin de la destination simulé se déplace sur la destination du plateau avant de décidé d'un statut.
	JoueurEchec::Status getJoueurStatus(int couleur);

	//! Methode de déplacement et d'annulation de déplacements
	void deplacement(const Deplacement & deplacement);
	void annulerDeplacement(const Deplacement & deplacement);

	void deplacementPion(const Deplacement & deplacement);
	void annulerDeplacementPion(const Deplacement & deplacement);

	void deplacementRoi(const Deplacement & deplacement);
	void annulerDeplacementRoi(const Deplacement & deplacement);

	//! Definition du plateau et de sa taille, ici fixe TO DO: rendre variable
	char plateau88[8*8];

	//! Toujours garder une trace de la position du roi sur le plateau
	char roi_noir_position;
	char roi_blanc_position;
};

#endif
