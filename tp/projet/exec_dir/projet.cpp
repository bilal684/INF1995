
#define F_CPU 8000000

#include "moteur.h"
#include "memoire_24.h"
#include <util/delay.h>
#include <avr/io.h>
#include <stdint.h>
#include "uart.h"
#include "can.h"
#include "minuterie.h"
#include "sonnerie.h"
#include "lcm_so1602dtr_m_fw.h"
#include "customprocs.h"

#define DEMO_DDR	DDRC
#define DEMO_PORT	PORTC

//Déclaration des variables volatiles
volatile bool partir = false;
volatile float distanceCm = 0.0;
volatile float distanceInch = 0.0;
volatile uint16_t compteurTemps;
volatile bool tourneDroite;

////////////////////////////////////////////////////////////////////////////////////////////
// FONCTION:            initialisationProjet()
// DESCRIPTION:         Initialiser les entrées, sorties, et interruptions externes
// PARAMETRES:          Aucun.
// VALEUR DE RETOUR:    Aucune.
////////////////////////////////////////////////////////////////////////////////////////////
void initialisationProjet()
{
	cli();//Bloquer les interruptions
	EICRA |= _BV(ISC00);//Ajuster le registre EICRA afin de sensibiliser les interruptions externes aux rising-edges et falling-edges
	DDRA = 0x00; // PORT A en mode entree.
	DDRB = 0x40; //PORTB en mode entree sauf PINB6 en mode sortie.
	DDRC = 0xff; //PORTC en mode entree.
	DDRD = 0xff; //PORTD en mode sortie.
	//initialisationUART ();
	sei();//Repermettre les interruptions	
}

////////////////////////////////////////////////////////////////////////////////////////////
// FONCTION:            initInteruptPinB6()
// DESCRIPTION:         Initialiser l'interruption de la pin B6
// PARAMETRES:          Aucun.
// VALEUR DE RETOUR:    Aucune.
////////////////////////////////////////////////////////////////////////////////////////////
void initInteruptPinB6()
{
	cli();
	PCMSK1 |= _BV(PCINT15); //external interrupt mask registrer PCMSK1.
	PCICR |= _BV(PCIE1);
	sei();
}

////////////////////////////////////////////////////////////////////////////////////////////
// FONCTION:            ISR(TIMER0_COMPA_vect)
// DESCRIPTION:         Route d'interruption du Timer0
// PARAMETRES:          TIMER0_COMPA_vect
// VALEUR DE RETOUR:    Aucune.
////////////////////////////////////////////////////////////////////////////////////////////
ISR(TIMER0_COMPA_vect)
{
	compteurTemps++;
	EIFR |= (1 << INTF0);
}

////////////////////////////////////////////////////////////////////////////////////////////
// FONCTION:            ISR(PCINT1_vect)
// DESCRIPTION:         Route d'interruption de la pin B6
// PARAMETRES:          PCINT1_vect
// VALEUR DE RETOUR:    Aucune.
////////////////////////////////////////////////////////////////////////////////////////////
ISR(PCINT1_vect)
{
	if (partir == false)
	{
		partirMinuterie0(1);
		partir = true;
		compteurTemps = 0;
	}
	else
	{
		arreterMinuterie0();
		if (tourneDroite)
		{
			pivoterADroite(210 - (10 * compteurTemps));
			if (compteurTemps > 14)
				arreterMoteurs();
		}
		else if (!tourneDroite)
		{
			pivoterAGauche(210 - (10 * compteurTemps));
			if (compteurTemps > 14)
				arreterMoteurs();
		}
		partir = false;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
// FONCTION:            uint16_t max(uint16_t a, uint16_t b)
// DESCRIPTION:         Retourne la valeur maximale parmi celles données en paramètres
// PARAMETRES:          uint16_t a, uint16_t b
// VALEUR DE RETOUR:    a ou b
////////////////////////////////////////////////////////////////////////////////////////////
uint16_t max(uint16_t a, uint16_t b)
{
	if (a < b)
		return b;
	else
		return a;
}

////////////////////////////////////////////////////////////////////////////////////////////
// FONCTION:            uint16_t min(uint16_t a, uint16_t b)
// DESCRIPTION:         Retourne la valeur minimale parmi celles données en paramètres
// PARAMETRES:          uint16_t a, uint16_t b
// VALEUR DE RETOUR:    a ou b
////////////////////////////////////////////////////////////////////////////////////////////
uint16_t min(uint16_t a, uint16_t b)
{
	if (a < b)
		return a;
	else
		return b;
}

////////////////////////////////////////////////////////////////////////////////////////////
// FONCTION:            void changerVoieAGaucheSurCoin(uint8_t vitesse)
// DESCRIPTION:         Faire un angle de 90° vers la gauche
//						lorsque le robot atteint le sommet d'un parcours formant un angle
// PARAMETRES:          uint8_t vitesse
// VALEUR DE RETOUR:    Aucune.
////////////////////////////////////////////////////////////////////////////////////////////
void changerVoieAGaucheSurCoin(uint8_t vitesse) // version 2 a tester
{
	pivoterAGauche(vitesse);
	while(PINB & 0x1F)
	{}
	while (!(PINB & 0x02))
	{}
}

////////////////////////////////////////////////////////////////////////////////////////////
// FONCTION:            void changerVoieADroiteSurCoin(uint8_t vitesse)
// DESCRIPTION:         Faire un angle de 90° vers la droite
//						lorsque le robot atteint le sommet d'un parcours formant un angle
// PARAMETRES:          uint8_t vitesse
// VALEUR DE RETOUR:    Aucune.
////////////////////////////////////////////////////////////////////////////////////////////
void changerVoieADroiteSurCoin(uint8_t vitesse)
{
	pivoterADroite(vitesse);
	while(PINB & 0x1F)
	{}
	while (!(PINB & 0x08))
	{}
}

////////////////////////////////////////////////////////////////////////////////////////////
// FONCTION:            void changerVoieAGauche(uint8_t vitesse)
// DESCRIPTION:         Déplacer le robot vers la voie à sa gauche
// PARAMETRES:          uint8_t vitesse
// VALEUR DE RETOUR:    Aucune.
////////////////////////////////////////////////////////////////////////////////////////////
void changerVoieAGauche(uint8_t vitesse) // version 2 a tester
{
	pivoterAGauche(vitesse);
	while(PINB & 0x1F)
	{}
	while (!(PINB & 0x10))
	{}
	avancer(vitesse);
	while (PINB & 0x1F)
	{}
	pivoterADroite(vitesse);
	while (!(PINB & 0x08))
	{}
}

////////////////////////////////////////////////////////////////////////////////////////////
// FONCTION:            void changerVoieADroite(uint8_t vitesse)
// DESCRIPTION:         Déplacer le robot vers la voie à sa droite
// PARAMETRES:          uint8_t vitesse
// VALEUR DE RETOUR:    Aucune.
////////////////////////////////////////////////////////////////////////////////////////////
void changerVoieADroite(uint8_t vitesse)
{
	pivoterADroite(vitesse);
	while(PINB & 0x1F)
	{}
	while (!(PINB & 0x01))
	{}
	avancer(vitesse);
	while (PINB & 0x1F)
	{}
	pivoterAGauche(vitesse);
	while (!(PINB & 0x08))
	{}
}

////////////////////////////////////////////////////////////////////////////////////////////
// FONCTION:            depasserCroisementT(uint8_t vitesse) 
// DESCRIPTION:         Faire un angle de 90°
//						lorsque le robot atteint un croisement en T
// PARAMETRES:          uint8_t vitesse
// VALEUR DE RETOUR:    Aucune.
////////////////////////////////////////////////////////////////////////////////////////////
void depasserCroisementT(uint8_t vitesse) 
{
	avancer(vitesse);
	do{
	}while(PINB & 0x10);
	pivoterADroite(vitesse);
	do{
	}while(!(PINB & 0x02));
	do{
	}while((PINB & 0x02));
	do{
	}while(!(PINB & 0x02));
	avancer(vitesse);
}

////////////////////////////////////////////////////////////////////////////////////////////
// FONCTION:            void voiesParalleles(uint8_t vitesse)
// DESCRIPTION:         Instructions de la tâche 1
// PARAMETRES:          uint8_t vitesse
// VALEUR DE RETOUR:    Aucune.
////////////////////////////////////////////////////////////////////////////////////////////
void voiesParalleles(uint8_t vitesse)
{
	uint8_t voie = 3;
	bool coupureDepasseeSurVoieTrois = false;
	bool fin = false;
	bool lumiere = false;
	can donnee;
	uint16_t photoRD = donnee.lecture(0);
	photoRD = photoRD >> 2;
	uint16_t seuilLumiereADroite = min(photoRD + 30, 250) ;
	
	uint16_t photoRG = donnee.lecture(1);
	photoRG = photoRG >> 2;
	uint16_t seuilLumiereAGauche = min(photoRG + 30, 250) ;
	
	enum Etat {SUR_VOIE, DEVIE_A_GAUCHE, DEVIE_A_DROITE, CHANGEMENT_A_GAUCHE, CHANGEMENT_A_DROITE};
	Etat etat = SUR_VOIE;
	compteurTemps = 0;
	partirMinuterie0(20);
	do{
			
		photoRD = donnee.lecture(0);
		photoRD = photoRD >> 2;
		photoRG = donnee.lecture(1);
		photoRG = photoRG >> 2;
					
		if (!coupureDepasseeSurVoieTrois)
		{
			if (compteurTemps > 4000)
			{
				coupureDepasseeSurVoieTrois = true;
				arreterMinuterie0();
				compteurTemps = 0;
			}
		}
		else if (fin && compteurTemps > 400)
			arreterSonnerie();
		
		if(etat == SUR_VOIE && photoRG > seuilLumiereAGauche && photoRG + seuilLumiereADroite >= photoRD + seuilLumiereAGauche  && voie > 1 && voie <= 5 && !fin)
		{
			etat = CHANGEMENT_A_GAUCHE;
			lumiere = true;
		}
		else if(etat == SUR_VOIE && photoRD > seuilLumiereADroite && photoRD + seuilLumiereAGauche > photoRD + seuilLumiereADroite  && voie >= 1 && voie < 5 && !fin)
		{
			etat = CHANGEMENT_A_DROITE;
			lumiere = true;
		}
		else if(!(PINB & 0x04) && (PINB & 0x1F))
		{
			if (PINB & 0x03)
				etat = DEVIE_A_DROITE;
			else if (PINB & 0x18)
				etat = DEVIE_A_GAUCHE;
		}
		else if(!(PINB & 0x1F))
		{
			_delay_ms(10);
			if(!(PINB & 0x1F))
			{
				if(voie < 3)
					etat = CHANGEMENT_A_DROITE;
				else
				{
					etat = CHANGEMENT_A_GAUCHE;
					if (voie == 3 && coupureDepasseeSurVoieTrois)
					{
						fin = true;
						sonneriePWM(60);
						compteurTemps = 0;
						partirMinuterie0(20);
					}
				}			
			}							
			else
				etat = SUR_VOIE;
		}
		else
			etat = SUR_VOIE;
						
		// actions
		switch (etat)
		{
			case SUR_VOIE:
				avancer(vitesse);
				break;
			case DEVIE_A_GAUCHE:
				tournerADroite(vitesse, 20);
				break;
			case DEVIE_A_DROITE:
				tournerAGauche(vitesse, 20);
				break;
			case CHANGEMENT_A_GAUCHE:
				if (voie == 3 && fin && !lumiere)
				{
					tournerAGauche(vitesse);
					do{
					}while(!(PINB & 0x04));
				}
				else
					changerVoieAGauche(vitesse);
				--voie;
				lumiere = false;
				break;
			case CHANGEMENT_A_DROITE:
				changerVoieADroite(vitesse);
				lumiere = false;
				++voie;
				break;
		}
		
	}while(!fin || etat != SUR_VOIE || compteurTemps < 800 || !(PINB & 0x04));
	arreterMoteurs();
	arreterMinuterie0();
	compteurTemps = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////
// FONCTION:            void parcoursSegmente(uint8_t vitesse)
// DESCRIPTION:         Instructions de la tâche 2
// PARAMETRES:          uint8_t vitesse
// VALEUR DE RETOUR:    Aucune.
////////////////////////////////////////////////////////////////////////////////////////////
void parcoursSegmente(uint8_t vitesse)
{
	uint8_t nbreCoupuresDetectees = 0;
	uint8_t numSegment = 1;
	
	uint16_t delaisSegementUn = 1750;
	uint16_t delaisSegementDeux = 2300;
	uint16_t delaisUnSegementTrois = 1800;
	uint16_t delaisDeuxSegementTrois = 4000;
	uint16_t delaisSegementQuatre = 700;
	bool surCroisementT = false;
	bool dernierCroisementDepasse = false;
	bool croisementTDepasseSurSegmentTrois = false;
	bool finParcours = false;
	compteurTemps = 1;
	enum Etat {SUR_VOIE, HORS_VOIE, DEVIE_A_GAUCHE, DEVIE_A_DROITE};
	Etat etat = SUR_VOIE;
	do {
		
		if (numSegment == 3)
		{
			if (surCroisementT && !croisementTDepasseSurSegmentTrois && !(PINB & 0x10))
				croisementTDepasseSurSegmentTrois = true;
			else if ((PINB & 0x1C) == 0x1C &&  compteurTemps > delaisUnSegementTrois && !croisementTDepasseSurSegmentTrois)
				surCroisementT = true;
			else if ((PINB & 0x1C) == 0x1C &&  compteurTemps > delaisUnSegementTrois && croisementTDepasseSurSegmentTrois &&  !dernierCroisementDepasse)
			{
				depasserCroisementT(vitesse);
				dernierCroisementDepasse = true;
			}
			else if ((PINB & 0x7) == 0x7 &&  compteurTemps > delaisDeuxSegementTrois)
			{
				avancer(vitesse);
				do{
				}while(PINB & 0x1F);
				changerVoieAGaucheSurCoin(vitesse);
				++numSegment;
				compteurTemps = 0;
				partirMinuterie0(20);
			}
		}
		else if (numSegment == 4 && ((PINB & 0x1C) == 0x1C || (PINB & 0x7) == 0x7) && compteurTemps > delaisSegementQuatre && !finParcours)
		{
			finParcours = true;
			compteurTemps = 0;
		}
		
		if((!(PINB & 0x04)) && (PINB & 0x1F))
		{
			if (PINB & 0x03)
				etat = DEVIE_A_DROITE;
			else if (PINB & 0x18)
				etat = DEVIE_A_GAUCHE;
		}
		else if(!(PINB & 0x1F))
		{
			_delay_ms(10);
			if(!(PINB & 0x1F))
			{
				etat = HORS_VOIE;
				++nbreCoupuresDetectees;							
			}							
			else
				etat = SUR_VOIE;
		}
		else
			etat = SUR_VOIE;
			
		// actions
		switch (etat)
		{
			case SUR_VOIE:
				if (numSegment == 1 && compteurTemps > delaisSegementUn)
				{
					changerVoieAGaucheSurCoin(vitesse);
					++numSegment;
					compteurTemps = 0;
					partirMinuterie0(20);
				}
				else 
					avancer(vitesse);
				break;
			case HORS_VOIE:
				if (numSegment == 1 )
				{
					if (nbreCoupuresDetectees <= 3)
					{
						avancer(vitesse);
						do{
						}while(!(PINB & 0x1F));
						if (nbreCoupuresDetectees == 3)
						{
							compteurTemps = 0;
							partirMinuterie0(20);
						}
					}	
					else if (nbreCoupuresDetectees == 4)
						changerVoieAGaucheSurCoin(vitesse);
				}
				else if (numSegment == 2)
				{
					if (compteurTemps < delaisSegementDeux)
						changerVoieADroiteSurCoin(vitesse);
					else if(compteurTemps > delaisSegementDeux)	
					{
						_delay_ms(300);
						tournerAGauche(vitesse, 50);
						do{
						}while(!(PINB & 0x04));
						++numSegment;
						compteurTemps = 0;
						partirMinuterie0(20);
					}
				}
				else if (numSegment == 3)
				{
					if (compteurTemps < delaisUnSegementTrois)
						changerVoieADroiteSurCoin(vitesse);
					else if(compteurTemps > delaisDeuxSegementTrois)
					{
						changerVoieAGaucheSurCoin(vitesse);
						++numSegment;
						compteurTemps = 0;
						partirMinuterie0(20);
					}
				}
				break;
			case DEVIE_A_GAUCHE:
				tournerADroite(vitesse, 20);
				break;
			case DEVIE_A_DROITE:
				tournerAGauche(vitesse, 20);
				break;
		}
	}while(!finParcours || compteurTemps < 400);
	arreterMinuterie0();
	compteurTemps = 0;
	arreterMoteurs();
}
////////////////////////////////////////////////////////////////////////////////////////////
// FONCTION:            void lancerSonar()	
// DESCRIPTION:         Déclencher le Trigger Pulse du sonar
// PARAMETRES:          Aucun.
// VALEUR DE RETOUR:    Aucune.
////////////////////////////////////////////////////////////////////////////////////////////
void lancerSonar()	
{
	if(partir == false)
	{
		_delay_ms(50);
		PORTB |= 0x40;
		_delay_us(10);
		PORTB &= ~(0x40);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
// FONCTION:            void rotationEtSons()
// DESCRIPTION:         Instructions de la tâche 3
// PARAMETRES:          Aucun.
// VALEUR DE RETOUR:    Aucune.
////////////////////////////////////////////////////////////////////////////////////////////
void rotationEtSons()
{
	_delay_ms(1000);
	sonneriePWM(45);
	_delay_ms(2000);
	arreterSonnerie();
	initInteruptPinB6();
	tourneDroite = true;
	for(uint16_t i = 0;i < 230;i++)
	{
		lancerSonar();
		if((PINB & 0b00000001)){
			sonneriePWM(60);
		}
		else if ((PINB & 0b00000010)){
			sonneriePWM(62);
		}
		else if ((PINB & 0b00000100)){
			sonneriePWM(64);
		}
		else if ((PINB & 0b00001000)){
			sonneriePWM(65);
		}
		else if ((PINB & 0b00010000)){
			sonneriePWM(67);
		}
		else if (!(PINB & 0b00011111))
		{
			arreterSonnerie();
		}
	}
	arreterSonnerie();
	tourneDroite = false;
	for(uint16_t i = 0;i < 230;i++)
	{
		lancerSonar();
		if((PINB & 0b00000001)){
			sonneriePWM(60);
		}
		else if ((PINB & 0b00000010)){
			sonneriePWM(62);
		}
		else if ((PINB & 0b00000100)){
			sonneriePWM(64);
		}
		else if ((PINB & 0b00001000)){
			sonneriePWM(65);
		}
		else if ((PINB & 0b00010000)){
			sonneriePWM(67);
		}
		else if (!(PINB & 0b00011111))
		{
			arreterSonnerie();
		}

	}
	arreterSonnerie();
	arreterMoteurs();
}

////////////////////////////////////////////////////////////////////////////////////////////
// FONCTION:            void selectionnerUneOption(uint8_t option)
// DESCRIPTION:         Permet de determiner par quelle tâche débuter
//						en modifiant la valeur du paramètre option entre 1 et3.
// PARAMETRES:          uint8_t option
// VALEUR DE RETOUR:    Aucune.
////////////////////////////////////////////////////////////////////////////////////////////
uint8_t selectionnerUneOption(uint8_t option)
{
	//Déclaration de l'objet disp de type LCM, permettant la gestion de l'écran LCD
	LCM disp(&DEMO_DDR, &DEMO_PORT);
	
	//Afficher un message de bienvenue au début de l'exprérience
	disp.write("JABA!!", 5);
	disp.write("Bienvenue!",19);
	
	do{
		//Afficher la tache suivante lorsque le bouton de selection est presse
		if(!(PINA & 0x04))
		{
			//La selection doit se faire entre 1 et 3
			_delay_ms(10);
			if(!(PINA & 0x04))
			{
				option++;
				if(option > 3)
					option = 1;
				//Effacer la tache precedente de l'ecran
				disp.clear();
				switch(option)
				{
					//Affichage du nom de la tâche sélectionnée, puis anti-rebond.
					case 1:
						disp.write("1) 5 voies", 0); _delay_ms(250);break;
					case 2:
						disp.write("2) Parcours",0);_delay_ms(250); break;
					case 3:
						disp.write("3) Rotation/sons", 0);_delay_ms(250); break;
					default:
						disp.write("Erreur", 0);_delay_ms(250); break;
				}
			}
		}
	}while(PINA & 0x08);
	return option;
}

