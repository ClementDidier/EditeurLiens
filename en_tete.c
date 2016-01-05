#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <byteswap.h>


void creation_en_tete(FILE* ElfFile){

	// enregistrement de la structure de donnee elfhdr
	Elf32_Ehdr elfHdr;
	fread(&elfHdr, 1, sizeof(elfHdr), ElfFile);
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
	elfHdr.e_version = __bswap_32(elfHdr.e_version);
	switch(elfHdr.e_version){
		case 0 :
			printf("Version : %d (Invalide Version)\n",elfHdr.e_version);
			break;
		case 1 :
			printf("Version : %d (Current Version)\n",elfHdr.e_version);
			break;
	}

	// ecriture de e_entry
	elfHdr.e_entry = __bswap_32(elfHdr.e_entry);
	printf("Entry point address : 0x%x\n",elfHdr.e_entry);

	// ecriture de e_phoff
	elfHdr.e_phoff = __bswap_32(elfHdr.e_phoff);
	printf("Phoff : %d\n", elfHdr.e_phoff);

	// ecriture de e_shoff
	elfHdr.e_shoff = __bswap_32(elfHdr.e_shoff);
	printf("Shoff : %d\n", elfHdr.e_shoff);

	// ecriture de e_flags
	elfHdr.e_flags = __bswap_32(elfHdr.e_flags);
	printf("Flags : 0x%x\n",elfHdr.e_flags);

	// ecriture de e_shentsize
	elfHdr.e_shentsize = __bswap_16(elfHdr.e_shentsize);
	printf("Taille de section headers : %d (bytes)\n",elfHdr.e_shentsize);

	// ecriture de e_shnum
	elfHdr.e_shnum = __bswap_16(elfHdr.e_shnum);
	printf("Nombre de section headers : %d\n",elfHdr.e_shnum);

	// ecriture de e_shstrndx
	elfHdr.e_shstrndx = __bswap_16(elfHdr.e_shstrndx);
	printf("L'index de la table des section headers : %d\n",elfHdr.e_shstrndx);
}


int main(){

FILE* fichier = fopen("Examples_loader/example1.o","r");
creation_en_tete(fichier);


return 0;
}
