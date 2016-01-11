#include "API.h"
#include <stdio.h>
#include <elf.h>

int main(int argc, char * argv[])
{
	FILE* f = NULL;
	if((f = fopen(argv[1], "r")) == NULL)
	{
		printf("Erreur lors de la lecture du fichier ELF");
		return -1;
	}
	
	Elf32_Ehdr header;
	Shdr_list shdr_list;
	Sym_list sym_list; 
	
	read_Elf32_Ehdr(f, &header);
	afficher_Elf32_Ehdr(header);
	
	read_Shdr_list(f, header, &shdr_list);
	afficher_Shdr_list(&shdr_list);
	
	read_Sym_list(f, header, &sym_list);
	afficher_Sym_list(sym_list);
	
/*	
	char **table;
	int i=0;
	table = sections_names_table(f,h);
	for(;i<h.e_shnum;i++)
	{
		printf("%s\n",table[i]);
	}	
*/	
	fclose(f);
	return 0;
}
