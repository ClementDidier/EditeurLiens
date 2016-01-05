#include <elf.h>
#include <stdio.h>
#include <byteswap.h>
#include <string.h>
#include <stdlib.h>


char* sh_type(Elf32_Word typeData)
{
	switch(typeData)
	{
		case 0:
			return "NULL";
			break;
		case 1:
			return "PROGBITS";
			break;
		case 2:
			return "SYMTAB";
			break;
		case 3:
			return "STRTAB";
			break;
		case 4:
			return "RELA";
			break;
		case 5:
			return "HASH";
			break;
		case 6:
			return "DYNAMIC";
			break;
		case 7:
			return "NOTE";
			break;
		case 8:
			return "NOBITS";
			break;
		case 9:
			return "REL";
			break;
		case 10:
			return "SHLIB";
			break;
		case 11:
			return "DYNSYM";
			break;
		case 0x70000000:
			return "LOPROC";
			break;
		case 0x7fffffff:
			return "HIPROC";
			break;
		case 0x80000000:
			return "LOUSER";
			break;
		case 0xffffffff:
			return "HIUSER";
			break;
		default:
			return "NULL";
	}
}

// TODO : Concatener les caractères des flags detectés
char* sh_flags(Elf32_Word flagsData)
{
	char* result = (char*)malloc(sizeof(char) * 4);
	
	// WRITE FLAG
	if(flagsData & 1)
	{
		strcat(result, "W");
	}
	// ALLOC FLAG
	else if((flagsData >> 1) & 1) 
	{
		strcat(result, "A");
	}
	// EXECINSTR
	else if((flagsData >> 2) & 1)
	{
		strcat(result, "X");
	}
	// MASKPROC
	else if((flagsData >> 3) & 1)
	{
		
	}
	
	return result;
}

void afficher_table_sections(FILE* elfFile)
{
	// Structure définissant l'entête du fichier .ELF
	Elf32_Ehdr elfFileHeader;

	// Structure définissant l'entête d'une section
	Elf32_Shdr elfSectionHeader;

	// Lecture de l'entête du fichier .ELF
	fread(&elfFileHeader, 1, sizeof(Elf32_Ehdr), elfFile);
	
	// Offset du tableau d'entête de sections
	int headerSectionsTableOffset = __bswap_32(elfFileHeader.e_shoff);
	

	// Place le curseur du stream à l'emplacement du tableau d'enetêtes de sections
	fseek(elfFile, headerSectionsTableOffset, SEEK_SET);

	// Affichage du nom des colonnes de données
	printf("%4s %8s %16s %8s %6s %6s %2s %3s\n", "[Nr]", "Nom",	"Type",	"Adr", "Décala.", "Taille",	"ES", "Fan");

	// Passage de la notation big endian à little endian du nombre de sections
	int sectionsCount = __bswap_16(elfFileHeader.e_shnum);
	int s_index;
	// Lecture de chaque section et affichage
	for(s_index = 0; s_index < sectionsCount; s_index++)
	{
		// Lecture de l'entête de la section
		fread(&elfSectionHeader, 1, sizeof(elfSectionHeader), elfFile);

		// Affichage des données de l'entête de la section courante
		// 		Nr 	  Nom  Type  Adr  Décala. Taille ES Fan LN Inf Al
		printf("[%2d] %08x %16s %08d %06x %06x %02x %3s\n", 
		s_index,
		elfSectionHeader.sh_name, // Obtenir le nom depuis le tableau de chaînes
		sh_type(__bswap_32(elfSectionHeader.sh_type)),
		elfSectionHeader.sh_addr,
		__bswap_32(elfSectionHeader.sh_offset),
		__bswap_32(elfSectionHeader.sh_size),
		__bswap_32(elfSectionHeader.sh_entsize),
		sh_flags(__bswap_32(elfSectionHeader.sh_flags)));
	}
}
