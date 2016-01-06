#include "API.h"

// Lis une structure Elf32_Rel (lis une ligne du tableau de relocation)
// Renvoie le nombre d'octets lus 
int read_Elf32_Rel( FILE *f, Elf32_Rel *r, int indice, Elf32_Shdr s,)
{
	unsigned long int size = sizeof(Elf32_Rel);
	fseek(f, s.sh_offset, SEEK_SET);
	fseek(f, indice*sizeof(Elf32_Rel), SEEK_CUR);
	fread(r, 1, sizeof(Elf32_Rel), elfFile);

	// Inversion big/little sur les champs de plus d'un octet 
	l2b_endian_32( (unsigned int *)&(r->offset));
	l2b_endian_32( (unsigned int *)&(r->info));
	return size;
}


// Lis une structure Elf32_Rela (lis une ligne du tableau de relocation)
// Renvoie le nombre d'octets lus 
int read_Elf32_Rela( FILE *f, Elf32_Rela *ra, int indice, Elf32_Shdr s,)
{
	unsigned long int size = sizeof(Elf32_Rela);
	fseek(f, s.sh_offset, SEEK_SET);
	fseek(f, indice*sizeof(Elf32_Rela), SEEK_CUR);
	fread(r, 1, sizeof(Elf32_Rela), elfFile);

	// Inversion big/little sur les champs de plus d'un octet 
	l2b_endian_32( (unsigned int *)&(ra->offset));
	l2b_endian_32( (unsigned int *)&(ra->info));
	l2b_endian_32( (unsigned int *)&(ra->addend));
	return size;
}

int main(int argc, char **argv){

	FILE* fichier = fopen(argv[1],"r");

 	Elf32_Ehdr h;

	Elf32_Shdr s;

	Elf32_Rel r;


	read_Elf32_Rel(fichier, &h);
	read_Elf32_Rel(fichier, &s, h);
	read_Elf32_Rel(fichier, &r, 0, s);

	return 0;

}
