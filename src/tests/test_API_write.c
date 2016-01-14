#include "../Programme_Editeur_Liens/API.h"
#include <stdio.h>
#include <elf.h>
#include "../Fonction_Non_Utilisées/affichage_sections.h"

int main(int argc, char * argv[])
{
	FILE* f = NULL;
	if((f = fopen(argv[1], "r")) == NULL)
	{
		printf("Erreur lors de la lecture du fichier ELF\n");
		return -1;
	}
	
	Elf32_Ehdr h;
	read_Elf32_Ehdr(f, &h);

	Shdr_list shdr_list;
	read_Shdr_list(f, h, &shdr_list);
	
	Sym_list sym_list;
	unsigned char ** names = sections_names_table(f,h);
	read_Sym_list(f, h, &sym_list, shdr_list, names);
	
	// Creation du fichier
	FILE * fresult = NULL;
	if((fresult = fopen("elfres.o", "w")) == NULL)
	{
		printf("Erreur lors de l'écriture du fichier ELF\n");
		return -1;
	}
	
	afficher_Elf32_Ehdr(h);
	afficher_Shdr_list(&shdr_list);
	
	write_Elf32_Ehdr(fresult, h);
	write_Shdr_list(fresult, h, &shdr_list);
	
	write_Sym_list(fresult, h, sym_list);
	
	rewind(fresult);
	//afficher_table_sections(fresult);
	
	fclose(f);
	fclose(fresult);
	return 0;
}
