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
	
	Elf32_Ehdr h;
	
	read_Elf32_Ehdr(f, &h);

	printf("HEADER  : \n\te_type : %d\n\te_machine : %d\n\te_version : %d\n\te_entry : %d\n\te_phoff : %d\n\te_shoff : %d\n\te_flags : %d\n\te_ehsize : %d\n\te_phentsize : %d\n\te_phnum : %d\n\te_shentsize : %d\n\te_shnum : %d\n\te_shstrndx : %d\n\n",
		h.e_type, h.e_machine, h.e_version, h.e_entry, h.e_phoff, h.e_shoff, h.e_flags, h.e_ehsize, h.e_phentsize, h.e_phnum, h.e_shentsize, h.e_shnum, h.e_shstrndx);
	
	
	read_Shdr_list( f );
	afficher_Shdr_list();
	read_Sym_list( f );
	afficher_Sym_list();
	read_header( f );
	afficher_Elf32_Ehdr( header );
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
