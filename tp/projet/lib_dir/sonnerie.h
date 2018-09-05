////////////////////////////////////////////////////////////////////////////////////////////
// FICHIER:            	sonnerie.h
// AUTEURS:             Abdellatif Amrani, Mohamed Amine Belaid, Bilal Itani, Jackie Phung
// DATE:                18 Mars 2015
// DESCRIPTION:         Ce fichier .h contient la definition de divers fonctions en lien
//			avec le son emis par le piezoelectrique.
////////////////////////////////////////////////////////////////////////////////////////////
#ifndef SONNERIE_H
#define SONNERIE_H
#define F_CPU 8000000
#include <util/delay.h>
#include <avr/io.h>
void sonneriePWM(uint8_t note);
void arreterSonnerie();
#endif

