#ifndef _RENUMERATION_SECTION_
#define _RENUMERATION_SECTION_

#include "API.h"



// modifie la structure des en-têtes de section en supprimant les REl et RELA
// modifie le tableau des indices de sections
void enlever_relocation();


//affiche le tableau num_sections (très util pour faire de tests)
void afficher_tableau();


#endif
