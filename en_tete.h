#ifndef __EN_TETE__
#define __EN_TETE__

#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <byteswap.h>
#include "API.h"

/*
	Permet d'afficher l'en-tête du fichier ElfFile :
		- iterpretation du fichier
		- type de fichier
		- architecture nécessaire
		- version de fichier
		- adresse à laquelle le système transfère initialement le contrôle
		- déplacement en octets de la table d'en-tête de programme
		- déplacement en octets de la table des en-têtes de sections
		- drapeaux  spécifiques  au  processeur
		- taille de l'en-tête
		- taille en octets d'une entrée de la table d'en-tête de programme
		- nombre d'entrées de la table d'en-tête de programme
		- taille en octets d'un en-tête de section
		- nombre d'entrées de la table des en-têtes de sections
		- indice  dans  la table des en-têtes de sections de l'entrée associée à
			la table deschaînes de noms des sections
*/

void creation_en_tete(FILE* ElfFile) ; 

#endif
