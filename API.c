#include "API.h"



// Fonctions d'inversion d'endianness little->big ou big->little
// Travaillent directement sur la mémoire 
void l2b_endian_32( unsigned int * val ){
	uint32_t b0,b1,b2,b3;
	uint32_t num = *val;
	b0 = (num & 0x000000ff) << 24u;
	b1 = (num & 0x0000ff00) << 8u;
	b2 = (num & 0x00ff0000) >> 8u;
	b3 = (num & 0xff000000) >> 24u;
	*val = b0 | b1 | b2 | b3;
}

void l2b_endian_16( unsigned int * val ){
	uint32_t b0,b1,b2,b3;
	uint32_t num = *val;
	b0 = (num & 0x000000ff) << 8u;
	b1 = (num & 0x0000ff00) >> 8u;
	b2 = (num & 0x00ff0000);
	b3 = (num & 0xff000000);
	*val = b0 | b1 | b2 | b3;
}


// Fonctions de lecture des valeurs codees en big endian vers une architecture little endian 
// Lis le header du fichier ELF
int read_Elf32_Ehdr( FILE *f, Elf32_Ehdr * h ){
	unsigned long int size = sizeof(Elf32_Ehdr);
	fread( h, size , 1, f);
	// Inversion de l'endianess 
	l2b_endian_16((unsigned int * )&(h->e_type));
	l2b_endian_16((unsigned int * )&(h->e_machine));
    l2b_endian_32((unsigned int * )&(h->e_version));
    l2b_endian_32((unsigned int * )&(h->e_entry));
    l2b_endian_32((unsigned int * )&(h->e_phoff));
    l2b_endian_32((unsigned int * )&(h->e_shoff));
    l2b_endian_32((unsigned int * )&(h->e_flags));
	l2b_endian_16((unsigned int * )&(h->e_ehsize));
	l2b_endian_16((unsigned int * )&(h->e_phentsize));
	l2b_endian_16((unsigned int * )&(h->e_phnum));
	l2b_endian_16((unsigned int * )&(h->e_shentsize));
	l2b_endian_16((unsigned int * )&(h->e_shnum));
	l2b_endian_16((unsigned int * )&(h->e_shstrndx));
	return size;
}

// Lis un header de section 
int read_Elf32_Shdr( FILE *f, Elf32_Shdr * h ){
	unsigned long int size = sizeof(Elf32_Shdr);
	fread( h, size , 1, f);
	// Inversion de l'endianess 
	l2b_endian_32((unsigned int * )&(h->sh_name));
	l2b_endian_32((unsigned int * )&(h->sh_type));
	l2b_endian_32((unsigned int * )&(h->sh_flags));
	l2b_endian_32((unsigned int * )&(h->sh_addr));
	l2b_endian_32((unsigned int * )&(h->sh_offset));
	l2b_endian_32((unsigned int * )&(h->sh_size));
	l2b_endian_32((unsigned int * )&(h->sh_link));
	l2b_endian_32((unsigned int * )&(h->sh_info));
	l2b_endian_32((unsigned int * )&(h->sh_addralign));
	l2b_endian_32((unsigned int * )&(h->sh_entsize));
}

// Lis une structure Elf32_Sym
// Renvoie le nombre d'octets lus 
int read_Elf32_Sym( FILE *f, Elf32_Sym *s ){
	unsigned long int size = sizeof(Elf32_Sym);
	fread( s, size , 1, f);
	// Inversion big/little sur les champs de plus d'un octet 
	l2b_endian_32( (unsigned int *)s );
	l2b_endian_32( (unsigned int *)&(s->st_value));
	l2b_endian_32( (unsigned int *)&(s->st_size));
	l2b_endian_16( (unsigned int *)&(s->st_shndx));
	return size;
}




