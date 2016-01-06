// MODULE DE FONCTIONS UTILES POUR LIRE DES DONNEES
#ifndef _API_
#define _API_

#include <elf.h>
#include <stdio.h>
#include <stdlib.h>

// Structures de données
typedef struct Shdr_list Shdr_list;
struct Shdr_list{
	Elf32_Shdr header;
	Shdr_list * next;
};

Shdr_list *shdr_list;

// Fonctions d'inversion d'endianness little->big ou big->little
// Travaillent directement sur la mémoire 
void l2b_endian_32( unsigned int * val );

void l2b_endian_16( unsigned int * val );

// Fonctions de lecture des valeurs codees en big endian vers une architecture little endian 
// Ces fonctions renvoient le nombre d'octets lus

// Lis le header du fichier ELF
int read_Elf32_Ehdr( FILE *f, Elf32_Ehdr * h );
// Lis un header de section 
int read_Elf32_Shdr( FILE *f, Elf32_Ehdr h, unsigned int index, Elf32_Shdr * s);
// Lis une structure Elf32_Sym
int read_Elf32_Sym( FILE *f, Elf32_Sym *s );
// Construit l'ensemble des Shdr :
void read_Shdr_list( FILE *f );
void afficher_Shdr_list();
// Lis une structure Elf32_Rela (lis une ligne du tableau de relocation)
int read_Elf32_Rela( FILE *f, Elf32_Rela *ra, int indice, Elf32_Shdr s);
// Lis une structure Elf32_Rel (lis une ligne du tableau de relocation)
int read_Elf32_Rel( FILE *f, Elf32_Rel *r, int indice, Elf32_Shdr s);


#endif
