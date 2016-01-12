// MODULE DE FONCTIONS UTILES POUR LIRE DES DONNEES
#ifndef _API_
#define _API_

#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <byteswap.h>
#include <string.h>

// Position de la valeur définissant l'endianess dans le tableau e_ident du header
#define ENDIANESS_VALUE_INDEX 5

// *************************************************************************************************************
// *************************************** Structures de données ***********************************************
// *************************************************************************************************************
typedef struct Shdr_list Shdr_list;
struct Shdr_list{
	Elf32_Shdr header;
	unsigned char * dump;
	Shdr_list * next;
};

typedef struct Sym_list Sym_list;
struct Sym_list{
	Elf32_Sym * list;
	int nb;
};

// Structure de données pour un accés simple aux données d'un dump
typedef struct Elf32_Shdr_Content Elf32_Shdr_Content;
struct Elf32_Shdr_Content
{
	Elf32_Addr offset;
	Elf32_Word info;
	int32_t sym;
	unsigned char type;
	Elf32_Shdr_Content * next;
};


// *************************************************************************************************************
// *************************** Fonctions d'inversion et de test de l'endianess *********************************
// *************************************************************************************************************
// Travaillent directement sur la mémoire 
// Deprecated
//void l2b_endian_32( unsigned int * val );
//void l2b_endian_16( unsigned int * val );

// Retourne 1 si l'endianess du header est en big endian, 0 dans le cas contraire
int is_big_endian(Elf32_Ehdr h);

int32_t recuperer_va//Convertit une chaine de caractères en valeur hexadécimale
int char_to_hex(char *strg);leur32(Elf32_Ehdr h, int32_t value);
int16_t recuperer_valeur16(Elf32_Ehdr h, int16_t value);

// *************************************************************************************************************
// ********* Fonctions de lecture des valeurs codees en big endian vers une architecture little endian *********
// *************************************************************************************************************
// Ces fonctions renvoient le nombre d'octets lus ( le plus souvent )

// Lis le header dans la variable globale
// Deprecated
//int read_header( FILE *f );
// Lis le header du fichier ELF dans une structure 
int read_Elf32_Ehdr( FILE *f, Elf32_Ehdr * h );
void afficher_Elf32_Ehdr( Elf32_Ehdr h );
// Lis un header de section 
int read_Elf32_Shdr( FILE *f, Elf32_Ehdr h, unsigned int index, Elf32_Shdr * s);
// Lis une structure Elf32_Sym
int read_Elf32_Sym( FILE *f, Elf32_Ehdr h, Elf32_Sym *s );
// Lis une structure Elf32_Rela (lis une ligne du tableau de relocation)
// l'indice correspond a la relocation voulue dans la section dans ( numero 0, 1, 2, 3, ... )
int read_Elf32_Rela( FILE *f, Elf32_Ehdr h, Elf32_Rela *ra, int indice, Elf32_Shdr s);
// Lis une structure Elf32_Rel (lis une ligne du tableau de relocation)
int read_Elf32_Rel( FILE *f, Elf32_Ehdr h, Elf32_Rel *r, int indice, Elf32_Shdr s);


// *************************************************************************************************************
// ****** Fonctions de construction et affichage de la structure de donnees sur laquelle on va travailler ******
// *************************************************************************************************************
// Construit l'ensemble des Sections, une liste chainee de structures qui contient à chaque fois le
// header et le dump d'une section :
void read_Shdr_list(FILE *f, Elf32_Ehdr h, Shdr_list * l);
void afficher_Shdr_list(Shdr_list * l);
void afficher_Shdr(Shdr_list * l);

void read_Sym_list(FILE *f, Elf32_Ehdr h, Sym_list * l);
void afficher_Sym_list(Sym_list l);
void afficher_Sym(Elf32_Sym S);

// Charge le contenu d'une section dans une structure de données personnalisée
void read_Elf32_Shdr_Content(Shdr_list *s, unsigned int index, Elf32_Shdr_Content * cp);
void afficher_Elf32_Shdr_Content(Elf32_Shdr_Content c);

// *************************************************************************************************************
// ***************** Fonction d'ecriture de nos structures vers un fichier ELF resultat ************************
// *************************************************************************************************************

inline void fwrite_value16(FILE * f, Elf32_Ehdr h, int value, int size);
inline void fwrite_value32(FILE * f, Elf32_Ehdr h, int value, int size);

// Ecrit le header dans le stream
void write_Elf32_Ehdr(FILE *f, Elf32_Ehdr h);
// Ecrit un header de section dans un fichier 
void write_Elf32_Shdr(FILE *f, Elf32_Ehdr h, unsigned int index, Elf32_Shdr s);
// Ecrit un dump de section dans un fichier
void write_dump( FILE * f,  unsigned char * dump, Elf32_Word size, Elf32_Off offset);
// Ecrit un symbole dans un fichier
void write_Elf32_Sym(FILE * f, Elf32_Ehdr h, Elf32_Sym s);

// Ecrit l'ensemble des sections
// D'abord les dumps, puis les headers correspondants
void write_Shdr_list(FILE * f, Elf32_Ehdr h, Shdr_list * l);

// Ecrit l'ensemble des symboles
void write_Sym_list(FILE * f, Elf32_Ehdr h, Sym_list l);

// Autre : 
// Renvoie un pointeur vers le debut de la Shdr_list correspondant au numero de section 
Shdr_list * find_section(int num, Shdr_list * l);
Shdr_list * find_section_name(char **names,char *name, Shdr_list *l);
Shdr_list * find_symbols_section(Shdr_list * l);

// Obtient la table des strings
char ** sections_names_table(FILE * f, Elf32_Ehdr h);

//Convertit une chaine de caractères en valeur hexadécimale
int char_to_hex(char *strg);
#endif
