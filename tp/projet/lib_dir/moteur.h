////////////////////////////////////////////////////////////////////////////////////////////
// FICHIER:            	moteur.h
// AUTEURS:             Abdellatif Amrani, Mohamed Amine Belaid, Bilal Itani, Jackie Phung
// DATE:                18 Mars 2015
// DESCRIPTION:         Ce fichier .h contient la definition de divers fonctions en lien
//			avec les moteurs.
////////////////////////////////////////////////////////////////////////////////////////////
#ifndef MOTEUR_H
#define MOTEUR_H
#include <stdint.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/io.h>

/*
 * Cette fonction permet l'ajustement de la vitesse des deux moteurs
*/
void ajustementPWM(uint8_t val1, uint8_t val2);
void tournerADroite(uint8_t vitesse);
void tournerAGauche(uint8_t vitesse);
void tournerADroite(uint8_t vitesse, uint8_t pourcent);
void tournerAGauche(uint8_t vitesse, uint8_t pourcent);
void pivoterAGauche(uint8_t vitesse);
void pivoterADroite(uint8_t vitesse);
void avancer(uint8_t vitesse);
void reculer(uint8_t vitesse);
//uint8_t changerVoie(uint8_t voie, uint8_t vitesse);
void changerVoieAGauche(uint8_t vitesse);
void changerVoieADroite(uint8_t vitesse);
void arreterMoteurs();
#endif
