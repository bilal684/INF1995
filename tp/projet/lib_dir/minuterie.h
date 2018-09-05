////////////////////////////////////////////////////////////////////////////////////////////
// FICHIER:            	minuterie.h
// AUTEURS:             Abdellatif Amrani, Mohamed Amine Belaid, Bilal Itani, Jackie Phung
// DATE:                18 Mars 2015
// DESCRIPTION:         Ce fichier .h contient la definition de divers fonctions en lien
//			avec les minuteries.
////////////////////////////////////////////////////////////////////////////////////////////
#ifndef MINUTERIE_H
#define MINUTERIE_H
#include <stdint.h>
/*
 * La fonction prend en parametres la duree du delai voulu en ms et 
 * le prescale sur lequel le timer est regle et renvoi le nombre de 
 * cycle d'horloge necessaire pour generer le delai.
*/

uint16_t calculerNombreCycles(uint16_t duree, uint16_t prescale);

/*
 *  Fonction qui permet de lancer une minuterie, elle prend un entier 
 *  non signe de 16 bits en parametre, duree qui correspondra a OCR1A.
*/

void partirMinuterie0(uint8_t duree);
void partirMinuterie1(uint16_t duree);
void partirMinuterie2(uint8_t duree);
void arreterMinuterie0();

#endif
