#ifndef MINUTERIE_H
#define MINUTERIE_H

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

void partirMinuterie(uint16_t duree);

#endif
