////////////////////////////////////////////////////////////////////////////////////////////
// FICHIER:             boutonPoussoir.cpp
// AUTEURS:             Abdellatif Amrani, Mohamed Amine Belaid, Bilal Itani, Jackie Phung
// DATE:                18 Mars 2015
// DESCRIPTION:         Ce fichier .cpp contient l'implementation de divers fonctions
//			lie aux interruptions
////////////////////////////////////////////////////////////////////////////////////////////
#include "initInterrupts.h"

//Cette fonction permet de sensibiliser les interruptions externes à ces fronts montant et/ou descendant
////////////////////////////////////////////////////////////////////////////////////////////
// FONCTION:            void initInterruptions(bool risingEdge, bool fallingEdge)
// DESCRIPTION:         Cette fonction intialise les interruptions aux rising edge ou au
//			falling edge de l'horloge.
// PARAMETRES:		Deux booleen (IN) permettant de determiner sur quelle front
//			initialiser les interruptions.
// VALEUR DE RETOUR :	Aucune.
////////////////////////////////////////////////////////////////////////////////////////////
void initInterruptions(bool risingEdge, bool fallingEdge)
{
	cli();//Bloquer les interruptions
	//Ajuster le registre EICRA afin de sensibiliser les interruptions externes:
	if(!risingEdge && fallingEdge)//aux falling edges
		EICRA |= _BV(ISC01);
	else if(risingEdge && !fallingEdge)//aux rising edges
		EICRA |= _BV(ISC01) | _BV(ISC00);
	else if(risingEdge && fallingEdge)//aux deux
		EICRA |= _BV(ISC00);
	sei();//repermettre les interruptions
}
