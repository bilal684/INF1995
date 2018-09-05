#ifndef PROJET_H 
#define PROJET_H

void initialisationProjet();
void voiesParalleles(uint8_t vitesse);
void parcoursSegmente(uint8_t vitesse);
void initInteruptPinB6();
void lancerSonar();
void rotationEtSons();
uint8_t selectionnerUneOption(uint8_t option);

#endif
