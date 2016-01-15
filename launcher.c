#include "./src/Programme_Editeur_Liens/API.h"

int main(int argc, char * argv[])
{
	FILE* f = NULL;
	if((f = fopen(argv[1], "r")) == NULL)
	{
		printf("Erreur lors de la lecture du fichier ELF\n");
		return -1;
	}
	
	//Initialisations des variables utiles
	Elf32_Ehdr h;
	Shdr_list shdr_list;
	Sym_list  sym_list;
	unsigned char ** names;
	
	// Lecture dans le fichier 
	read_Elf32_Ehdr(f,&h);
	names = sections_names_table(f,h);	
	read_Shdr_list(f, h, &shdr_list);
	read_Sym_list(f, h, &sym_list, shdr_list, names);
	
	afficher_en_tete(h);
	afficher_Shdr_list(&shdr_list);
	afficher_reimplantation(h, &shdr_list, names);
	afficher_Sym_list(sym_list);
	
	fclose(f);
}
