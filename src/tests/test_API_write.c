#include "../api/API.h"
#include <stdio.h>
#include <elf.h>
#include "../affichage_sections.h"

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
	
	Sym_list sym_list;
	read_Sym_list(f, header, &sym_list);
	
	// Creation du fichier
	FILE * fresult = NULL;
	if((fresult = fopen("elfres.o", "w")) == NULL)
	{
		printf("Erreur lors de l'écriture du fichier ELF\n");
		return -1;
	}
	
	afficher_Elf32_Ehdr(header);
	afficher_Shdr_list(&shdr_list);
	
	write_Elf32_Ehdr(fresult, header);
	write_Shdr_list(fresult, header, &shdr_list);
	
	write_Sym_list(fresult, header, sym_list);
	
	rewind(fresult);
	afficher_table_sections(fresult);
	
	fclose(f);
	fclose(fresult);
	return 0;
}
