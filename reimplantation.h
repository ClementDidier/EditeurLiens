#ifndef _REIMPLANTATION_
#define _REIMPLANTATION_

#include "API.h"

//ecrit à l'offset de c en parametre adresse
void ecrire(Elf32_Shdr_Content *c, int32_t result, Shdr_list *list);

//parametre content_list + info
void ARM(Elf32_Shdr_Content *rel_content, int info);

//Réimplantation des types R_ARM_ABS
//parametre shdr_list : rel_list
//appel arm pour chaque content_list avec info
void ARM_ABS(Shdr_list *rel_list);

#endif
