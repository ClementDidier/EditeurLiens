// MODULE DE FONCTIONS UTILES POUR LIRE DES DONNEES
#ifndef _API_
#define _API_

#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <byteswap.h>

// Structures de données
Elf32_Ehdr header;

typedef struct Shdr_list Shdr_list;
struct Shdr_list{
	Elf32_Shdr header;
	unsigned char * dump;
	Shdr_list * next;
};

Shdr_list *shdr_list;
Shdr_list *rel_list;

int *num_sections;
int size_num_sections;

typedef struct Sym_list Sym_list;
struct Sym_list{
	Elf32_Sym * list;
	int nb;
};

Sym_list sym_list; 

// Fonctions d'inversion d'endianness little->big ou big->little
// Travaillent directement sur la mémoire 
void l2b_endian_32( unsigned int * val );

void l2b_endian_16( unsigned int * val );

// Fonctions de lecture des valeurs codees en big endian vers une architecture little endian 
// Ces fonctions renvoient le nombre d'octets lus ( le plus souvent )

// Lis le header dans la variable globale
int read_header( FILE *f );
// Lis le header du fichier ELF dans une structure 
int read_Elf32_Ehdr( FILE *f, Elf32_Ehdr * h );
void afficher_Elf32_Ehdr( Elf32_Ehdr h );
// Lis un header de section 
int read_Elf32_Shdr( FILE *f, Elf32_Ehdr h, unsigned int index, Elf32_Shdr * s);
// Lis une structure Elf32_Sym
int read_Elf32_Sym( FILE *f, Elf32_Sym *s );
// Lis une structure Elf32_Rela (lis une ligne du tableau de relocation)
// l'indice correspond a la relocation voulue dans la section dans ( numero 0, 1, 2, 3, ... )
int read_Elf32_Rela( FILE *f, Elf32_Rela *ra, int indice, Elf32_Shdr s);
// Lis une structure Elf32_Rel (lis une ligne du tableau de relocation)
int read_Elf32_Rel( FILE *f, Elf32_Rel *r, int indice, Elf32_Shdr s);

// Fonctions de construction et affichage de la structure de donnees sur laquelle on va travailler 

// Construit l'ensemble des Sections, une liste chainee de structures qui contient à chaque fois le
// header et le dump d'une section :
void read_Shdr_list( FILE *f );
void afficher_Shdr_list();

void read_Sym_list( FILE *f );
void afficher_Sym_list();

// Fonction d'ecriture de nos structures vers un fichier ELF resultat 

// Ecrit le header dans le stream
void write_Elf32_Ehdr(FILE *f, Elf32_Ehdr h);
// Ecrit un header de section dans un fichier 
void write_Elf32_Shdr(FILE *f, Elf32_Ehdr h, unsigned int index, Elf32_Shdr s);
// Ecrit un dump de section dans un fichier
void write_dump( FILE * f,  unsigned char * dump, Elf32_Word size);

// Ecrit l'ensemble des sections
// D'abord les dumps, puis les headers correspondants
void write_Shdr_list( FILE *f);


// Autre : 
// Renvoie un pointeur vers le debut de la Shdr_list correspondant au numero de section 
Shdr_list * find_section( int num );
#endif
