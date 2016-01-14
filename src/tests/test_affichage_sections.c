#include <stdio.h>
#include "../Fonction_Non_Utilisées/affichage_sections.h"

int main(int argc, char *argv[]) 
{
	FILE* ElfFile = NULL;
	if((ElfFile = fopen(argv[1], "r")) == NULL)
	{
		printf("Erreur lors de la lecture du fichier ELF\n");
		return -1;
	}
	
	afficher_table_sections(ElfFile);

	return 0;
}
