////////////////////////////////////////////////////////////////////////////////////////////
// FICHIER:            	initInterrupts.h
// AUTEURS:             Abdellatif Amrani, Mohamed Amine Belaid, Bilal Itani, Jackie Phung
// DATE:                18 Mars 2015
// DESCRIPTION:         Ce fichier .h contient la definition de divers fonctions en lien
//			avec l'initialisation d'interruptions
////////////////////////////////////////////////////////////////////////////////////////////
#ifndef INIT_INTERRUPTS_H
#define INIT_INTERRUPTS_H

#include <avr/io.h>
#include <avr/interrupt.h>

//Cette fonction permet de sensibiliser les interruptions externes à ces fronts montant et/ou descendant
void initInterruptions(bool risingEdge, bool fallingEdge);

#endif
