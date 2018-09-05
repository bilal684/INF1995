////////////////////////////////////////////////////////////////////////////////////////////
// FICHIER:             moteur.cpp
// AUTEURS:             Abdellatif Amrani, Mohamed Amine Belaid, Bilal Itani, Jackie Phung
// DATE:                18 Mars 2015
// DESCRIPTION:         Ce fichier .cpp contient l'implementation de divers fonctions
//			lie aux moteurs.
////////////////////////////////////////////////////////////////////////////////////////////

#define F_CPU 8000000

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "moteur.h"
#include "uart.h"

////////////////////////////////////////////////////////////////////////////////////////////
// FONCTION:            void ajustementPWM(uint8_t val1, uint8_t val2) 
// DESCRIPTION:        	Cette fonction permet d'ajuster le signal PWM des moteurs.
// PARAMETRES:		Un entier non signe sur 8 bits definissant OCR1A (IN) et un entier
//			non signe sur 8 bits definissant OCR1B.     
// VALEUR DE RETOUR :	Aucune.
////////////////////////////////////////////////////////////////////////////////////////////
void ajustementPWM(uint8_t val1, uint8_t val2) 
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
	
	
	OCR1A = val1;
	OCR1B = val2;

	// division d'horloge par 8 ­ implique une frequence de PWM fixe
	TCCR1B |= _BV(CS11);
}
////////////////////////////////////////////////////////////////////////////////////////////
// FONCTION:            void arreterMoteurs()
// DESCRIPTION:        	Cette fonction permet l'arret des moteurs.
// PARAMETRES:		Aucun.
// VALEUR DE RETOUR :	Aucune.
////////////////////////////////////////////////////////////////////////////////////////////
void arreterMoteurs()
{
	TCCR1A &= ~_BV(COM1A0);
	TCCR1A &= ~_BV(COM1A1);
	TCCR1A &= ~_BV(COM1B0);
	TCCR1A &= ~_BV(COM1B1);
}
////////////////////////////////////////////////////////////////////////////////////////////
// FONCTION:            void avancer(uint8_t vitesse)
// DESCRIPTION:        	Cette fonction permet de faire avancer le robot.
// PARAMETRES:		Un entier non signe sur 8 bits representant la vitesse des roues.
// VALEUR DE RETOUR :	Aucune.
////////////////////////////////////////////////////////////////////////////////////////////
void avancer(uint8_t vitesse)
{
	PORTD = ( PORTD & 0xFB ); // PORTD2 a 0
	PORTD = ( PORTD & 0xF7 ); // PORTD3 a 0
	ajustementPWM(255 - vitesse, 255 - vitesse);	
}
////////////////////////////////////////////////////////////////////////////////////////////
// FONCTION:            void reculer(uint8_t vitesse)
// DESCRIPTION:        	Cette fonction permet de faire reculer le robot.
// PARAMETRES:		Un entier non signe sur 8 bits representant la vitesse des roues.
// VALEUR DE RETOUR :	Aucune.
////////////////////////////////////////////////////////////////////////////////////////////
void reculer(uint8_t vitesse)
{
	
	PORTD = ( PORTD | 0x04 ); // PORTD2 a 1
	PORTD = ( PORTD | 0x08 ); // PORTD3 a 1
	ajustementPWM(255 - vitesse, 255 - vitesse);	
}
////////////////////////////////////////////////////////////////////////////////////////////
// FONCTION:            void pivoterADroite(uint8_t vitesse)
// DESCRIPTION:        	Cette fonction permet de faire pivoter a droite le robot autour
//			d'un axe de rotation situe au milieu du robot.
// PARAMETRES:		Un entier non signe sur 8 bits representant la vitesse des roues.
// VALEUR DE RETOUR :	Aucune.
////////////////////////////////////////////////////////////////////////////////////////////
void pivoterADroite(uint8_t vitesse)
{
	ajustementPWM(255 - vitesse,255 - vitesse);	
	PORTD = ( PORTD & 0xFB ); // PORTD2 a 0
	PORTD = ( PORTD | 0x08 ); // PORTD3 a 1	
}
////////////////////////////////////////////////////////////////////////////////////////////
// FONCTION:            void pivoterADroite(uint8_t vitesse)
// DESCRIPTION:        	Cette fonction permet de faire pivoter a gauche le robot autour
//			d'un axe de rotation situe au milieu du robot.
// PARAMETRES:		Un entier non signe sur 8 bits representant la vitesse des roues.
// VALEUR DE RETOUR :	Aucune.
////////////////////////////////////////////////////////////////////////////////////////////
void pivoterAGauche(uint8_t vitesse)
{
	ajustementPWM(255 - vitesse,255 - vitesse);
	PORTD = ( PORTD | 0x04 ); // PORTD2 a 1
	PORTD = ( PORTD & 0xF7 ); // PORTD3 a 0
}
////////////////////////////////////////////////////////////////////////////////////////////
// FONCTION:            void pivoterADroite(uint8_t vitesse)
// DESCRIPTION:        	Cette fonction permet de faire tourner a droite le robot autour
//			d'un axe de rotation situe sur la roue droite.
// PARAMETRES:		Un entier non signe sur 8 bits representant la vitesse de la roue
//			gauche.
// VALEUR DE RETOUR :	Aucune.
////////////////////////////////////////////////////////////////////////////////////////////
void tournerADroite(uint8_t vitesse)
{
	ajustementPWM(255 ,255 - vitesse);	
	PORTD = ( PORTD & 0xFB ); // PORTD2 a 0
}
////////////////////////////////////////////////////////////////////////////////////////////
// FONCTION:            void tournerAGauche(uint8_t vitesse)
// DESCRIPTION:        	Cette fonction permet de faire tourner a gauche le robot autour
//			d'un axe de rotation situe sur la roue gauche.
// PARAMETRES:		Un entier non signe sur 8 bits representant la vitesse de la
//			roue droite.
// VALEUR DE RETOUR :	Aucune.
////////////////////////////////////////////////////////////////////////////////////////////
void tournerAGauche(uint8_t vitesse)
{
	ajustementPWM(255 - vitesse, 255);
	PORTD = ( PORTD & 0xF7 ); // PORTD3 a 0
}
////////////////////////////////////////////////////////////////////////////////////////////
// FONCTION:            void tournerADroite(uint8_t vitesse, uint8_t pourcent)
// DESCRIPTION:        	Cette fonction permet de faire tourner a droite le robot autour
//			d'un axe de rotation situe sur la roue gauche.
// PARAMETRES:		Un entier non signe sur 8 bits representant la vitesse de la roue
//			gauche (IN) et un entier non signe sur 8 bits representant le 
//			pourcentage PWM de la vitesse de la roue.
// VALEUR DE RETOUR :	Aucune.
////////////////////////////////////////////////////////////////////////////////////////////
void tournerADroite(uint8_t vitesse, uint8_t pourcent)
{
	if(pourcent > 100)
		pourcent = 100;
	ajustementPWM(255 - vitesse * pourcent / 100 ,255 - vitesse);	
	PORTD = ( PORTD & 0xFB ); // PORTD2 a 0
}
////////////////////////////////////////////////////////////////////////////////////////////
// FONCTION:            void tournerAGauche(uint8_t vitesse, uint8_t pourcent)
// DESCRIPTION:        	Cette fonction permet de faire tourner a gauche le robot autour
//			d'un axe de rotation situe sur la roue droite.
// PARAMETRES:		Un entier non signe sur 8 bits representant la vitesse de la roue
//			gauche (IN) et un entier non signe sur 8 bits representant le 
//			pourcentage PWM de la vitesse de la roue.
// VALEUR DE RETOUR :	Aucune.
////////////////////////////////////////////////////////////////////////////////////////////
void tournerAGauche(uint8_t vitesse, uint8_t pourcent)
{
	if(pourcent > 100)
		pourcent = 100;
	ajustementPWM(255 - vitesse, 255 - vitesse * pourcent / 100);
	PORTD = ( PORTD & 0xF7 ); // PORTD3 a 0
}


