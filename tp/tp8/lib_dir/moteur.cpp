#define F_CPU 8000000

#include <util/delay.h>
#include <avr/io.h>
#include "moteur.h"

/*
 * Cette fonction permet l'ajustement de la vitesse des deux moteurs
*/
void ajustementPWM(uint8_t vMoteur1, uint8_t vMoteur2) 
{
	// mise a un des sorties OC1A et OC1B sur comparaison
	TCCR1A |= _BV(COM1A0);
	TCCR1A |= _BV(COM1A1);
	TCCR1A |= _BV(COM1B0);
	TCCR1A |= _BV(COM1B1);
	// reussie en mode PWM 8 bits, phase correcte
	// et valeur de TOP fixe a 0xFF (mode #1 de la table 16­5
	// page 130 de la description technique du ATmega324PA)
	TCCR1A |= _BV(WGM10);
	
	
	OCR1A = vMoteur1;
	OCR1B = vMoteur2;

	// division d'horloge par 8 ­ implique une frequence de PWM fixe
	TCCR1B |= _BV(CS11);
}
