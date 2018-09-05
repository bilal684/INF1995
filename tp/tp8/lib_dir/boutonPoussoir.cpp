#include "boutonPoussoir.h"

/*
 *Cette fonction permet de sensibiliser les interruptions externes 
 * du bouton poussoir à ces fronts montant et/ou descendant
 */
void initialisationBouton(bool risingEdge, bool fallingEdge)
{
  cli();//Bloquer les interruptions
  EIMSK |= _BV(INT0);//Ajuster le registre EIMSK afin de permettre les interruptions externes au moyen du bouton poussoir
  //Ajuster le registre EICRA afin de sensibiliser les interruptions externes:
  if(!risingEdge && fallingEdge)//aux falling edges
    EICRA |= _BV(ISC01);
  else if(risingEdge && !fallingEdge)//aux rising edges
    EICRA |= _BV(ISC01) | _BV(ISC00);
  else if(risingEdge && fallingEdge)//aux deux
    EICRA |= _BV(ISC00);
  sei();//repermettre les interruptions
}