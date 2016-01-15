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



// *************************************************************************************************************
// *************************** Fonctions d'inversion et de test de l'endianess *********************************
// *************************************************************************************************************
// Travaillent directement sur la mémoire 
// Deprecated
//void l2b_endian_32( unsigned int * val );
//void l2b_endian_16( unsigned int * val );

// Retourne 1 si l'endianess du header est en big endian, 0 dans le cas contraire
int my_is_big_endian(Elf32_Ehdr h);

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

// *************************************************************************************************************
// ****** Fonctions de construction et affichage de la structure de donnees sur laquelle on va travailler ******
// *************************************************************************************************************
// Construit l'ensemble des Sections, une liste chainee de structures qui contient à chaque fois le
// header et le dump d'une section :
void read_Shdr_list(FILE *f, Elf32_Ehdr h, Shdr_list * l);
void afficher_Shdr_list(Shdr_list * l);
void afficher_Shdr(Shdr_list * l);

Sym_list read_Sym_list(FILE *f, Elf32_Ehdr h, Sym_list * l, Shdr_list sl, unsigned char **names);
void afficher_Sym_list(Sym_list l, FILE *f, Elf32_Ehdr h, unsigned char **names,Shdr_list *s);
void afficher_Sym(Elf32_Sym S, FILE *f, Elf32_Ehdr h, unsigned char **names,Shdr_list *s);


//Affichage qui correspond à celui du readelf

void afficher_reimplantation(Elf32_Ehdr h, Shdr_list * shdr_list, unsigned char ** names);
void afficher_section(Shdr_list *s,unsigned char **names,int x);
void afficher_section_name(Shdr_list *s,unsigned char **names,char *name);
//Afficher l'en tête du fichier en format présentable
void afficher_en_tete(Elf32_Ehdr h);


// *************************************************************************************************************
// ***************** Fonction d'ecriture de nos structures vers un fichier ELF resultat ************************
// *************************************************************************************************************

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
Shdr_list * find_section_name(unsigned char **names,char *name, Shdr_list *l);
Shdr_list * find_symbols_section(Shdr_list * l);

// Obtient la table des strings
unsigned char ** sections_names_table(FILE * f, Elf32_Ehdr h);

// *************************************************************************************************************
// ***************************** Fonctions de libérationdes structures utilisées *******************************
// *************************************************************************************************************

void liberer_Shdr_list(Shdr_list *sl);
void liberer_Sym_list(Sym_list *sl);
void liberer_tab_name(unsigned char ** names, int h_shnum);
void liberer_num_sections(int * num_sections);

#endif
