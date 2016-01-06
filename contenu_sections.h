#ifndef _CONTENU_SECTIONS_
#define _CONTENU_SECTION_

//Procédure d'inversion de big endian sur un Header (en tête)
//Attention ne swap qu'une partie des valeurs
void bytes_swap_elfHeader(Elf32_Ehdr *elfHeader);

//Procédure d'inversion de big endian sur un Header de section
//Attention ne swap qu'une partie des valeurs
void bytes_swap_elfSections(Elf32_Shdr *elfSections)

//Fonction qui afficher à l'écran le contenu de la section une fois trouvée
void ecrire_contenu(FILE *f,char strg[N],int size);

#endif
