#define T_PIEZO 4.45
#define A_PIEZO 2.27
#include <avr/io.h>
#include "minuterie.h"
#include "moteur.h"
#include "memoire_24.h"

volatile bool minuterie0Expiree;
volatile bool minuterie1Expiree;

ISR(TIMER0_COMPA_vect)
{
	minuterie0Expiree = true;
}

ISR(TIMER1_COMPA_vect)
{
	minuterie1Expiree = !(minuterie1Expiree);
	if(minuterie1Expiree){
		PORTC = 0x1;
		partirMinuterie1(A_PIEZO);
		}
	else{
		PORTC = 0x0;
		partirMinuterie1(calculDelai(T_PIEZO);
		}
}

ISR(TIMER2_COMPA_vect)
{
	ajustementPWM(0,0);
}


void initialisation()
{
	cli();//Bloquer les interruptions
	EICRA |= _BV(ISC00);//Ajuster le registre EICRA afin de sensibiliser les interruptions externes aux rising-edges et falling-edges
	sei();//Repermettre les interruptions
}

int main()
{
	//Initialisation des pins: B et C en sortie
	DDRB = 0xff;
	DDRC = 0xff;
	
	//Initialisation des interruptions externes
	initialisation();
	
	uint8_t instruction = 0;
	uint8_t operande = 0;
	uint8_t addresse = 0;
	uint8_t vitesse = 0;
	Memoire24CXXX laMemoire;
	for (;instruction != 0xFF;)
	{
		laMemoire.lecture(addresse, &instruction);
		addresse++;
		laMemoire.lecture(addresse, &operande);
		addresse++;
		switch (instruction){
		case 0x01 : break; //dbt debut de programme.
		case 0x02 : //att attendre.
			minuterie0Expiree = false;
			partirMinuterie0(25 * operande);
			do {} while(!minuterie0Expiree);
			break; 
		case 0x44 : //dal allumer la del
			if(operande == 0x01)//en rouge
				PORTB = 0x2;
			else //en vert
				PORTB = 0x1;
			break; 
		case 0x45 : //det eteindre la del
			PORTB = 0x0;
			break; 
		case 0x48 : //sgo jouer une sonorite
			PORTC = 0x0;
			minuterieExpiree = false;
			partirMinuterie(calculDelai(T_880, 1024));
		break;
		case 0x09 : break; //sar arreter de jouer la sonorite.
		case 0x60 : break; //mar arreter les moteurs.
		case 0x61 : break; //mar arreter les moteurs.
		case 0x62 : break; //mav avancer.
		case 0x63 : break; //mre reculer.
		case 0x64 : break; //trd tourner a droite.
		case 0x65 : break; //trg tourner a gauche.
		case 0xC0 : break; //dbc debut de boucle.
		case 0xC1 : break; //fbc fin de boucle.
		case 0xFF : break; //fin fin de programme.
		}	
	}
	
}
