#include "API.h"

// Fonctions d'inversion d'endianness little->big ou big->little
// Travaillent directement sur la mémoire 
void l2b_endian_32( unsigned int * val )
{
	uint32_t b0,b1,b2,b3;
	uint32_t num = *val;
	b0 = (num & 0x000000ff) << 24u;
	b1 = (num & 0x0000ff00) << 8u;
	b2 = (num & 0x00ff0000) >> 8u;
	b3 = (num & 0xff000000) >> 24u;
	*val = b0 | b1 | b2 | b3;
}

void l2b_endian_16( unsigned int * val )
{
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
int read_Elf32_Ehdr( FILE *f, Elf32_Ehdr * h )
{
	unsigned long int size = sizeof(Elf32_Ehdr);
	fseek(f, 0, SEEK_SET);
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
int read_Elf32_Shdr( FILE *f, Elf32_Ehdr h, unsigned int index, Elf32_Shdr * s)
{
	unsigned long int size = sizeof(Elf32_Shdr);
	fseek(f, h.e_shoff + size * index, SEEK_SET);
	fread(s, size, 1, f);
	// Inversion de l'endianess 
	l2b_endian_32((unsigned int * )&(s->sh_name));
	l2b_endian_32((unsigned int * )&(s->sh_type));
	l2b_endian_32((unsigned int * )&(s->sh_flags));
	l2b_endian_32((unsigned int * )&(s->sh_addr));
	l2b_endian_32((unsigned int * )&(s->sh_offset));
	l2b_endian_32((unsigned int * )&(s->sh_size));
	l2b_endian_32((unsigned int * )&(s->sh_link));
	l2b_endian_32((unsigned int * )&(s->sh_info));
	l2b_endian_32((unsigned int * )&(s->sh_addralign));
	l2b_endian_32((unsigned int * )&(s->sh_entsize));
	return size;
}

// Lis une structure Elf32_Sym
// Renvoie le nombre d'octets lus 
int read_Elf32_Sym( FILE *f, Elf32_Sym *s)
{
	unsigned long int size = sizeof(Elf32_Sym);
	fread( s, size , 1, f);
	// Inversion big/little sur les champs de plus d'un octet 
	l2b_endian_32( (unsigned int *)s );
	l2b_endian_32( (unsigned int *)&(s->st_value));
	l2b_endian_32( (unsigned int *)&(s->st_size));
	l2b_endian_16( (unsigned int *)&(s->st_shndx));
	return size;
}


void read_Shdr_list( FILE *f )
{
	Elf32_Ehdr h;
	int i = 0;
	Shdr_list *L = malloc( sizeof(Shdr_list) ), *Q = L, *N;
	L->next = NULL;
	read_Elf32_Ehdr(f, &h);
	read_Elf32_Shdr(f, h, 0, &(L->header));
	for( i = 1; i < h.e_shnum; i++ ){
		N = malloc( sizeof(Shdr_list) );
		read_Elf32_Shdr(f, h, i, &(N->header));
		Q->next = N;
		Q = N;
		N->next = NULL;
	}
	shdr_list = L;
	
}

void afficher_Shdr( Elf32_Shdr s){
	printf("SECTION HEADER :\n\tsh_name : %d\n\tsh_type : %d\n\tsh_addr : %d\n\tsh_offset : %d\n\tsh_size : %d\n\tsh_entsize : %d\n\tsh_flags : %d\n\tsh_link : %d\n\tsh_info : %d\n\tsh_addralign : %d\n", 
		 s.sh_name, s.sh_type, s.sh_addr, s.sh_offset, s.sh_size, s.sh_entsize, s.sh_flags, s.sh_link, s.sh_info, s.sh_addralign);	
	
}

void afficher_Shdr_list( ){
	Shdr_list * L = shdr_list;
	printf(" Liste des section headers : \n");
	while( L != NULL ){
		afficher_Shdr(L->header);
		L = L->next;
	}
}


// Lis une structure Elf32_Rel (lis une ligne du tableau de relocation)
// Renvoie le nombre d'octets lus 
int read_Elf32_Rel( FILE *f, Elf32_Rel *r, int indice, Elf32_Shdr s)
{
	unsigned long int size = sizeof(Elf32_Rel);
	fseek(f, s.sh_offset, SEEK_SET);
	fseek(f, indice*sizeof(Elf32_Rel), SEEK_CUR);
	fread(r, sizeof(Elf32_Rel), 1,f);

	// Inversion big/little sur les champs de plus d'un octet 
	l2b_endian_32( (unsigned int *)&(r->r_offset));
	l2b_endian_32( (unsigned int *)&(r->r_info));
	return size;
}


// Lis une structure Elf32_Rela (lis une ligne du tableau de relocation)
// Renvoie le nombre d'octets lus 
int read_Elf32_Rela( FILE *f, Elf32_Rela *ra, int indice, Elf32_Shdr s)
{
	unsigned long int size = sizeof(Elf32_Rela);
	fseek(f, s.sh_offset, SEEK_SET);
	fseek(f, indice*sizeof(Elf32_Rela), SEEK_CUR);
	fread(ra, sizeof(Elf32_Rela), 1, f);

	// Inversion big/little sur les champs de plus d'un octet 
	l2b_endian_32( (unsigned int *)&(ra->r_offset));
	l2b_endian_32( (unsigned int *)&(ra->r_info));
	l2b_endian_32( (unsigned int *)&(ra->r_addend));
	return size;
}




