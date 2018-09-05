////////////////////////////////////////////////////////////////////////////////////////////
// FICHIER:             minuterie.cpp
// AUTEURS:             Abdellatif Amrani, Mohamed Amine Belaid, Bilal Itani, Jackie Phung
// DATE:                18 Mars 2015
// DESCRIPTION:         Ce fichier .cpp contient l'implementation de divers fonctions
//			lie aux minuteries.
////////////////////////////////////////////////////////////////////////////////////////////
#define F_CPU 8000000
#include <util/delay.h>
#include <avr/io.h>
#include "minuterie.h"
#include <stdint.h>


//Inspiré de : http://maxembedded.com/2011/06/introduction­to­avr­timers/
/*
 * La fonction prend en parametres la duree du delai voulu en ms et 
 * le prescale sur lequel le timer est regle et renvoi le nombre de 
 * cycle d'horloge necessaire pour generer le delai.
*/

////////////////////////////////////////////////////////////////////////////////////////////
// FONCTION:            uint16_t calculerNombreCycles(uint16_t duree, uint16_t prescale)
// DESCRIPTION:        	Cette fonction permet de calculer la valeur que doit compter le
//			timer1 en fonction de la duree (en ms) et d'une variable prescale.
// PARAMETRES:		Un entier non signe sur 16 bits representant la duree (IN) et un
//			entier non signe sur 16 bits representant le prescale (IN).           
// VALEUR DE RETOUR :	Un entier non signe sur 16 bits representant le nombre
//			auquelle doit compter le compteur pour le delai specifie en
//			parametre.
////////////////////////////////////////////////////////////////////////////////////////////
uint16_t calculerNombreCycles(uint16_t duree, uint16_t prescale)
{
	float frequenceReduite = F_CPU / (float(prescale)); //résultat en Hz
	float periodeReduite = 1000 / frequenceReduite; //résultat en ms
	uint16_t compteurReduit = (duree / periodeReduite) - 1;
	return compteurReduit;
}


////////////////////////////////////////////////////////////////////////////////////////////
// FONCTION:            void partirMinuterie0(uint8_t duree)
// DESCRIPTION:        	Cette fonction permet de partir la minuterie 0.
// PARAMETRES:		Un entier non signe sur 8 bits representant la duree (IN).         
// VALEUR DE RETOUR :	Aucune.
////////////////////////////////////////////////////////////////////////////////////////////
void partirMinuterie0(uint8_t duree) { 
	// mettre les bits du timer a 0

	TCNT0 = 0; //Compteur commence a 0.

	// mettre la valeut de comparaison a duree

	OCR0A = duree;

	// clear on compare match

	TCCR0A |= _BV(COM0A0); //Toggle on compare match
	// prescalar clk/1024

	TCCR0B |= _BV(CS02); //Prescaler /1024

	TCCR0B |= _BV(CS00);

	TCCR0A |= _BV(WGM01); //Mode CTC.

	// output compare A match interrupt enable.

	TIMSK0 = _BV(OCIE0A);
}

////////////////////////////////////////////////////////////////////////////////////////////
// FONCTION:            void partirMinuterie1(uint16_t duree)
// DESCRIPTION:        	Cette fonction permet de partir la minuterie 1.
// PARAMETRES:		Un entier non signe sur 16 bits representant la duree (IN).         
// VALEUR DE RETOUR :	Aucune.
////////////////////////////////////////////////////////////////////////////////////////////
void partirMinuterie1(uint16_t duree) { 
	// mettre les bits du timer a 0

	TCNT1 = 0; //Compteur commence a 0.

	// mettre la valeut de comparaison a duree

	OCR1A = calculerNombreCycles(duree, 1024);

	// clear on compare match

	TCCR1A |= _BV(COM1A1); //Set OCnA/OCnB on Compare Match (Set output to

	// prescalar clk/1024

	TCCR1B |= _BV(CS12); //Prescaler /1024

	TCCR1B |= _BV(CS10);

	TCCR1B |= _BV(WGM12); //Mode CTC.

	// output compare A match interrupt enable.

	TIMSK1 = _BV(OCIE1A);

}
////////////////////////////////////////////////////////////////////////////////////////////
// FONCTION:            void partirMinuterie2(uint8_t duree) 
// DESCRIPTION:        	Cette fonction permet de partir la minuterie 2.
// PARAMETRES:		Un entier non signe sur 8 bits representant la duree (IN).         
// VALEUR DE RETOUR :	Aucune.
////////////////////////////////////////////////////////////////////////////////////////////
void partirMinuterie2(uint8_t duree) { 
	// mettre les bits du timer a 0

	TCNT2 = 0; //Compteur commence a 0.

	// mettre la valeut de comparaison a duree

	OCR2A = duree;

	// clear on compare match

	TCCR2A |= _BV(COM2A0); //Set OCnA/OCnB on Compare Match (Set output to
	TCCR2A |= _BV(COM2A1);

	// prescalar clk/1024

	TCCR2B |= _BV(CS22); //Prescaler /1024
	TCCR2B |= _BV(CS21);
	TCCR2B |= _BV(CS20);

	TCCR2B |= _BV(WGM21); //Mode CTC.

	// output compare A match interrupt enable.

	TIMSK2 = _BV(OCIE2A);
}
////////////////////////////////////////////////////////////////////////////////////////////
// FONCTION:            void arreterMinuterie0()
// DESCRIPTION:        	Cette fonction permet l'arret de la minuterie 0;
// PARAMETRES:		Un entier non signe sur 8 bits representant la duree (IN).         
// VALEUR DE RETOUR :	Aucune.
////////////////////////////////////////////////////////////////////////////////////////////
void arreterMinuterie0()
{
	TCCR0A &= ~_BV(COM0A0);
}


