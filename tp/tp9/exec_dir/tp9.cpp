#define F_CPU 8000000

#include "moteur.h"
#include "memoire_24.h"
#include <util/delay.h>
#include <avr/io.h>
#include <stdint.h>
#include <math.h>
#include "uart.h"
#include "minuterie.h"
#include "sonnerie.h"

# define vitesse 127

volatile bool minuterie0Expiree = false;
volatile bool minuterie2Son = false;
volatile float frequenceSon = 0;
volatile uint8_t pourcentageSon = 0;
volatile bool sonActif = false;

void initialisationGeneral()
{
	cli();//Bloquer les interruptions
	EICRA |= _BV(ISC00);//Ajuster le registre EICRA afin de sensibiliser les interruptions externes aux rising-edges et falling-edges
	DDRD = 0xff;
	DDRB = 0xff;
	DDRC = 0xff;
	initialisationUART ( );
	sei();//Repermettre les interruptions	
}

ISR(TIMER0_COMPA_vect)
{
	minuterie0Expiree = true;
}

int main()
{
	initialisationGeneral();
	uint8_t instruction = 0;
	uint8_t operande = 0;
	uint8_t adresse = 0;
	uint8_t adresseDebutBoucle = 0;
	uint8_t premierOctet = 0;
	uint16_t nbOctets = 0;
	uint8_t iteration = 0;

	Memoire24CXXX laMemoire;

	laMemoire.lecture(adresse, &premierOctet);
	//_delay_ms(5);	
	adresse ++;
	nbOctets = premierOctet;
	transmissionEntierUART(premierOctet);
	transmissionUART('\n');
		
	laMemoire.lecture(adresse, &premierOctet);
	adresse ++;
	//_delay_ms(5);	
	transmissionEntierUART(premierOctet);
	transmissionUART('\n');			
	
	nbOctets <<= 8;
	nbOctets |= premierOctet;
	transmissionEntierUART(nbOctets);
	transmissionUART('\n');

	// tant que on a pas debut on lit la memoire	
	do
	{
		laMemoire.lecture(adresse, &instruction);
		_delay_ms(5);			
		adresse++;			
		
		transmissionEntierUART(instruction);
		transmissionUART('\n');		
		
		laMemoire.lecture(adresse, &operande);
		_delay_ms(5);
		adresse++;			
		
		transmissionEntierUART(operande);
		transmissionUART('\n');		
	}while(instruction != 0x01);

	for(uint8_t i = 0; i < 5; i++)
	{
		
		PORTB = ( PORTB & 0xFE ); // PORTB0 a 0
		PORTB = ( PORTB | 0x02 ); // PORTB1 a 1
		_delay_ms(100);
		PORTB = ( PORTB | 0x01 ); // PORTB0 a 1
		PORTB = ( PORTB & 0xFD ); // PORTB1 a 0
		_delay_ms(100);
	}
	transmissionUART('L');
	transmissionUART('\n');

	// on commence ici le programme	
	for ( ; instruction != 0xFF && nbOctets != 0 ; nbOctets-=2)
	{
		//lecture de l'instruction suivante et son operande		
		laMemoire.lecture(adresse, &instruction);
		_delay_ms(5);			
		adresse++;			
		
		transmissionEntierUART(instruction);
		transmissionUART('\n');		

		laMemoire.lecture(adresse, &operande);
		_delay_ms(5);
		adresse++;
			
		transmissionEntierUART(operande);
		transmissionUART('\n');
		
		// execution du code correspondant a l'instruction
		switch (instruction){
			case 0x02 : //att attendre. 10 attend 250 ms
				transmissionUART('A');
				transmissionUART('\n');
				minuterie0Expiree = false;
				partirMinuterie0(25 * operande);
				do 
				{
				}
				while(!minuterie0Expiree);
				transmissionUART('F');
				transmissionUART('\n');
				break; 
			case 0x44 : //dal allumer la del on ignore l'operande car on a un seul del
				PORTB |= 1; // PORTB0 a 1
				PORTB &= ~(1 << 1); // PORTB1 a 0
				break; 
			case 0x45 : //det eteindre la del
				PORTB &= ~1; // PORTB0 a 0
				PORTB &= ~(1 << 1); // PORTB1 a 0
				break; 
			case 0x48 : //sgo jouer une sonorite
				frequenceSon = 110 * pow(2,(operande - 45)/12);
				pourcentageSon = (frequenceSon * 0xFF) / 880;
				sonneriePWM(pourcentageSon);
				break;
				
			case 0x09 : 
				arretSonnerie(); break;
			case 0x60 : //mar arreter les moteurs.
				ajustementPWM(255, 255); 
				break; 
			case 0x61 : //mar arreter les moteurs.
				ajustementPWM(255, 255); 
				break; 
			case 0x62 : //mav avancer.
				transmissionUART('V');
				transmissionUART('\n');
				avancer(operande); 
				_delay_ms(1000);
				break; 
			case 0x63 : //mre reculer.
				transmissionUART('R');
				transmissionUART('\n');
 				reculer(operande);
				_delay_ms(1000);
				break;
			case 0x64 :  //trd tourner a droite.
				transmissionUART('D');
				transmissionUART('\n');
				tournerADroite(vitesse);				
				_delay_ms(1500);
				ajustementPWM(255, 255);
				transmissionUART('d');
				transmissionUART('\n');
				break;
			case 0x65 :  //trg tourner a gauche.
				transmissionUART('G');
				transmissionUART('\n');
				tournerAGauche(vitesse);				
				_delay_ms(1500);
				ajustementPWM(255, 255);
				transmissionUART('g');
				transmissionUART('\n');
				break;
			case 0xC0 : //dbc debut de boucle.
				iteration = operande + 1;
				adresseDebutBoucle = adresse;				
				break;		
			case 0xC1 : //fbc fin de boucle.
				if (iteration != 0)
				{
					iteration--;
					adresse = adresseDebutBoucle;
				}
				break;
			default :
				break;
		}	
	}
	return 0;
}
