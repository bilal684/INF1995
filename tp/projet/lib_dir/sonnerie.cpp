////////////////////////////////////////////////////////////////////////////////////////////
// FICHIER:             sonnerie.cpp
// AUTEURS:             Abdellatif Amrani, Mohamed Amine Belaid, Bilal Itani, Jackie Phung
// DATE:                18 Mars 2015
// DESCRIPTION:         Ce fichier .cpp contient l'implementation de divers fonctions
//			au piezoelectrique.
////////////////////////////////////////////////////////////////////////////////////////////

#include "sonnerie.h"
#include "minuterie.h"
#include <util/delay.h>
////////////////////////////////////////////////////////////////////////////////////////////
// FONCTION:            void sonneriePWM(uint8_t note)
// DESCRIPTION:        	Cette fonction permet d'ajuster la frequence du son emis par le 
//			piezoelectrique.
// PARAMETRES:		Un entier non signe sur 8 bits representant la note.
// VALEUR DE RETOUR :	Aucune.
////////////////////////////////////////////////////////////////////////////////////////////
void sonneriePWM(uint8_t note) 
{
	uint16_t frequenceSon = 110 * pow(2,(float)(note - 45)/12.0); //calcul de la frequence selon les valeurs du tableau de l'enonce du TP9.
	OCR2A = (F_CPU / (2*256* frequenceSon)) -1; //page 144 documentation atmel.
	TCCR2A |= _BV(WGM21);//Mode CTC
	TCCR2A |= _BV(COM2A0);// toggle OC2A on compare match
	// division d'horloge par 256.
	TCCR2B |= _BV(CS22);
	TCCR2B |= _BV(CS21);
}
////////////////////////////////////////////////////////////////////////////////////////////
// FONCTION:            void arreterSonnerie()
// DESCRIPTION:        	Cette fonction permet d'arreter le son emis par le piezoelectrique.
// PARAMETRES:		Aucun.
// VALEUR DE RETOUR :	Aucune.
////////////////////////////////////////////////////////////////////////////////////////////
void arreterSonnerie()
{
	TCCR2A = (TCCR2A & ~_BV(COM2A0)); //Desactivation du "toggle on compare match" du mode CTC.
}
