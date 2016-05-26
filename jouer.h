#ifndef CHESS_PLAYER_H_INCLUDED
#define CHESS_PLAYER_H_INCLUDED

class Plateau;
class Deplacement;

class JoueurEchec
{
	public:

		enum Status { Normal, EnEchec, Pat, EchecMat };

		JoueurEchec(int couleur)
		 : couleur(couleur)
		{};

		virtual ~JoueurEchec(){};
		
		virtual bool getDeplacement(Plateau & plateau, Deplacement & deplacement) const = 0;

	protected:

		int couleur;
};

#endif

