////////////////////////////////////////////////////////////////////////////////////////////
// FONCTION:            int main()
// DESCRIPTION:         Contient l'execution du programme principal.
// PARAMETRES:          Aucun.
// VALEUR DE RETOUR:    Aucune.
// BRANCHEMENTS:		-Moteurs:
//						 Moteur droit: fil orange connecté entre les pins Disable droite et D3
//									   fil jaune connecté entre les pins Enable droite et D5
//						 Moteur droit: fil orange connecté entre les pins Disable droite et D2
//									   fil jaune connecté entre les pins Enable droite et D4
//						-Boutons poussoirs:
//						 Bouton de sélection (fil noir) connecté à la pin A2
//						 Bouton de confirmation (fil vert) connecté à la pin A3
//						-Photorésistances:
//						 Photorésistance gauche (fil brun) connectée à la pin B0
//						 Photorésistance droite (fil blanc) connectée à la pin B1
//						-Afficheur connecté au port C
//						-Suiveur de ligne connecté aux pins B0à5 et à son VCC/GND
//						-Sonar:
//						 Alimentation connecté à VCC/GND du port D
//						 Trigger Input (fil bleu) connecté à la pin B6
//						 Echo Output (fil jaune) connecté à la pin B7
//						-Piézoélectrique  connecté aux pins D6&7
////////////////////////////////////////////////////////////////////////////////////////////
#define F_CPU 8000000
#include <util/delay.h>
#include "projet.h"
#include "moteur.h"
#include "minuterie.h"
#include <avr/io.h>
#include <stdint.h>
#include "uart.h"
#include "lcm_so1602dtr_m_fw.h"

#define DEMO_DDR	DDRC
#define DEMO_PORT	PORTC

int main()
{
	uint8_t vitesse = 120;
	uint8_t option = 0;
	LCM disp(&DEMO_DDR, &DEMO_PORT);
	
	initialisationProjet();

	//Permettre la défilation des différentes taches tant que le bouton de confirmation n'a pas ete presse
	option = selectionnerUneOption(option);
	_delay_ms(1000);
	switch(option)
	{
		//Effectuer successivement toutes les tâches à partir de celle sélectionnée
		case 1:
			disp.clear();
			disp.write("1) 5 voies", 0);
			voiesParalleles(vitesse);
		case 2:
			disp.clear();
			disp.write("2) Parcours",0);
			parcoursSegmente(vitesse);
		case 3: 
			disp.clear();
			disp.write("3) Rotation/sons", 0);
			rotationEtSons();
			break;
		default:
			break;
	}

}
