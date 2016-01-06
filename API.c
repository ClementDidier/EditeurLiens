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


// Lecture de l'ensemble des Section Headers  

void read_Shdr_list( FILE *f )
{
	Elf32_Ehdr h;
	int i;
	Shdr_list *L = malloc( sizeof(Shdr_list) ), *Q = L, *N;
	L->next = NULL;
	read_Elf32_Ehdr(f, &h);
	read_Elf32_Shdr(f, h, 0, &(L->header));
	for( i = 1; i < h.e_shnum; i++ ){
		N = malloc( sizeof(Shdr_list) );
		// Lecture du header
		read_Elf32_Shdr(f, h, i, &(N->header));
		// Lecture du dump raw de la section
		fseek( f, N->header.sh_offset, SEEK_SET );
		N->dump = malloc( N->header.sh_size );
		fread( N->dump, N->header.sh_size, 1, f); 
		// Passage a la section suivante 
		Q->next = N;
		Q = N;
		N->next = NULL;
	}
	shdr_list = L;
	
}

void afficher_Shdr( Shdr_list *L){
	Elf32_Shdr s = L->header;
	int i;
	printf("  [+]Header\n\tsh_name : %d\n\tsh_type : %d\n\tsh_addr : %d\n\tsh_offset : %d\n\tsh_size : %d\n\tsh_entsize : %d\n\tsh_flags : %d\n\tsh_link : %d\n\tsh_info : %d\n\tsh_addralign : %d\n",  s.sh_name, s.sh_type, s.sh_addr, s.sh_offset, s.sh_size, s.sh_entsize, s.sh_flags, s.sh_link, s.sh_info, s.sh_addralign);	
	printf("  [+] Content\n\t");
	for( i = 0; i < L->header.sh_size; i++ ){
		printf("%.02x", (unsigned char)(L->dump[i]));
	}
	printf("\n");
}

void afficher_Shdr_list( ){
	int i = 0;
	Shdr_list * L = shdr_list;
	printf(" Liste des section headers : \n");
	while( L != NULL ){
		printf(" Section [%d] :\n",i++);
		afficher_Shdr(L);
		L = L->next;
	}
}

// Lecture de l'ensemble des symboles 
void read_Sym_list( FILE *f ){
	unsigned int tmp;
	unsigned int shentsize, shoff;
	unsigned int symtab_off = 0, strtab_off = 0, symtab_size = 0; 
	unsigned int strtab_index, size, i;
		
	// Lecture des informations sur la table des section headers ( offset, taille d'un header, nombre de header )
	fseek( f, 32, SEEK_SET );	// 32 = offset du champs e_shoff 
	fread( &shoff, 4, 1, f ); l2b_endian_32( &shoff );	
	fseek( f, 44, SEEK_SET );	// 44 = offset du champs e_shentsize
	fread( &tmp, 4, 1, f ); l2b_endian_32( &tmp );
	shentsize = ( tmp & 0xFFFF ); 
	fseek( f, 46, SEEK_SET );	// 46 = offset du champs e_shnum
	fread( &tmp, 4, 1, f ); l2b_endian_32( &tmp );
	
	// Recherche dans les headers de celui de .symtab
	fseek( f, shoff, SEEK_SET );
	// Pour chaque header on regarde si c'est le bon ( i.e si son type correspond a SHT_SYMTAB )
	while( (symtab_off == 0 || strtab_off == 0 )){
		fseek( f, 4, SEEK_CUR );
		fread( &tmp, 4, 1, f ); l2b_endian_32( &tmp );
		if ( tmp == SHT_SYMTAB ){
			// On a trouve le header de la table des symboles
			// Lecture des infos 
			fseek( f, 8, SEEK_CUR );
			fread( &symtab_off, 4, 1, f ); l2b_endian_32( &symtab_off );
			fread( &symtab_size, 4, 1, f ); l2b_endian_32( &symtab_size );
			// Lecture de l'index dans la table des headers de la table des strings associee : .strtab 
			fread( &strtab_index, 4, 1, f ); l2b_endian_32( &strtab_index );
			// Deplacement du descripteur de fichier vers le header de .strtab
			fseek( f, shoff + shentsize*strtab_index, SEEK_SET );
			fseek( f, 16, SEEK_CUR );
			// Lecture de son offset 
			fread( &strtab_off, 4, 1, f ); l2b_endian_32( &strtab_off );
		}else{
			fseek( f, 8*sizeof(Elf32_Word), SEEK_CUR );
		}
	} 
	
	// Lecture de la table des symboles un par un : 
	fseek( f, symtab_off, SEEK_SET );
	size = symtab_size / sizeof(Elf32_Sym);
	sym_list.nb = size;
	sym_list.list = malloc( size * sizeof(Elf32_Sym) );
	for( i = 0; i < size; i++ )
		read_Elf32_Sym( f, &(sym_list.list[i]) );
}


void afficher_Sym( Elf32_Sym S ){
	printf("\tname : %d \tvalue : %d \tsize : %d \tinfo : %d \tother : %d \tshndx : %d\n",
	S.st_name, S.st_value, S.st_size, S.st_info, S.st_other, S.st_shndx);  
}

void afficher_Sym_list(){
	int i;
	for( i = 0; i < sym_list.nb; i++ ){
		printf("Symbole [%d] : \n", i);
		afficher_Sym( sym_list.list[i] );
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




