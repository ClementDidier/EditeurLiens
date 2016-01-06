#include "Table_Symboles.h"

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

// Lis un symbole 
// Renvoie le nombre d'octets lus 
int read_symb( FILE *f, Elf32_Sym *s ){
	unsigned long int size = sizeof(Elf32_Sym);
	int i;
	fread( s, size , 1, f);
	// Inversion big/little sur les champs de plus d'un octet 
	l2b_endian_32( (unsigned int *)s );
	l2b_endian_32( (unsigned int *)&(s->st_value));
	l2b_endian_32( (unsigned int *)&(s->st_size));
	l2b_endian_16( (unsigned int *)&(s->st_shndx));
	return size;
}

void afficher_type_bind( unsigned char info ){
	switch(ELF32_ST_TYPE(info)){
		case 0:
			printf("NOTYPE  ");
			break;
		case 1:
			printf("OBJECT  ");
			break;
		case 2:
			printf("FUNC    ");
			break;
		case 3:
			printf("SECTION ");
			break;
		case 4:
			printf("FILE   ");
			break;
		case 13:
			printf("LOPROC ");
			break;
		case 15:
			printf("HIPROC ");
			break;
	}
	switch(ELF32_ST_BIND(info)){
		case 0:
			printf("LOCAL  ");
			break;
		case 1:
			printf("GLOBAL ");
			break;
		case 2:
			printf("WEAK   ");
			break;
		case 13:
			printf("LOPROC ");
			break;
		case 15:
			printf("HIPROC ");
			break;
	}
}

afficher_ndx( int val ){
	switch( val ){
		case SHN_ABS:
			printf("ABS  ");
			break;
		case SHN_COMMON:
			printf("COMM ");
			break;
		case SHN_UNDEF:
			printf("UND  ");
			break;
		default:
			printf("%04d ", val);
	}
}

// ATTENTION : modifie le descripteur de fichier 
void afficher_symb( Elf32_Sym *s , unsigned int strtab_off, FILE *f){
	char name[256];
	fpos_t pos;
	// Value - size - type - bind - Ndx ( numero de section concernee ) 
	printf("%08x %04x ", s->st_value, s->st_size);
	afficher_type_bind( s->st_info );
	afficher_ndx( (int)s->st_shndx );
	// Name
	fgetpos( f, &pos );
	fseek( f, strtab_off + s->st_name, SEEK_SET );
	fscanf( f,"%s", name);
	fsetpos( f, &pos );
	printf("%s\n", name);
}


int afficher_table_symboles( FILE *f ){

	unsigned int tmp;
	unsigned int shentsize, shnum, shoff;
	unsigned int symtab_off = 0, strtab_off = 0, symtab_size = 0; 
	int i, strtab_index, lus;
	Elf32_Sym symb;
		
	// Lecture des informations sur la table des section headers ( offset, taille d'un header, nombre de header )
	fseek( f, 32, SEEK_SET );	// 32 = offset du champs e_shoff 
	fread( &shoff, 4, 1, f ); l2b_endian_32( &shoff );	
	fseek( f, 44, SEEK_SET );	// 44 = offset du champs e_shentsize
	fread( &tmp, 4, 1, f ); l2b_endian_32( &tmp );
	shentsize = ( tmp & 0xFFFF ); 
	fseek( f, 46, SEEK_SET );	// 46 = offset du champs e_shnum
	fread( &tmp, 4, 1, f ); l2b_endian_32( &tmp );
	shnum = ( tmp & 0xFFFF ); 
	
	printf("[+] Section headers table at offset : %d\n[+] Size of section headers : %d\n[+] Number of section headers : %d\n\n", shoff, shentsize, shnum );
	
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
	
	printf("[+] Section .symtab at offset : %x\n[+] Size of section .symtab : %x\n[+] Section .strtab at offset : %x\n\n", symtab_off, symtab_size, strtab_off );
	
	// Lecture de la table des symboles un par un : 
	fseek( f, symtab_off, SEEK_SET );
	lus = 0;
	printf("\nSymbol table :\n\n");
	printf("   Value Size Type    Bind   Ndx   Name\n");
	while( lus < symtab_size ){
		lus += read_symb(f, &symb);
		afficher_symb( &symb, strtab_off, f );
	}
}
