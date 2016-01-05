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
			return "ARM_ATTRIBUTES";
	}
}

char* sh_flags(Elf32_Word flagsData)
{
	char* result = (char*)malloc(sizeof(char) * 3);
	
	// WRITE FLAG
	if(flagsData & 0x1)
	{
		strcat(result, "W");
	}
	// ALLOC FLAG
	if(flagsData & 0x2) 
	{
		strcat(result, "A");
	}
	// EXECINSTR
	if(flagsData & 0x4)
	{
		strcat(result, "X");
	}
	// MASKPROC
	if(flagsData & 0xf0000000)
	{
		// TODO : strcat le symbol du masque, à afficher
	}
	
	return result;
}

// Attention : ne bytes swap qu'une partie des valeurs
void bytes_swap_file_header(Elf32_Ehdr* header)
{
	header->e_shoff = __bswap_32(header->e_shoff);
	header->e_phnum = __bswap_16(header->e_phnum);
	header->e_shentsize = __bswap_16(header->e_shentsize);
	header->e_shnum = __bswap_16(header->e_shnum);
	header->e_shstrndx = __bswap_16(header->e_shstrndx);
}

void bytes_swap_section_header(Elf32_Shdr* section)
{
	section->sh_name = __bswap_32(section->sh_name);
	section->sh_type = __bswap_32(section->sh_type);
	section->sh_flags = __bswap_32(section->sh_flags);
	section->sh_addr = __bswap_32(section->sh_addr);
	section->sh_offset = __bswap_32(section->sh_offset);
	section->sh_size = __bswap_32(section->sh_size);
	section->sh_link = __bswap_32(section->sh_link);
	section->sh_info = __bswap_32(section->sh_info);
	section->sh_addralign = __bswap_32(section->sh_addralign);
	section->sh_entsize = __bswap_32(section->sh_entsize);
}

void afficher_table_sections(FILE* elfFile)
{
	// Structure définissant l'entête du fichier .ELF
	Elf32_Ehdr elfFileHeader;

	// Structure définissant l'entête d'une section
	Elf32_Shdr elfSectionHeader;

	// Lecture de l'entête du fichier .ELF
	fread(&elfFileHeader, sizeof(Elf32_Ehdr), 1, elfFile);
	
	// Swap les valeurs de la structure
	bytes_swap_file_header(&elfFileHeader);
	
	// Offset du tableau d'entête de sections
	int headerSectionsTableOffset = elfFileHeader.e_shoff;
	
	// Place le curseur du stream à l'emplacement du tableau d'entêtes de sections
	fseek(elfFile, headerSectionsTableOffset, SEEK_SET);
	
	int offset = elfFileHeader.e_shoff + elfFileHeader.e_shstrndx*elfFileHeader.e_shentsize;
	
	fseek(elfFile, offset, SEEK_SET);
	fseek(elfFile, 16, SEEK_CUR );
	fread( &offset, 4, 1, elfFile );
	offset = __bswap_32(offset);
	
	// Affichage du nom des colonnes de données
	printf("%4s %20s %16s %8s %6s %6s %2s %3s %2s %3s %2s\n", "[Nr]", "Nom",	"Type",	"Adr", "Décala.", "Taille",	"ES", "Fan", "LN", "Inf", "Al");
	
	int sectionsCount = elfFileHeader.e_shnum;
	int s_index;
	for(s_index = 0; s_index < sectionsCount; s_index++)
	{
		// Mets le curseur sur l'entête de la section courante
		fseek(elfFile, headerSectionsTableOffset + sizeof(elfSectionHeader) * s_index, SEEK_SET);
		
		// Lecture de l'entête de la section
		fread(&elfSectionHeader, sizeof(elfSectionHeader), 1, elfFile);

		// Swap des valeurs de la structure
		bytes_swap_section_header(&elfSectionHeader);

		// *** Obtiention du nom de la section ***
		fseek(elfFile, offset + elfSectionHeader.sh_name, SEEK_SET);
		char name[256];
		fscanf(elfFile, "%255s", name);
		// ***************************************
		
		
		// Obtient les symboles définissant les FLAGS
		char* flagsSymb = sh_flags(elfSectionHeader.sh_flags);

		// Affichage des données de l'entête de la section courante
		// 		Nr 	  Nom  Type  Adr  Décala. Taille ES Fan LN Inf Al
		printf("[%2d] %20s %16s %08d %06x %06x %02x %3s %2d %3d %2d\n", 
		s_index,
		name,
		sh_type(elfSectionHeader.sh_type),
		elfSectionHeader.sh_addr,
		elfSectionHeader.sh_offset,
		elfSectionHeader.sh_size,
		elfSectionHeader.sh_entsize,
		flagsSymb,
		elfSectionHeader.sh_link,
		elfSectionHeader.sh_info,
		elfSectionHeader.sh_addralign);
		
		// Libére la mémoire utilisée par la chaîne de symboles
		free(flagsSymb);
	}
}
