#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <byteswap.h>

void creation_reimplantation(FILE* elfFile){
	
	Elf32_Rel elfRel;

	Elf32_Rela elfRela;

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

	// Passage de la notation big endian à little endian du nombre de sections
	int sectionsCount = __bswap_16(elfFileHeader.e_shnum);
	int s_index;
	// Lecture de chaque section et affichage
	for(s_index = 0; s_index < sectionsCount; s_index++)
	{
		// Lecture de l'entête de la section
		fread(&elfSectionHeader, 1, sizeof(elfSectionHeader), elfFile);

	 	// Obtenir le nom depuis le tableau de chaînes
		elfSectionHeader.sh_type = __bswap_32(elfSectionHeader.sh_type);
		if(elfSectionHeader.sh_type == SHT_REL){
			elfSectionHeader.sh_offset = __bswap_32(elfSectionHeader.sh_offset);
			fseek(elfFile, elfSectionHeader.sh_offset, SEEK_SET);
			fread(&elfRel, 1, sizeof(Elf32_Rel), elfFile);
			elfRel.r_offset = __bswap_32(elfRel.r_offset);
			elfRel.r_info = __bswap_32(elfRel.r_info);
			printf("Offset : 0x%x\n",elfRel.r_offset);
			printf("Info : 0x%x\n",elfRel.r_info );
		}else if(elfSectionHeader.sh_type == SHT_RELA){
			elfSectionHeader.sh_offset = __bswap_32(elfSectionHeader.sh_offset);
			fseek(elfFile, elfSectionHeader.sh_offset, SEEK_SET);
			fread(&elfRela, 1, sizeof(Elf32_Rela), elfFile);
			elfRela.r_offset = __bswap_32(elfRela.r_offset);
			elfRela.r_info = __bswap_32(elfRela.r_info);
			printf("Offset : 0x%x\n",elfRela.r_offset);
			printf("Info : 0x%x\n",elfRela.r_info );
		}
		
	}
}

int main(int argc, char **argv){

	FILE* fichier = fopen(argv[1],"r");
	creation_reimplantation(fichier);

	return 0;

}