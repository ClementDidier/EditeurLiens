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



int afficher_table_symboles( FILE *f ){

	unsigned int current_pos, tmp;
	unsigned int shentsize, shnum, shoff;
	
	// Lecture des informations sur la table des section headers
	fseek( f, 32, SEEK_SET );	// 32 = offset du champs e_shoff 
	fread( &shoff, 4, 1, f );
	l2b_endian_32( &shoff );	
	fseek( f, 44, SEEK_SET );	// 44 = offset du champs e_shentsize
	fread( &tmp, 4, 1, f );
	l2b_endian_32( &tmp );
	shentsize = ( tmp & 0xFFFF ); 
	fseek( f, 46, SEEK_SET );	// 46 = offset du champs e_shnum
	fread( &tmp, 4, 1, f );
	l2b_endian_32( &tmp );
	shnum = ( tmp & 0xFFFF ); 
	
	printf("[+] Section headers table at offset : %d\n[+] Size of section headers : %d\n[+] Number of section headers : %d\n\n", shoff, shentsize, shnum );
	
	// 

}
