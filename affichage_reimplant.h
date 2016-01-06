#ifndef __AFFICHAGE_REIMPLANT__
#define __AFFICHAGE_REIMPLANT__

#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <byteswap.h>
#include "API.h"

/* 
Affichage des tables de reimplantation
L'informations affiches:
	la cible (offset) de la reimplantation
	L'information de la reimplantation
*/
void creation_reimplantation(FILE* elfFile);

#endif
