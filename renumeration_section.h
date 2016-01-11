#ifndef _RENUMERATION_SECTION_
#define _RENUMERATION_SECTION_

#include "API.h"

// modifie la structure des en-têtes de section en supprimant les REl et RELA
// modifie le tableau des indices de sections
void enlever_relocation(Elf32_Ehdr h, Shdr_list * l, Shdr_list * rl, int * num_sections);

//affiche le tableau num_sections (très util pour faire de tests)
void afficher_tableau_sections(int * num_sections, int size_num_sections);


#endif
