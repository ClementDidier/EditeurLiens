// MODULE DE FONCTIONS UTILES POUR LIRE DES DONNEES
#ifndef _API_
#define _API_

#include <elf.h>
#include <stdio.h>

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


#endif
