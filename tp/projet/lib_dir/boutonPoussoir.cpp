////////////////////////////////////////////////////////////////////////////////////////////
// FICHIER:             boutonPoussoir.cpp
// AUTEURS:             Abdellatif Amrani, Mohamed Amine Belaid, Bilal Itani, Jackie Phung
// DATE:                18 Mars 2015
// DESCRIPTION:         Ce fichier .cpp contient l'implementation de divers fonctions
//			lie au bouton poussoire.
////////////////////////////////////////////////////////////////////////////////////////////

#include "boutonPoussoir.h"

////////////////////////////////////////////////////////////////////////////////////////////
// FONCTION:            void initialisationBouton()
// DESCRIPTION:         Cette fonction permet l'initialisation des interruptions externes
//			pour le bouton poussoire.
// PARAMETRES:		Aucun.           
// VALEUR DE RETOUR :	Aucune.
////////////////////////////////////////////////////////////////////////////////////////////
void initialisationBouton()
{
  EIMSK |= _BV(INT0);//Ajuster le registre EIMSK afin de permettre les interruptions externes au moyen du bouton poussoir
}
