#define F_CPU 8000000
#include <avr/io.h>
#include "memoire_24.h"
#include <util/delay.h>

using namespace std;

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

// De PC vers l'USART
uint8_t UARTReceive ( void ) {
	/* Wait for date to be received */
	while ( !( UCSR0A & (1<<RXC0)) )
		;
	/* get and return received data from buffer*/
	return UDR0;
}

int main(){
	initialisationUART ( );
	
	Memoire24CXXX laMemoire;
	uint8_t adresse = 0;
	
	uint8_t caractere = UARTReceive (  );
	while(1){
		laMemoire.ecriture(adresse, caractere);
		_delay_ms(5);
		adresse ++;
		caractere = UARTReceive (  );
	}
	return 0;
}
