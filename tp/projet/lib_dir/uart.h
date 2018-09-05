#ifndef UART_H
#define UART_H


void initialisationUART ( void );

void transmissionUART ( uint8_t donnee );

/*
Transmission d'un entier sur 8bits de l'UART vers le PC.
*/

void transmissionEntierUART(uint8_t entier);
#endif
