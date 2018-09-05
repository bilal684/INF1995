#ifndef BOUTONPOUSSOIR_H
#define BOUTONPOUSSOIR_H

#include <avr/io.h>
#include <avr/interrupt.h>


/*
 *Cette fonction permet de sensibiliser les interruptions externes 
 * du bouton poussoir à ces fronts montant et/ou descendant
 */
void initialisationBouton(bool risingEdge, bool fallingEdge);

#endif