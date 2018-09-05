#define F_CPU 8000000
#include <util/delay.h>
#include <avr/io.h>
#include "minuterie.h"


//Inspiré de : http://maxembedded.com/2011/06/introduction­to­avr­timers/
/*
 * La fonction prend en parametres la duree du delai voulu en ms et 
 * le prescale sur lequel le timer est regle et renvoi le nombre de 
 * cycle d'horloge necessaire pour generer le delai.
*/


uint16_t calculerNombreCycles(uint16_t duree, uint16_t prescale)
{
	float frequenceReduite = F_CPU / (float(prescale)); //résultat en Hz
	float periodeReduite = 1000 / frequenceReduite; //résultat en ms
	uint16_t compteurReduit = (duree / periodeReduite) - 1;
	return compteurReduit;
}

/*
 *  Fonction qui permet de lancer une minuterie, elle prend un entier 
 *  non signe de 16 bits en parametre, duree qui correspondra a OCR1A.
*/
void partirMinuterie(uint16_t duree) { 
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
