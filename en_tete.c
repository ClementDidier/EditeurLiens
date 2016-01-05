#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <byteswap.h>


void creation_en_tete(FILE* ElfFile){

	// enregistrement de la structure de donnee elfhdr
	Elf32_Ehdr elfHdr;
	fread(&elfHdr, 1, sizeof elfHdr, ElfFile);
	/* ou :
	int i;
	for(i=0;i<16;i++){
		fread(&elfHdr.e_ident[i], 1, 1, ElfFile);
	}
	fread((uint16_t*)&elfHdr.e_type, sizeof(uint16_t), 1, ElfFile);
	fread(&elfHdr.e_machine, sizeof(uint16_t), 1, ElfFile);
	fread(&elfHdr.e_version, sizeof(uint32_t), 1, ElfFile);
	fread(&elfHdr.e_entry, sizeof(uint32_t), 1, ElfFile);
	fread(&elfHdr.e_phoff, sizeof(uint32_t), 1, ElfFile);
	fread(&elfHdr.e_shoff, sizeof(uint32_t), 1, ElfFile);
	fread(&elfHdr.e_flags, sizeof(uint32_t), 1, ElfFile);
	fread(&elfHdr.e_ehsize, sizeof(uint16_t), 1, ElfFile);
	fread(&elfHdr.e_phentsize, sizeof(uint16_t), 1, ElfFile);
	fread(&elfHdr.e_phnum, sizeof(uint16_t), 1, ElfFile);
	fread(&elfHdr.e_shentsize, sizeof(uint16_t), 1, ElfFile);
	fread(&elfHdr.e_shnum, sizeof(uint16_t), 1, ElfFile);
	fread(&elfHdr.e_shstrndx, sizeof(uint16_t), 1, ElfFile);
	*/

	// ecriture de e_ident (Magique)
	int i;
	for(i=0;i<16;i++){
		if(elfHdr.e_ident[i]/16 == 0){
			printf(" 0%x", elfHdr.e_ident[i]);
		}else{
			printf(" %x",elfHdr.e_ident[i]);
		}
	}
	printf(" \n");



	// ecriture de e_type
	
	elfHdr.e_type = __bswap_16(elfHdr.e_type);
	switch(elfHdr.e_type){
		case ET_NONE :
			printf("Type : No file type");
			break;
		case ET_REL :
			printf("Type : Relocatable file");
			break;
		case 2 :
			printf("Type : Executable file");
			break;
		case 3 :
			printf("Type : Shared object file");
			break;
		case 4 :
			printf("Type : Core file");
			break;
		case 0xff00 :
			printf("Type : Processor-specific");
			break;
		case 0xffff :
			printf("Type : Processor-specific");
			break;
		default :
			printf("Il y a une erreur sur le type du fichier");
			break;

	}
	printf(" \n");

	// ecriture de e_machine
	elfHdr.e_machine = __bswap_16(elfHdr.e_machine);

	printf(" \n");

	// ecriture de e_version
	printf("version :  %x", elfHdr.e_version);

}


int main(){

FILE* fichier = fopen("Examples_loader/example1.o","r");
creation_en_tete(fichier);


return 0;
}
