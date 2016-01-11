#include "API.h"
#include <stdio.h>
#include <elf.h>

int main(int argc, char * argv[])
{
	FILE* f = NULL;
	if((f = fopen(argv[1], "r")) == NULL)
	{
		printf("Erreur lors de la lecture du fichier ELF\n");
		return -1;
	}
	
	Elf32_Ehdr header;
	read_Elf32_Ehdr(f, &header);

	Shdr_list shdr_list;
	read_Shdr_list(f, header, &shdr_list);

	//printf("HEADER FICHIER 1 : \n\te_type : %d\n\te_machine : %d\n\te_version : %d\n\te_entry : %d\n\te_phoff : %d\n\te_shoff : %d\n\te_flags : %d\n\te_ehsize : %d\n\te_phentsize : %d\n\te_phnum : %d\n\te_shentsize : %d\n\te_shnum : %d\n\te_shstrndx : %d\n\n",
		//h.e_type, h.e_machine, h.e_version, h.e_entry, h.e_phoff, h.e_shoff, h.e_flags, h.e_ehsize, h.e_phentsize, h.e_phnum, h.e_shentsize, h.e_shnum, h.e_shstrndx);
	
	
	// Creation du fichier
	FILE * fresult = NULL;
	if((fresult = fopen("elfres.o", "w+")) == NULL)
	{
		printf("Erreur lors de l'écriture du fichier ELF\n");
		return -1;
	}
	
	write_Elf32_Ehdr(fresult, header);
	write_Shdr_list(fresult, header, &shdr_list);
	afficher_Shdr_list(&shdr_list);
	
	fclose(f);
	fclose(fresult);
	return 0;
}
