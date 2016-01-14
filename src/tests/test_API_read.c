#include "../Programme_Editeur_Liens/API.h"
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
	
	Elf32_Ehdr h;
	Shdr_list shdr_list;
	Sym_list sym_list; 
	char **names;
	
	printf("-- Lecture de l'entête du fichier...\n");
	read_Elf32_Ehdr(f, &h);
	afficher_Elf32_Ehdr(h);
	
	printf("-- Lecture des entêtes de sections du fichier...\n");
	read_Shdr_list(f, h, &shdr_list);
	afficher_Shdr_list(&shdr_list);

	printf("-- Lecture de la liste de symboles du fichier...\n");
	names = sections_names_table(f,h);
	read_Sym_list(f, h, &sym_list, shdr_list, names);
	afficher_Sym_list(sym_list);
	
	int i=0;
	for(;i<h.e_shnum;i++)
	{
		printf("%s\n",names[i]);
	}	

	fclose(f);
	return 0;
}
