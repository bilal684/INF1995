////////////////////////////////////////////////////////////////////////////////////////////
// FICHIER:             programmerMem.cpp
// AUTEURS:             Abdellatif Amrani, Mohamed Amine Belaid, Bilal Itani, Jackie Phung
// DATE:                18 Mars 2015
// DESCRIPTION:         Ce fichier .cpp contient le programme qui enregistre les
//			instructions presentes au prealable dans le buffer dans la memoire.
////////////////////////////////////////////////////////////////////////////////////////////

#define F_CPU 8000000
#include <avr/io.h>
#include "memoire_24.h"
#include <util/delay.h>

using namespace std;

////////////////////////////////////////////////////////////////////////////////////////////
// FONCTION:            void initialisationUART (void) 
// DESCRIPTION:         Cette fonction permet l'initialisation de l'UART en modifiant la
//			valeur des registres ci-dessous.
// PARAMETRES:          Aucun.
// VALEUR DE RETOUR:    Aucune.
////////////////////////////////////////////////////////////////////////////////////////////
void initialisationUART ( void ) {

	// 2400 bauds. Nous vous donnons la valeur des deux
	// premier registres pour vous éviter des complications
	UBRR0H = 0;
	UBRR0L = 0xCF;
	// permettre la reception et la transmission par le UART0
	// page 185
	UCSR0A |= _BV(TXC0) ;
	UCSR0A |= _BV(RXC0) ;

	UCSR0B |= _BV(RXEN0) ;
	UCSR0B |= _BV(TXEN0) ;
	// Format des trames: 8 bits, 1 stop bits, none parity
	// page 188
	UCSR0C |= _BV(UCSZ01) ;
	UCSR0C |= _BV(UCSZ00) ;
}


////////////////////////////////////////////////////////////////////////////////////////////
// FONCTION:            uint8_t ReceptionUART ( void )
// DESCRIPTION:         Cette fonction permet le transfert des donnees presentes dans le
//			buffer vers la memoire.
// PARAMETRES:          Aucun.
// VALEUR DE RETOUR:    Aucune.
////////////////////////////////////////////////////////////////////////////////////////////
uint8_t ReceptionUART ( void ) {
	// Attente de la reception des donnees 
	while ( !( UCSR0A & (1<<RXC0)) )
		;
	//prendre et retourner les valeurs recues du buffer.
	return UDR0;
}

////////////////////////////////////////////////////////////////////////////////////////////
// FONCTION:            int main()
// DESCRIPTION:         Contient l'execution du programme principal.
// PARAMETRES:          Aucun.
// VALEUR DE RETOUR:    Aucune.
////////////////////////////////////////////////////////////////////////////////////////////
int main(){
	initialisationUART ( );
	
	Memoire24CXXX laMemoire;
	uint8_t adresse = 0, caractere = 0;

	while(1){
		caractere = ReceptionUART (  );
		laMemoire.ecriture(adresse, caractere);
		_delay_ms(5);
		adresse ++;
	}
	return 0;
}
