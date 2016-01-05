#include <elf.h>
#include <stdio.h>

void afficher_table_sections(FILE* elfFile)
{
	// Structure définissant l'entête du fichier .ELF
	Elf32_Ehdr elfFileHeader;

	// Structure définissant l'entête d'une section
	Elf32_Shdr elfSectionHeader;

	// Lecture de l'entête du fichier .ELF
	fread(&elfFileHeader, sizeof(Elf32_Ehdr), 1, elfFile);

	// DEBUG -- Mauvaise lecture de l'entête
	printf("Nombre de sections headers : %d\n", elfFileHeader.e_shnum);

	// e_shoff : Emplacement en octets du tableau d'entêtes de sections (depuis le début du fichier)
	// Place le curseur en lecture du stream à l'emplacement du tableau d'enetêtes de sections
	fseek(elfFile, elfFileHeader.e_shoff + (elfFileHeader.e_shstrndx * sizeof(elfSectionHeader)), SEEK_SET);

	// Lecture de l'entête de la première section
	//fread(&elfSectionHeader, 1, sizeof(elfSectionHeader), elfFile);
	//fseek(elfFile, elfSectionHeader.sh_offset, SEEK_SET);
	
	// Lecture de chaque section et affichage
	int s_index;
	for(s_index = 0; s_index < elfFileHeader.e_shnum; s_index++)
	{
		// Place le curseur à l'entête de la section d'index s_index
		fseek(elfFile, elfFileHeader.e_shoff + s_index * sizeof(elfSectionHeader), SEEK_SET);
		// Lecture de l'entête de la section
		fread(&elfSectionHeader, sizeof(Elf32_Shdr), 1, elfFile);
	
		// Affichage des données de l'entête de la section courante
		//printf("Name : %d\n", elfSectionHeader.sh_name);
	}
}
