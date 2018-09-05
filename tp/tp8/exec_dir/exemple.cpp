/*
 * Exemple.cpp
 * Description: En utilisant la librarie statique nouvellement créée,
 * ce programme allume la LED de couleur différente, selon si
 * le bouton poussoir est préssé ou pas.
 * Table des états:
 *  État | Entrée(bouton) | État suivant | Sortie(couleur de la LED)
 *  1	 |	false	  |	1	 |		Rouge
 *  1	 |	true	  |	2	 |		Rouge
 *  2	 |	false	  |	1	 |		Verte
 *  2	 |	true	  |	2	 |		Verte
 * Branchements:
 * pin B2 connectée au Del Libre +; pin B1 connectée au Del Libre -
 */

#include "boutonPoussoir.h"

volatile bool bouton = false;

ISR(INT0_vect)
{
	bouton = !(bouton);
	EIFR |= (1 << INTF0);
}

int main()
{
	//Initialisation des ports: B en sortie et D en entrée
	DDRB = 0xff;
	DDRD = 0x00;
	//Sensibilisation du bouton poussoir aux fronts montant et sortant
	initialisationBouton(true, true);
	while(true)//boucle infinie
	{
		if(bouton)//Si le bouton est préssé (État 2)
			PORTB = 0x1;//Allumer la LED en vert
		else//sinon (État 1)
			PORTB = 0x2;//Allumer la LED en rouge
	}
}
