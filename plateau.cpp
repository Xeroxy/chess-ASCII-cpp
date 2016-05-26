#include <cstdio>
#include <cstring>
#include <list>
#include "plateau.h"
#include "jouer.h"

using namespace std;

void Deplacement::affichage(void) const {

	const char * nomCases[] = {
		"A1", "B1", "C1", "D1", "E1", "F1", "G1", "H1",
		"A2", "B2", "C2", "D2", "E2", "F2", "G2", "H2",
		"A3", "B3", "C3", "D3", "E3", "F3", "G3", "H3",
		"A4", "B4", "C4", "D4", "E4", "F4", "G4", "H4",
		"A5", "B5", "C5", "D5", "E5", "F5", "G5", "H5",
		"A6", "B6", "C6", "D6", "E6", "F6", "G6", "H6",
		"A7", "B7", "C7", "D7", "E7", "F7", "G7", "H7",
		"A8", "B8", "C8", "D8", "E8", "F8", "G8", "H8"
	};		

	switch(TYPE_PIECE(typePiece)) {
		case PION:
			printf("Le pion ");
			break;
		case TOUR:
			printf("La tour ");
			break;
		case CAVALIER:
			printf("Le cavalier ");
			break;
		case FOU:
			printf("Le fou ");
			break;
		case DAME:
			printf("La dame ");
			break;
		case ROI:
			printf("Le roi ");
			break;
	}

	if(EST_NOIR(typePiece))
		printf("noirs ");
	else 
		printf("blanc ");

	printf("de %s à %s:\n", nomCases[(int)origine], nomCases[(int)destination]);
}

bool Deplacement::operator==(const Deplacement & b) const
{
	if(origine != b.origine)
		return false;
	if(destination != b.destination)
		return false;
	if(capture != b.capture)
		return false;
	if(typePiece != b.typePiece)
		return false;
		
	return true;
}


Plateau::Plateau()
{
	memset((void*)plateau88, VIDE, sizeof(plateau88));
}

void Plateau::affichage(void) const{

	char typePiece;
	int representation, fixe, passant, ligne, colone;

	printf("   ___ ___ ___ ___ ___ ___ ___ ___ \n  ");

	for(ligne = 7; ligne >= 0; ligne--)
	{
		for(colone = 0; colone < 8; colone++)
		{
			typePiece = this->plateau88[ligne*8+colone];
			representation = typePieceEnASCII(typePiece);
			fixe = (EST_DEPLACE(typePiece) || (typePiece == VIDE)) ? ' ' : '.';
			passant = EST_PASSANT(typePiece) ? '`' : ' ';
			printf("|%c%c%c", fixe, representation, passant);
		}
		printf("|\n%d |___|___|___|___|___|___|___|___|\n  ", ligne + 1);
	}
	printf("  A   B   C   D   E   F   G   H  \n\n");
}

char Plateau::typePieceEnASCII(int typePiece) const{ //! Lié les pièces a leur caractères d'affichage
	switch(TYPE_PIECE(typePiece))
	{
		case PION:
			if(EST_NOIR(typePiece))
				return 'P';
			else
				return 'p';
		case TOUR:
			if(EST_NOIR(typePiece))
				return 'T';
			else
				return 't';
		case CAVALIER:
			if(EST_NOIR(typePiece))
				return 'C';
			else
				return 'c';
		case FOU:
			if(EST_NOIR(typePiece))
				return 'F';
			else
				return 'f';
		case DAME:
			if(EST_NOIR(typePiece))
				return 'D';
			else
				return 'd';
		case ROI:
			if(EST_NOIR(typePiece))
				return 'R';
			else
				return 'r';
	}
	
	return ' ';
}

void Plateau::initialisationPlateau(void){
	//! On vide le plateau 
	memset((void*)plateau88, VIDE, sizeof(plateau88));

	//! Mise en place de l'aristocratie des BLANCS
	plateau88[A1] = TOUR; plateau88[B1] = CAVALIER; plateau88[C1] = FOU; plateau88[D1] = DAME;
	plateau88[E1] = ROI; plateau88[F1] = FOU; plateau88[G1] = CAVALIER; plateau88[H1] = TOUR;
	
	//! Mise en place de l'aristocratie des NOIRS
	plateau88[A8] = SET_NOIR(TOUR); plateau88[B8] = SET_NOIR(CAVALIER);
	plateau88[C8] = SET_NOIR(FOU); plateau88[D8] = SET_NOIR(DAME);
	plateau88[E8] = SET_NOIR(ROI); plateau88[F8] = SET_NOIR(FOU);
	plateau88[G8] = SET_NOIR(CAVALIER); plateau88[H8] = SET_NOIR(TOUR);
	
	//! Mise en place des pions BLANCS
	plateau88[A2] = plateau88[B2] = plateau88[C2] = plateau88[D2] =
	plateau88[E2] = plateau88[F2] = plateau88[G2] = plateau88[H2] = PION;
		
	//! Mise en place des pions NOIRS
	plateau88[A7] = plateau88[B7] = plateau88[C7] = plateau88[D7] =
	plateau88[E7] = plateau88[F7] = plateau88[G7] = plateau88[H7] = SET_NOIR(PION);

	//! Mise à jours des coordonnées des ROIS
	roi_noir_position = E8;
	roi_blanc_position = E1;
}
//! Effectue les déplacements sur le plateau de n'impote quel type de pièce
void Plateau::getDeplacements(int couleur, list<Deplacement> & moves, list<Deplacement> & captures, list<Deplacement> & deplacementsNull){
	int position, typePiece;
	
	for(position = 0; position < 64; position++)
	{
		if((typePiece = this->plateau88[position]) != VIDE)
		{
			if(EST_NOIR(typePiece) == couleur)
			{
				switch(TYPE_PIECE(typePiece))
				{
					case PION:
						getDeplacementsPions(typePiece, position, moves, captures, deplacementsNull);
						break;
					case TOUR:
						getDeplacementsTours(typePiece, position, moves, captures);
						break;
					case CAVALIER:
						getDeplacementsCavaliers(typePiece, position, moves, captures);
						break;
					case FOU:
						getDeplacementsFous(typePiece, position, moves, captures);
						break;
					case DAME:
						getDeplacementsDames(typePiece, position, moves, captures);
						break;
					case ROI:
						getDeplacementsRois(typePiece, position, moves, captures);
						break;
					default:
						break;
				}
			}
		}
	}
}

void Plateau::getDeplacementsPions(int typePiece, int position, list<Deplacement> & moves, list<Deplacement> & captures, list<Deplacement>  & deplacementsNull) const{
	Deplacement nouveauDeplacement;
	int positionCible, typePieceCible;

	//! Si un pion a précedement été en passant, il ne l'est plus
	//! Il s'agit d'un déplacement nul
	if(EST_PASSANT(typePiece))
	{
		nouveauDeplacement.typePiece = SUPR_PASSANT(typePiece);
		nouveauDeplacement.origine = position;
		nouveauDeplacement.destination = position;
		nouveauDeplacement.capture = typePiece;
		deplacementsNull.push_back(nouveauDeplacement);
		
		typePiece = SUPR_PASSANT(typePiece);
	}

	//! On set une fois le type de la piece et la position d'origine
	nouveauDeplacement.typePiece = typePiece;
	nouveauDeplacement.origine = position;

	//! ICI LES DIFFERENTS DEPLACEMENTS POSSIBLES
	//! 1. Une case en avant
	positionCible = EST_NOIR(typePiece) ? position - 8 : position + 8;
	if((positionCible >= 0) && (positionCible < 64))
	{
		if((typePieceCible = this->plateau88[positionCible]) == VIDE)
		{
			nouveauDeplacement.destination = positionCible;
			nouveauDeplacement.capture = typePieceCible;
			moves.push_back(nouveauDeplacement);
			
			//! 2. Deux case en avant si le pions n'a pas encore bougé
			if(!EST_DEPLACE(typePiece))
			{
				positionCible = EST_NOIR(typePiece) ? position - 16 : position + 16;
				if((positionCible >= 0) && (positionCible < 64))
				{
					if((typePieceCible = this->plateau88[positionCible]) == VIDE)
					{
						nouveauDeplacement.destination = positionCible;
						nouveauDeplacement.capture = typePieceCible;

						//! Active l'attribut en passant. on le clear plus tard
						nouveauDeplacement.typePiece = SET_PASSANT(typePiece);
						moves.push_back(nouveauDeplacement);
						nouveauDeplacement.typePiece = typePiece;
					}
				}
			} // END 2.
		}
	} // END 1.
	
	//! 3. Capture en avant (BLANC gauche; NOIR droite)
	if(position % 8 != 0)
	{
		positionCible = EST_NOIR(typePiece) ? position - 9 : position + 7;
		if((positionCible >= 0) && (positionCible < 64))
		{
			if((typePieceCible = this->plateau88[positionCible]) != VIDE)
			{
				if(EST_NOIR(typePieceCible) != EST_NOIR(typePiece))
				{
					nouveauDeplacement.destination = positionCible;
					nouveauDeplacement.capture = typePieceCible;
					captures.push_back(nouveauDeplacement);
				}
			}
			else
			{
				// En passant?
				typePieceCible = this->plateau88[position - 1];
				if(EST_PASSANT(typePieceCible))
				{
					if(EST_NOIR(typePieceCible) != EST_NOIR(typePiece))
					{
						nouveauDeplacement.destination = positionCible;
						nouveauDeplacement.capture = typePieceCible;
						captures.push_back(nouveauDeplacement);
					}				
				}
			}
		}
	}
	
	//! 4. Capture en avant (BLANC droite; NOIR gauche)
	if(position % 8 != 7)
	{
		positionCible = EST_NOIR(typePiece) ? position - 7 : position + 9;
		if((positionCible >= 0) && (positionCible < 64))
		{
			if((typePieceCible = this->plateau88[positionCible]) != VIDE)
			{
				if(EST_NOIR(typePieceCible) != EST_NOIR(typePiece))
				{
					nouveauDeplacement.destination = positionCible;
					nouveauDeplacement.capture = typePieceCible;
					captures.push_back(nouveauDeplacement);
				}
			}
			else
			{
				// En passant?
				typePieceCible = this->plateau88[position + 1];
				if(EST_PASSANT(typePieceCible))
				{
					if(EST_NOIR(typePieceCible) != EST_NOIR(typePiece))
					{
						nouveauDeplacement.destination = positionCible;
						nouveauDeplacement.capture = typePieceCible;
						captures.push_back(nouveauDeplacement);
					}				
				}
			}
		}
	}	
}

void Plateau::getDeplacementsTours(int typePiece, int position, list<Deplacement> & moves, list<Deplacement> & captures) const
{
	Deplacement nouveauDeplacement;
	int positionCible, typePieceCible, end;

	//! On set une fois le type de la piece et la position d'origine
	nouveauDeplacement.typePiece = typePiece;
	nouveauDeplacement.origine = position;

	//! ICI LES DIFFERENTS DEPLACEMENTS POSSIBLES
	//! 1. Haut
	for(positionCible = position + 8; positionCible < 64; positionCible += 8) //! pour la position actuel +8 juqu'a 64 on fait varier de 8
	{
		if((typePieceCible = this->plateau88[positionCible]) != VIDE)
		{
			if(EST_NOIR(typePiece) != EST_NOIR(typePieceCible))
			{
				nouveauDeplacement.destination = positionCible;
				nouveauDeplacement.capture = typePieceCible;
				captures.push_back(nouveauDeplacement);
			}
			
			break;
		}
		else
		{
			nouveauDeplacement.destination = positionCible;
			nouveauDeplacement.capture = typePieceCible;
			moves.push_back(nouveauDeplacement);
		}
	}

	//! 2. Bas
	for(positionCible = position - 8; positionCible >= 0; positionCible -= 8)
	{	
		if((typePieceCible = this->plateau88[positionCible]) != VIDE)
		{
			if(EST_NOIR(typePiece) != EST_NOIR(typePieceCible))
			{
				nouveauDeplacement.destination = positionCible;
				nouveauDeplacement.capture = typePieceCible;
				captures.push_back(nouveauDeplacement);
			}
			
			break;
		}
		else
		{
			nouveauDeplacement.destination = positionCible;
			nouveauDeplacement.capture = typePieceCible;
			moves.push_back(nouveauDeplacement);
		}
	}

	// 3. Gauche
	for(positionCible = position - 1, end = position - (position % 8); positionCible >= end; positionCible--)
	{
		if((typePieceCible = this->plateau88[positionCible]) != VIDE)
		{
			if(EST_NOIR(typePiece) != EST_NOIR(typePieceCible))
			{
				nouveauDeplacement.destination = positionCible;
				nouveauDeplacement.capture = typePieceCible;
				captures.push_back(nouveauDeplacement);
			}
			
			break;
		}
		else
		{
			nouveauDeplacement.destination = positionCible;
			nouveauDeplacement.capture = typePieceCible;
			moves.push_back(nouveauDeplacement);
		}
	}

	// 4. Droite
	for(positionCible = position + 1, end = position + (8 - position % 8); positionCible < end; positionCible++)
	{
		if((typePieceCible = this->plateau88[positionCible]) != VIDE)
		{
			if(EST_NOIR(typePiece) != EST_NOIR(typePieceCible))
			{
				nouveauDeplacement.destination = positionCible;
				nouveauDeplacement.capture = typePieceCible;
				captures.push_back(nouveauDeplacement);
			}
			
			break;
		}
		else
		{
			nouveauDeplacement.destination = positionCible;
			nouveauDeplacement.capture = typePieceCible;
			moves.push_back(nouveauDeplacement);
		}
	}
}

void Plateau::getDeplacementsCavaliers(int typePiece, int position, list<Deplacement> & moves, list<Deplacement> & captures) const
{
	Deplacement nouveauDeplacement;
	int positionCible, typePieceCible, ligne, colone;

	nouveauDeplacement.typePiece = typePiece;
	nouveauDeplacement.origine = position;

	//! Determine les lignes et les colonnes
	ligne = position / 8;
	colone = position % 8;

	//! 1. Haut
	if(ligne < 6)
	{
		//! Droite
		if(colone < 7)
		{
			positionCible = position + 17;
		
			if((typePieceCible = this->plateau88[positionCible]) != VIDE)
			{
				if(EST_NOIR(typePiece) != EST_NOIR(typePieceCible))
				{
					nouveauDeplacement.capture = typePieceCible;
					nouveauDeplacement.destination = positionCible;
					captures.push_back(nouveauDeplacement);
				}
			}
			else
			{
				nouveauDeplacement.capture = typePieceCible;
				nouveauDeplacement.destination = positionCible;
				moves.push_back(nouveauDeplacement);
			}
		}
		
		//! Gauche
		if(colone > 0)
		{
			positionCible = position + 15;
		
			if((typePieceCible = this->plateau88[positionCible]) != VIDE)
			{
				if(EST_NOIR(typePiece) != EST_NOIR(typePieceCible))
				{
					nouveauDeplacement.capture = typePieceCible;
					nouveauDeplacement.destination = positionCible;
					captures.push_back(nouveauDeplacement);
				}
			}
			else
			{
				nouveauDeplacement.capture = typePieceCible;
				nouveauDeplacement.destination = positionCible;
				moves.push_back(nouveauDeplacement);
			}		
		}
	}
	
	//! 2. Bas
	if(ligne > 1)
	{
		//! Doite
		if(colone < 7)
		{
			positionCible = position - 15;
		
			if((typePieceCible = this->plateau88[positionCible]) != VIDE)
			{
				if(EST_NOIR(typePiece) != EST_NOIR(typePieceCible))
				{
					nouveauDeplacement.capture = typePieceCible;
					nouveauDeplacement.destination = positionCible;
					captures.push_back(nouveauDeplacement);
				}
			}
			else
			{
				nouveauDeplacement.capture = typePieceCible;
				nouveauDeplacement.destination = positionCible;
				moves.push_back(nouveauDeplacement);
			}
		}
		
		//! Gauche
		if(colone > 0)
		{
			positionCible = position - 17;
		
			if((typePieceCible = this->plateau88[positionCible]) != VIDE)
			{
				if(EST_NOIR(typePiece) != EST_NOIR(typePieceCible))
				{
					nouveauDeplacement.capture = typePieceCible;
					nouveauDeplacement.destination = positionCible;
					captures.push_back(nouveauDeplacement);
				}
			}
			else
			{
				nouveauDeplacement.capture = typePieceCible;
				nouveauDeplacement.destination = positionCible;
				moves.push_back(nouveauDeplacement);
			}		
		}
	}

	//! 3. Droite
	if(colone < 6)
	{
		//! Haut
		if(ligne < 7)
		{
			positionCible = position + 10;
		
			if((typePieceCible = this->plateau88[positionCible]) != VIDE)
			{
				if(EST_NOIR(typePiece) != EST_NOIR(typePieceCible))
				{
					nouveauDeplacement.capture = typePieceCible;
					nouveauDeplacement.destination = positionCible;
					captures.push_back(nouveauDeplacement);
				}
			}
			else
			{
				nouveauDeplacement.capture = typePieceCible;
				nouveauDeplacement.destination = positionCible;
				moves.push_back(nouveauDeplacement);
			}
		}
		
		//! Bas
		if(ligne > 0)
		{
			positionCible = position - 6;
		
			if((typePieceCible = this->plateau88[positionCible]) != VIDE)
			{
				if(EST_NOIR(typePiece) != EST_NOIR(typePieceCible))
				{
					nouveauDeplacement.capture = typePieceCible;
					nouveauDeplacement.destination = positionCible;
					captures.push_back(nouveauDeplacement);
				}
			}
			else
			{
				nouveauDeplacement.capture = typePieceCible;
				nouveauDeplacement.destination = positionCible;
				moves.push_back(nouveauDeplacement);
			}		
		}
	}

	//! 4. Gauche
	if(colone > 1)
	{
		//! Haut
		if(ligne < 7)
		{
			positionCible = position + 6;
		
			if((typePieceCible = this->plateau88[positionCible]) != VIDE)
			{
				if(EST_NOIR(typePiece) != EST_NOIR(typePieceCible))
				{
					nouveauDeplacement.capture = typePieceCible;
					nouveauDeplacement.destination = positionCible;
					captures.push_back(nouveauDeplacement);
				}
			}
			else
			{
				nouveauDeplacement.capture = typePieceCible;
				nouveauDeplacement.destination = positionCible;
				moves.push_back(nouveauDeplacement);
			}
		}
		
		//! Bas
		if(ligne > 0)
		{
			positionCible = position - 10;
		
			if((typePieceCible = this->plateau88[positionCible]) != VIDE)
			{
				if(EST_NOIR(typePiece) != EST_NOIR(typePieceCible))
				{
					nouveauDeplacement.capture = typePieceCible;
					nouveauDeplacement.destination = positionCible;
					captures.push_back(nouveauDeplacement);
				}
			}
			else
			{
				nouveauDeplacement.capture = typePieceCible;
				nouveauDeplacement.destination = positionCible;
				moves.push_back(nouveauDeplacement);
			}		
		}
	}	
}

void Plateau::getDeplacementsFous(int typePiece, int position, list<Deplacement> & moves, list<Deplacement> & captures) const
{
	Deplacement nouveauDeplacement;
	int positionCible, typePieceCible, ligne, colone, i, j;

	nouveauDeplacement.typePiece = typePiece;
	nouveauDeplacement.origine = position;

	//! Determine les lignes et les colonnes
	ligne = position / 8;
	colone = position % 8;

	//! 1. Go nord-est
	for(i = ligne + 1, j = colone + 1; (i < 8) && (j < 8); i++, j++)
	{
		positionCible = i * 8 + j;
		if((typePieceCible = this->plateau88[positionCible]) != VIDE)
		{
			if(EST_NOIR(typePiece) != EST_NOIR(typePieceCible))
			{
				nouveauDeplacement.destination = positionCible;
				nouveauDeplacement.capture = typePieceCible;
				captures.push_back(nouveauDeplacement);
			}

			break;
		}
		else
		{
			nouveauDeplacement.destination = positionCible;
			nouveauDeplacement.capture = typePieceCible;
			moves.push_back(nouveauDeplacement);
		}
	}
	
	//! 2. Go sud-est
	for(i = ligne - 1, j = colone + 1; (i >= 0) && (j < 8); i--, j++)
	{
		positionCible = i * 8 + j;
		if((typePieceCible = this->plateau88[positionCible]) != VIDE)
		{
			if(EST_NOIR(typePiece) != EST_NOIR(typePieceCible))
			{
				nouveauDeplacement.destination = positionCible;
				nouveauDeplacement.capture = typePieceCible;
				captures.push_back(nouveauDeplacement);
			}
			
			break;
		}
		else
		{
			nouveauDeplacement.destination = positionCible;
			nouveauDeplacement.capture = typePieceCible;
			moves.push_back(nouveauDeplacement);
		}
	}

	//! 3. Go sud-ouest
	for(i = ligne - 1, j = colone - 1; (i >= 0) && (j >= 0); i--, j--)
	{
		positionCible = i * 8 + j;
		if((typePieceCible = this->plateau88[positionCible]) != VIDE)
		{
			if(EST_NOIR(typePiece) != EST_NOIR(typePieceCible))
			{
				nouveauDeplacement.destination = positionCible;
				nouveauDeplacement.capture = typePieceCible;
				captures.push_back(nouveauDeplacement);
			}
			
			break;
		}
		else
		{
			nouveauDeplacement.destination = positionCible;
			nouveauDeplacement.capture = typePieceCible;
			moves.push_back(nouveauDeplacement);
		}
	}

	//! 4. Go nord-ouest
	for(i = ligne + 1, j = colone - 1; (i < 8) && (j >= 0); i++, j--)
	{
		positionCible = i * 8 + j;
		if((typePieceCible = this->plateau88[positionCible]) != VIDE)
		{
			if(EST_NOIR(typePiece) != EST_NOIR(typePieceCible))
			{
				nouveauDeplacement.destination = positionCible;
				nouveauDeplacement.capture = typePieceCible;
				captures.push_back(nouveauDeplacement);
			}
			
			break;
		}
		else
		{
			nouveauDeplacement.destination = positionCible;
			nouveauDeplacement.capture = typePieceCible;
			moves.push_back(nouveauDeplacement);
		}
	}
}

void Plateau::getDeplacementsDames(int typePiece, int position, list<Deplacement> & moves, list<Deplacement> & captures) const
{
	//! La dame n'est que le produit cartésien des methode de déplacement de la tour et du fou !!
	this->getDeplacementsTours(typePiece, position, moves, captures);
	this->getDeplacementsFous(typePiece, position, moves, captures);
}


void Plateau::getDeplacementsRois(int typePiece, int position, list<Deplacement> & moves, list<Deplacement> & captures)
{
	Deplacement nouveauDeplacement;
	int positionCible, typePieceCible, ligne, colone;

	nouveauDeplacement.typePiece = typePiece;
	nouveauDeplacement.origine = position;

	//! Determine les lignes et les colonnes
	ligne = position / 8;
	colone = position % 8;

	//! 1. Deplacement gauche
	if(colone > 0)
	{
		//! 1.1 Haut
		if(ligne < 7)
		{
			positionCible = position + 7;
			if((typePieceCible = this->plateau88[positionCible]) != VIDE)
			{
				if(EST_NOIR(typePieceCible) != EST_NOIR(typePiece))
				{
					nouveauDeplacement.capture = typePieceCible;
					nouveauDeplacement.destination = positionCible;
					captures.push_back(nouveauDeplacement);
				}
			}
			else
			{
				nouveauDeplacement.destination = positionCible;
				nouveauDeplacement.capture = typePieceCible;
				moves.push_back(nouveauDeplacement);
			}
		}
		
		//! 1.2 Milieu
		positionCible = position - 1;
		if((typePieceCible = this->plateau88[positionCible]) != VIDE)
		{
			if(EST_NOIR(typePieceCible) != EST_NOIR(typePiece))
			{
				nouveauDeplacement.capture = typePieceCible;
				nouveauDeplacement.destination = positionCible;
				captures.push_back(nouveauDeplacement);
			}
		}
		else
		{
			nouveauDeplacement.destination = positionCible;
			nouveauDeplacement.capture = typePieceCible;
			moves.push_back(nouveauDeplacement);
		}
		
		//! 1.3 Bas
		if(ligne > 0)
		{
			positionCible = position - 9;
			if((typePieceCible = this->plateau88[positionCible]) != VIDE)
			{
				if(EST_NOIR(typePieceCible) != EST_NOIR(typePiece))
				{
					nouveauDeplacement.capture = typePieceCible;
					nouveauDeplacement.destination = positionCible;
					captures.push_back(nouveauDeplacement);
				}
			}
			else
			{
				nouveauDeplacement.destination = positionCible;
				nouveauDeplacement.capture = typePieceCible;
				moves.push_back(nouveauDeplacement);
			}
		}
	}
	
	//! 2. Deplacement droite
	if(colone < 7)
	{
		//! 2.1 Haut
		if(ligne < 7)
		{
			positionCible = position + 9;
			if((typePieceCible = this->plateau88[positionCible]) != VIDE)
			{
				if(EST_NOIR(typePieceCible) != EST_NOIR(typePiece))
				{
					nouveauDeplacement.capture = typePieceCible;
					nouveauDeplacement.destination = positionCible;
					captures.push_back(nouveauDeplacement);
				}
			}
			else
			{
				nouveauDeplacement.destination = positionCible;
				nouveauDeplacement.capture = typePieceCible;
				moves.push_back(nouveauDeplacement);
			}
		}
		
		//! 2.2 Milieu
		positionCible = position + 1;
		if((typePieceCible = this->plateau88[positionCible]) != VIDE)
		{
			if(EST_NOIR(typePieceCible) != EST_NOIR(typePiece))
			{
				nouveauDeplacement.capture = typePieceCible;
				nouveauDeplacement.destination = positionCible;
				captures.push_back(nouveauDeplacement);
			}
		}
		else
		{
			nouveauDeplacement.destination = positionCible;
			nouveauDeplacement.capture = typePieceCible;
			moves.push_back(nouveauDeplacement);
		}
		
		//! 2.3 Bas
		if(ligne > 0)
		{
			positionCible = position - 7;
			if((typePieceCible = this->plateau88[positionCible]) != VIDE)
			{
				if(EST_NOIR(typePieceCible) != EST_NOIR(typePiece))
				{
					nouveauDeplacement.capture = typePieceCible;
					nouveauDeplacement.destination = positionCible;
					captures.push_back(nouveauDeplacement);
				}
			}
			else
			{
				nouveauDeplacement.destination = positionCible;
				nouveauDeplacement.capture = typePieceCible;
				moves.push_back(nouveauDeplacement);
			}
		}
	}
	
	//! 3. Tout droit haut
	if(ligne < 7)
	{
		//! 2.2 Milieu
		positionCible = position + 8;
		if((typePieceCible = this->plateau88[positionCible]) != VIDE)
		{
			if(EST_NOIR(typePieceCible) != EST_NOIR(typePiece))
			{
				nouveauDeplacement.capture = typePieceCible;
				nouveauDeplacement.destination = positionCible;
				captures.push_back(nouveauDeplacement);
			}
		}
		else
		{
			nouveauDeplacement.destination = positionCible;
			nouveauDeplacement.capture = typePieceCible;
			moves.push_back(nouveauDeplacement);
		}	
	}
	
	//! 4. Tout droit bas
	if(ligne > 0)
	{
		//! 2.2 Milieu
		positionCible = position - 8;
		if((typePieceCible = this->plateau88[positionCible]) != VIDE)
		{
			if(EST_NOIR(typePieceCible) != EST_NOIR(typePiece))
			{
				nouveauDeplacement.capture = typePieceCible;
				nouveauDeplacement.destination = positionCible;
				captures.push_back(nouveauDeplacement);
			}
		}
		else
		{
			nouveauDeplacement.destination = positionCible;
			nouveauDeplacement.capture = typePieceCible;
			moves.push_back(nouveauDeplacement);
		}	
	}

	//! 5. Roque
	if(!EST_DEPLACE(typePiece) && !estVulnerable(position, typePiece))
	{
		//! Court
		positionCible = EST_NOIR(typePiece) ? F8 : F1;
		if((this->plateau88[positionCible] == VIDE) && !estVulnerable(positionCible, typePiece))
		{
			positionCible = EST_NOIR(typePiece) ? G8 : G1;
			if((this->plateau88[positionCible] == VIDE) && !estVulnerable(positionCible, typePiece))
			{
				positionCible = EST_NOIR(typePiece) ? H8 : H1;
				typePieceCible = this->plateau88[positionCible];
				if(!EST_DEPLACE(typePieceCible) && (TYPE_PIECE(typePieceCible) == TOUR) && !estVulnerable(positionCible, typePiece))
				{
					if(EST_NOIR(typePieceCible) == EST_NOIR(typePiece))
					{
						nouveauDeplacement.capture = VIDE;
						nouveauDeplacement.destination = EST_NOIR(typePiece) ? G8 : G1;
						moves.push_back(nouveauDeplacement);
					}
				}
			}
		}
		
		//! Long
		positionCible = EST_NOIR(typePiece) ? B8 : B1;
		if((this->plateau88[positionCible] == VIDE) && !estVulnerable(positionCible, typePiece))
		{
			positionCible = EST_NOIR(typePiece) ? C8 : C1;
			if((this->plateau88[positionCible] == VIDE) && !estVulnerable(positionCible, typePiece))
			{
				positionCible = EST_NOIR(typePiece) ? D8 : D1;
				if((this->plateau88[positionCible] == VIDE) && !estVulnerable(positionCible, typePiece))
				{
					positionCible = EST_NOIR(typePiece) ? A8 : A1;
					typePieceCible = this->plateau88[positionCible];
					if(!EST_DEPLACE(typePieceCible) && (TYPE_PIECE(typePieceCible) == TOUR) && !estVulnerable(positionCible, typePiece))
					{
						if(EST_NOIR(typePieceCible) == EST_NOIR(typePiece))
						{
							nouveauDeplacement.capture = VIDE;
							nouveauDeplacement.destination = EST_NOIR(typePiece) ? C8 : C1;
							moves.push_back(nouveauDeplacement);
						}
					}
				}
			}
		}
	}
}

bool Plateau::estVulnerable(int position, int typePiece) const
{
	int positionCible, typePieceCible, ligne, colone, i,j, end;

	ligne = position / 8;
	colone = position % 8;

	//! 1. Recherche les TOURS, DAMES et ROIS - Au dessus
	for(positionCible = position + 8; positionCible < 64; positionCible += 8)
	{
		if((typePieceCible = this->plateau88[positionCible]) != VIDE)
		{
			if(EST_NOIR(typePieceCible) != EST_NOIR(typePiece))
			{
				if((positionCible - position) == 8)
				{
					if(TYPE_PIECE(typePieceCible) == ROI)
						return true;
				}

				if((TYPE_PIECE(typePieceCible) == TOUR) || (TYPE_PIECE(typePieceCible) == DAME))
					return true;
			}

			break;
		}
	}

	//! 2. Recherche les TOURS, DAMES et ROIS - Au dessous
	for(positionCible = position - 8; positionCible >= 0; positionCible -= 8)
	{
		if((typePieceCible = this->plateau88[positionCible]) != VIDE)
		{
			if(EST_NOIR(typePieceCible) != EST_NOIR(typePiece))
			{
				if((position - positionCible) == 8)
				{
					if(TYPE_PIECE(typePieceCible) == ROI)
						return true;
				}

				if((TYPE_PIECE(typePieceCible) == TOUR) || (TYPE_PIECE(typePieceCible) == DAME))
					return true;
			}

			break;
		}
	}

	//! 3. Recherche les TOURS, DAMES et ROIS - A gauche
	for(positionCible = position - 1, end = position - (position % 8); positionCible >= end; positionCible--)
	{
		if((typePieceCible = this->plateau88[positionCible]) != VIDE)
		{
			if(EST_NOIR(typePiece) != EST_NOIR(typePieceCible))
			{
				if((position - positionCible) == 1)
				{
					if(TYPE_PIECE(typePieceCible) == ROI)
						return true;
				}

				if((TYPE_PIECE(typePieceCible) == TOUR) || (TYPE_PIECE(typePieceCible) == DAME))
					return true;
			}

			break;
		}
	}

	//! 4. Recherche les TOURS, DAMES et ROIS - A droite
	for(positionCible = position + 1, end = position + (8 - position % 8); positionCible < end; positionCible++)
	{
		if((typePieceCible = this->plateau88[positionCible]) != VIDE)
		{
			if(EST_NOIR(typePiece) != EST_NOIR(typePieceCible))
			{
				if((positionCible - position) == 1)
				{
					if(TYPE_PIECE(typePieceCible) == ROI)
						return true;
				}

				if((TYPE_PIECE(typePieceCible) == TOUR) || (TYPE_PIECE(typePieceCible) == DAME))
					return true;
			}

			break;
		}
	}

	//! 5. Recherche les FOUS, DAMES, ROIS et PIONS - Nord-Est
	for(i = ligne + 1, j = colone + 1; (i < 8) && (j < 8); i++, j++)
	{
		positionCible = i * 8 + j;
		if((typePieceCible = this->plateau88[positionCible]) != VIDE)
		{
			if(EST_NOIR(typePiece) != EST_NOIR(typePieceCible))
			{
				if((positionCible - position) == 9)
				{
					if(TYPE_PIECE(typePieceCible) == ROI)
						return true;
					else if(!EST_NOIR(typePiece) && (TYPE_PIECE(typePieceCible) == PION))
						return true;
				}

				if((TYPE_PIECE(typePieceCible) == FOU) || (TYPE_PIECE(typePieceCible) == DAME))
					return true;
			}

			break;
		}
	}
	
	//! 6. Recherche les FOUS, DAMES, ROIS et PIONS - Sud-Est
	for(i = ligne - 1, j = colone + 1; (i >= 0) && (j < 8); i--, j++)
	{
		positionCible = i * 8 + j;
		if((typePieceCible = this->plateau88[positionCible]) != VIDE)
		{
			if(EST_NOIR(typePiece) != EST_NOIR(typePieceCible))
			{
				if((position - positionCible) == 7)
				{
					if(TYPE_PIECE(typePieceCible) == ROI)
						return true;
					else if(EST_NOIR(typePiece) && (TYPE_PIECE(typePieceCible) == PION))
						return true;
				}

				if((TYPE_PIECE(typePieceCible) == FOU) || (TYPE_PIECE(typePieceCible) == DAME))
					return true;
			}

			break;
		}
	}

	//! 7. Recherche les FOUS, DAMES, ROIS et PIONS - Sud-Ouest
	for(i = ligne - 1, j = colone - 1; (i >= 0) && (j >= 0); i--, j--)
	{
		positionCible = i * 8 + j;
		if((typePieceCible = this->plateau88[positionCible]) != VIDE)
		{
			if(EST_NOIR(typePiece) != EST_NOIR(typePieceCible))
			{
				if((position - positionCible) == 9)
				{
					if(TYPE_PIECE(typePieceCible) == ROI)
						return true;
					else if(EST_NOIR(typePiece) && (TYPE_PIECE(typePieceCible) == PION))
						return true;
				}

				if((TYPE_PIECE(typePieceCible) == FOU) || (TYPE_PIECE(typePieceCible) == DAME))
					return true;
			}

			break;
		}
	}

	//! 8. Recherche les FOUS, DAMES, ROIS et PIONS - Nord-Ouest
	for(i = ligne + 1, j = colone - 1; (i < 8) && (j >= 0); i++, j--)
	{
		positionCible = i * 8 + j;
		if((typePieceCible = this->plateau88[positionCible]) != VIDE)
		{
			if(EST_NOIR(typePiece) != EST_NOIR(typePieceCible))
			{
				if((positionCible - position) == 7)
				{
					if(TYPE_PIECE(typePieceCible) == ROI)
						return true;
					else if(!EST_NOIR(typePiece) && (TYPE_PIECE(typePieceCible) == PION))
						return true;
				}

				if((TYPE_PIECE(typePieceCible) == FOU) || (TYPE_PIECE(typePieceCible) == DAME))
					return true;
			}

			break;
		}
	}
	
	//! 9. Recherche les CAVALIERS - Au dessus
	if(ligne < 6)
	{
		//! Droite
		if(colone < 7)
		{
			positionCible = position + 17;

			if((typePieceCible = this->plateau88[positionCible]) != VIDE)
			{
				if(EST_NOIR(typePiece) != EST_NOIR(typePieceCible))
				{
					if(TYPE_PIECE(typePieceCible) == CAVALIER) return true;
				}
			}
		}
		
		//! Gauche
		if(colone > 0)
		{
			positionCible = position + 15;
		
			if((typePieceCible = this->plateau88[positionCible]) != VIDE)
			{
				if(EST_NOIR(typePiece) != EST_NOIR(typePieceCible))
				{
					if(TYPE_PIECE(typePieceCible) == CAVALIER) return true;
				}
			}		
		}
	}
	
	//! 10. Recherche les CAVALIERS - Au dessous
	if(ligne > 1)
	{
		//! Droite
		if(colone < 7)
		{
			positionCible = position - 15;
		
			if((typePieceCible = this->plateau88[positionCible]) != VIDE)
			{
				if(EST_NOIR(typePiece) != EST_NOIR(typePieceCible))
				{
					if(TYPE_PIECE(typePieceCible) == CAVALIER) return true;
				}
			}
		}
		
		//! Gauche
		if(colone > 0)
		{
			positionCible = position - 17;
		
			if((typePieceCible = this->plateau88[positionCible]) != VIDE)
			{
				if(EST_NOIR(typePiece) != EST_NOIR(typePieceCible))
				{
					if(TYPE_PIECE(typePieceCible) == CAVALIER) return true;
				}
			}		
		}
	}

	//! 11. Recherche les CAVALIERS - Droite
	if(colone < 6)
	{
		//! Au dessus
		if(ligne < 7)
		{
			positionCible = position + 10;
		
			if((typePieceCible = this->plateau88[positionCible]) != VIDE)
			{
				if(EST_NOIR(typePiece) != EST_NOIR(typePieceCible))
				{
					if(TYPE_PIECE(typePieceCible) == CAVALIER) return true;
				}
			}
		}
		
		//! Au dessous
		if(ligne > 0)
		{
			positionCible = position - 6;
		
			if((typePieceCible = this->plateau88[positionCible]) != VIDE)
			{
				if(EST_NOIR(typePiece) != EST_NOIR(typePieceCible))
				{
					if(TYPE_PIECE(typePieceCible) == CAVALIER) return true;
				}
			}		
		}
	}

	//! 12. Recherche les CAVALIERS - Gauche
	if(colone > 1)
	{
		//! Au dessus
		if(ligne < 7)
		{
			positionCible = position + 6;
		
			if((typePieceCible = this->plateau88[positionCible]) != VIDE)
			{
				if(EST_NOIR(typePiece) != EST_NOIR(typePieceCible))
				{
					if(TYPE_PIECE(typePieceCible) == CAVALIER) return true;
				}
			}
		}
		
		//! En dessous
		if(ligne > 0)
		{
			positionCible = position - 10;
		
			if((typePieceCible = this->plateau88[positionCible]) != VIDE)
			{
				if(EST_NOIR(typePiece) != EST_NOIR(typePieceCible))
				{
					if(TYPE_PIECE(typePieceCible) == CAVALIER) return true;
				}
			}		
		}
	}	

	return false;
}
//! Verifie si un deplacement est valide
bool Plateau::estDeplacementValide(int couleur, Deplacement & deplacement)
{
	bool valid = false;
	list<Deplacement> reguliers, nulls;

	getDeplacements(couleur, reguliers, reguliers, nulls);

	for(list<Deplacement>::iterator i = reguliers.begin(); i != reguliers.end() && !valid; ++i)
	{
		if(deplacement.origine == (*i).origine && deplacement.destination == (*i).destination)
		{
			deplacement = *i;

			this->deplacement(deplacement);
			if(!estVulnerable(couleur ? roi_noir_position : roi_blanc_position, couleur))
				valid = true;
			annulerDeplacement(*i);
		}
	}

	return valid;
}
//! Renvoi le statut en cours
JoueurEchec::Status Plateau::getJoueurStatus(int couleur)
{
	bool king_vulnerable = false, can_move = false;
	list<Deplacement> reguliers, nulls;

	getDeplacements(couleur, reguliers, reguliers, nulls);

	if(estVulnerable(couleur ? roi_noir_position : roi_blanc_position, couleur))
		king_vulnerable = true;

	for(list<Deplacement>::iterator i = reguliers.begin(); i != reguliers.end() && !can_move; ++i)
	{
		this->deplacement(*i);
		if(!estVulnerable(couleur ? roi_noir_position : roi_blanc_position, couleur))
		{
			can_move = true;
		}
		annulerDeplacement(*i);
	}

	if(king_vulnerable && can_move)
		return JoueurEchec::EnEchec;
	if(king_vulnerable && !can_move)
		return JoueurEchec::EchecMat;
	if(!king_vulnerable && !can_move)
		return JoueurEchec::Pat;

	return JoueurEchec::Normal;
}
//! METHODE DE DEPLACEMENTS
void Plateau::deplacement(const Deplacement & deplacement)
{
	//! Les rois recoivent un traitement spécial
	switch(TYPE_PIECE(deplacement.typePiece))
	{
		case ROI:
			deplacementRoi(deplacement);
			break;
		case PION:
			if(deplacement.destination != deplacement.origine) {
				deplacementPion(deplacement);
				break;
			}
		default:
			this->plateau88[(int)deplacement.origine] = VIDE;
			this->plateau88[(int)deplacement.destination] = SET_DEPLACE(deplacement.typePiece);
			break;
	}
}

void Plateau::annulerDeplacement(const Deplacement & deplacement)
{
	//! Les rois recoivent un traitement spécial
	switch(TYPE_PIECE(deplacement.typePiece))
	{
		case ROI:
			annulerDeplacementRoi(deplacement);
			break;
		case PION:
			if(deplacement.destination != deplacement.origine) {
				annulerDeplacementPion(deplacement);
				break;
			}
		default:
			this->plateau88[(int)deplacement.origine] = deplacement.typePiece;
			this->plateau88[(int)deplacement.destination] = deplacement.capture;
			break;
	}
}

void Plateau::deplacementPion(const Deplacement & deplacement)
{
	int capture_field;

	//! Verifie si capture en passant
	if(EST_PASSANT(deplacement.capture))
	{
		if(EST_NOIR(deplacement.typePiece))
		{
			capture_field = deplacement.destination + 8;
			if((deplacement.origine / 8) == 3)
				this->plateau88[capture_field] = VIDE;
		}
		else
		{
			capture_field = deplacement.destination - 8;
			if((deplacement.origine / 8) == 4)
				this->plateau88[capture_field] = VIDE;
		}
	}

	this->plateau88[(int)deplacement.origine] = VIDE;

	//! Promotion d'un pion
	if(EST_NOIR(deplacement.typePiece)) {
		if(deplacement.destination / 8 == 0)
			this->plateau88[(int)deplacement.destination] = SET_DEPLACE(SET_NOIR(DAME));
		else
			this->plateau88[(int)deplacement.destination] = SET_DEPLACE(deplacement.typePiece);
	}
	else {
		if(deplacement.destination / 8 == 7)
			this->plateau88[(int)deplacement.destination] = SET_DEPLACE(DAME);
		else
			this->plateau88[(int)deplacement.destination] = SET_DEPLACE(deplacement.typePiece);
	}
}

void Plateau::annulerDeplacementPion(const Deplacement & deplacement)
{
	int capture_field;

	this->plateau88[(int)deplacement.origine] = SUPR_PASSANT(deplacement.typePiece);

	//! Verifie si capture en passant
	if(EST_PASSANT(deplacement.capture))
	{
		if(EST_NOIR(deplacement.typePiece))
		{
			capture_field = deplacement.destination + 8;
			if(deplacement.origine / 8 == 3) {
				this->plateau88[capture_field] = deplacement.capture;
				this->plateau88[(int)deplacement.destination] = VIDE;
			}
			else {
				this->plateau88[(int)deplacement.destination] = deplacement.capture;
			}
		}
		else
		{
			capture_field = deplacement.destination - 8;
			if(deplacement.origine / 8 == 4) {
				this->plateau88[capture_field] = deplacement.capture;
				this->plateau88[(int)deplacement.destination] = VIDE;
			}
			else {
				this->plateau88[(int)deplacement.destination] = deplacement.capture;
			}
		}
	}
	else
	{
		this->plateau88[(int)deplacement.destination] = deplacement.capture;
	}
}

void Plateau::deplacementRoi(const Deplacement & deplacement)
{
	//! Verifie si roque il y a
	if(!EST_DEPLACE(deplacement.typePiece))
	{
		switch(deplacement.destination)
		{
			case G1:
				this->plateau88[H1] = VIDE;
				this->plateau88[F1] = SET_DEPLACE(TOUR);
				break;
			case G8:
				this->plateau88[H8] = VIDE;
				this->plateau88[F8] = SET_DEPLACE(SET_NOIR(TOUR));
				break;
			case C1:
				this->plateau88[A1] = VIDE;
				this->plateau88[D1] = SET_DEPLACE(TOUR);
				break;
			case C8:
				this->plateau88[A8] = VIDE;
				this->plateau88[D8] = SET_DEPLACE(SET_NOIR(TOUR));
				break;
			default:
				break;
		}
	}

	//! Déplacement régulié
	this->plateau88[(int)deplacement.origine] = VIDE;
	this->plateau88[(int)deplacement.destination] = SET_DEPLACE(deplacement.typePiece);
	
	//! Mise a jour de la position des rois
	if(EST_NOIR(deplacement.typePiece)) {
		roi_noir_position = deplacement.destination;
	}
	else {
		roi_blanc_position = deplacement.destination;
	}
}

void Plateau::annulerDeplacementRoi(const Deplacement & deplacement)
{
	//! Verifie si roque il y a
	if(!EST_DEPLACE(deplacement.typePiece))
	{
		//! Set la tour en fonction de la cible du roi
		switch(deplacement.destination)
		{
			case G1:
				this->plateau88[H1] = TOUR;
				this->plateau88[F1] = VIDE;
				break;
			case G8:
				this->plateau88[H8] = SET_NOIR(TOUR);
				this->plateau88[F8] = VIDE;
				break;
			case C1:
				this->plateau88[A1] = TOUR;
				this->plateau88[D1] = VIDE;
				break;
			case C8:
				this->plateau88[A8] = SET_NOIR(TOUR);
				this->plateau88[D8] = VIDE;
				break;
			default:
				break;
		}
	}

	//! Annulation de déplacement régulière
	this->plateau88[(int)deplacement.origine] = deplacement.typePiece;
	this->plateau88[(int)deplacement.destination] = deplacement.capture;

	//! Mise a jour de la position des rois
	if(EST_NOIR(deplacement.typePiece)) {
		roi_noir_position = deplacement.origine;
	}
	else {
		roi_blanc_position = deplacement.origine;
	}
}
