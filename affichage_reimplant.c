#include "affichage_reimplant.h"

void creation_reimplantation(FILE* elfFile){
	
	Elf32_Rel elfRel;

	Elf32_Rela elfRela;

	// Structure définissant l'entête du fichier .ELF
	Elf32_Ehdr elfFileHeader;

	// Structure définissant l'entête d'une section
	Elf32_Shdr elfSectionHeader;

	// Lecture de l'entête du fichier .ELF
	read_Elf32_Ehdr( elfFile, &elfFileHeader);
	
	// Offset du tableau d'entête de sections
	int headerSectionsTableOffset = elfFileHeader.e_shoff;
	

	// Place le curseur du stream à l'emplacement du tableau d'enetêtes de sections
	fseek(elfFile, headerSectionsTableOffset, SEEK_SET);

	// Passage de la notation big endian à little endian du nombre de sections
	int sectionsCount = elfFileHeader.e_shnum;
	int s_index;

	// Lecture de chaque section et affichage
	for(s_index = 0; s_index < sectionsCount; s_index++)
	{

		//recuperation du nom :
		int offset = elfFileHeader.e_shoff + elfFileHeader.e_shstrndx*elfFileHeader.e_shentsize;
		fseek(elfFile, offset, SEEK_SET);
		fseek(elfFile, 16, SEEK_CUR );
		fread( &offset, 4, 1, elfFile );
		offset = __bswap_32(offset);
		fseek(elfFile, offset + elfSectionHeader.sh_name, SEEK_SET);
		char name[256];
		fscanf(elfFile, "%255s", name);

		// sans l'API
		//fseek(elfFile, headerSectionsTableOffset + sizeof(elfSectionHeader) * s_index, SEEK_SET);
		// Lecture de l'entête de la section
		//fread(&elfSectionHeader, 1, sizeof(elfSectionHeader), elfFile);

		//Avec l'API
		read_Elf32_Shdr(elfFile, elfFileHeader, s_index, &elfSectionHeader);

		if(elfSectionHeader.sh_type == SHT_REL){
			fseek(elfFile, elfSectionHeader.sh_offset, SEEK_SET);
			printf("Section de relocation : %s à l'adresse de décalage : 0x%x :\n", name,elfSectionHeader.sh_offset );
			int size;
			for(size=0; size < elfSectionHeader.sh_size; size += sizeof(Elf32_Rel)){
				fread(&elfRel, 1, sizeof(Elf32_Rel), elfFile);
				elfRel.r_offset = __bswap_32(elfRel.r_offset);
				elfRel.r_info = __bswap_32(elfRel.r_info);
				printf("Offset : 0x%x",elfRel.r_offset);
				printf("     Info : 0x%x\n",elfRel.r_info );
			}
			printf("\n");


		}else if(elfSectionHeader.sh_type == SHT_RELA){
			elfSectionHeader.sh_offset = __bswap_32(elfSectionHeader.sh_offset);
			fseek(elfFile, elfSectionHeader.sh_offset, SEEK_SET);
			elfSectionHeader.sh_size = __bswap_32(elfSectionHeader.sh_size);
			int size;
			for(size=0; size < elfSectionHeader.sh_size; size += sizeof(Elf32_Rela)){
				fread(&elfRela, 1, sizeof(Elf32_Rela), elfFile);
				elfRela.r_offset = __bswap_32(elfRela.r_offset);
				elfRela.r_info = __bswap_32(elfRela.r_info);
				printf("Offset : 0x%x",elfRela.r_offset);
				printf("   Info : 0x%x",elfRela.r_info );
				printf("   Addent : 0x%x\n",elfRela.r_addend );
			}
			printf("\n");

		}
		
	}
}

int main(int argc, char **argv){

	FILE* fichier = fopen(argv[1],"r");
	creation_reimplantation(fichier);

	return 0;

}
