#define F_CPU 8000000
#include <avr/io.h>
#include "uart.h"
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

// Transmission d'un caractere de l'UART vers le PC
void transmissionUART ( uint8_t donnee ) {
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) )
		;
	/* Put data into buffer, sends the data */
	UDR0 = donnee;
}

/*
Transmission d'un entier sur 8bits de l'UART vers le PC.
*/

void transmissionEntierUART(uint8_t entier){
	if(entier < 10)
		//Transmission de l'entier + le code ASCII de 0 pour afficher le caractere entier.
		transmissionUART(entier + 48); 
	else
	{
		if(entier < 100){
			//Affichage du caractere representant la dizaine.
			transmissionEntierUART(entier/10); 
			//Affichage du caractere representant l'unite.
			transmissionEntierUART(entier%10); 
		}
		else{
			//Affichage du caractere representant les centaine
			transmissionEntierUART(entier/100);
			//Permet le renvoi de la partie des dizaines et des unites dans la meme fonction.
			transmissionEntierUART(entier%100); 
							
		}
	}
}		

