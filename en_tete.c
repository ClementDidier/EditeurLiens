#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <byteswap.h>
#include "en_tete.h"


void creation_en_tete(FILE* ElfFile){

	// enregistrement de la structure de donnee elfhdr

	 Elf32_Ehdr elfHdr;
	//fread(&elfHdr, 1, sizeof elfHdr, ElfFile);

	
	int i;
	for(i=0;i<16;i++){
		fread(&elfHdr.e_ident[i], 1, 1, ElfFile);
	}
	if(elfHdr.e_ident[5] == ELFDATA2LSB){	//little indian
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
	}else if (elfHdr.e_ident[5] == ELFDATA2MSB) {
		fread((uint16_t*)&elfHdr.e_type, sizeof(uint16_t), 1, ElfFile);
		elfHdr.e_type = __bswap_16(elfHdr.e_type);
		fread(&elfHdr.e_machine, sizeof(uint16_t), 1, ElfFile);
		elfHdr.e_machine = __bswap_16(elfHdr.e_machine);
		fread(&elfHdr.e_version, sizeof(uint32_t), 1, ElfFile);
		elfHdr.e_version = __bswap_32(elfHdr.e_version);
		fread(&elfHdr.e_entry, sizeof(uint32_t), 1, ElfFile);
		elfHdr.e_entry = __bswap_32(elfHdr.e_entry);
		fread(&elfHdr.e_phoff, sizeof(uint32_t), 1, ElfFile);
		elfHdr.e_phoff = __bswap_32(elfHdr.e_phoff);
		fread(&elfHdr.e_shoff, sizeof(uint32_t), 1, ElfFile);
		elfHdr.e_shoff = __bswap_32(elfHdr.e_shoff);
		fread(&elfHdr.e_flags, sizeof(uint32_t), 1, ElfFile);
		elfHdr.e_flags = __bswap_32(elfHdr.e_flags);
		fread(&elfHdr.e_ehsize, sizeof(uint16_t), 1, ElfFile);
		elfHdr.e_ehsize = __bswap_16(elfHdr.e_ehsize);
		fread(&elfHdr.e_phentsize, sizeof(uint16_t), 1, ElfFile);
		elfHdr.e_phentsize = __bswap_16(elfHdr.e_phentsize);
		fread(&elfHdr.e_phnum, sizeof(uint16_t), 1, ElfFile);
		elfHdr.e_phnum = __bswap_16(elfHdr.e_phnum);
		fread(&elfHdr.e_shentsize, sizeof(uint16_t), 1, ElfFile);
		elfHdr.e_shentsize = __bswap_16(elfHdr.e_shentsize);
		fread(&elfHdr.e_shnum, sizeof(uint16_t), 1, ElfFile);
		elfHdr.e_shnum = __bswap_16(elfHdr.e_shnum);
		fread(&elfHdr.e_shstrndx, sizeof(uint16_t), 1, ElfFile);
		elfHdr.e_shstrndx = __bswap_16(elfHdr.e_shstrndx);
	} else {

		printf("erreur dans le codage utilisé");
	}
	

	// ecriture de e_ident (Magique)
	printf("e_ident : ");
	for(i=0;i<16;i++){
		if(elfHdr.e_ident[i]/16 == 0){
			printf(" 0%x", elfHdr.e_ident[i]);
		}else{
			printf(" %x",elfHdr.e_ident[i]);
		}
	}
	printf(" \n");



	// ecriture de e_type
	
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
	printf("\n");

	// ecriture de e_machine

	switch(elfHdr.e_machine){
		case EM_NONE :
			printf("Machine : machine inconne");
			break;
		case EM_M32 :
			printf("Machine : AT&T WE 32100");
			break;
		case EM_SPARC :
			printf("Machine : Sun Microsystems SPARC");
			break;
		case EM_386 :
			printf("Machine : Intel 80386");
			break;
		case EM_68K :
			printf("Machine : Motorola 68000");
			break;
		case EM_88K :
			printf("Machine : Motorola 88000");
			break;
		case EM_860 :
			printf("Machine : Intel 80860");
			break;
		case EM_MIPS :
			printf("Machine : MIPS RS3000");
			break;
		case EM_PARISC :
			printf("Machine : HP/PA");
			break;
		case EM_SPARC32PLUS :
			printf("Machine : SPARC avec jeu d'instruction étendu");
			break;
		case EM_PPC :
			printf("Machine : PowerPC");
			break;
		case EM_PPC64 :
			printf("Machine : PowerPC 64 bits");
			break;
		case EM_S390 :
			printf("Machine : IBM S/390");
			break;
		case EM_ARM :
			printf("Machine : ARM RISC");
			break;
		case EM_SH :
			printf("Machine : Renesas SuperH");
			break;
		case EM_SPARCV9 :
			printf("Machine : SPARCC v9 64 bits");
			break;
		case EM_IA_64 :
			printf("Machine : Intel Itanium");
			break;
		case EM_X86_64 :
			printf("Machine : AMD x86-64");
			break;
		case EM_VAX :
			printf("Machine : DEC Vax");
			break;
		default :
			printf("Il y a une erreur sur la machine du fichier");
			break;

	}
	printf("\n");

	// ecriture de e_version
	switch(elfHdr.e_version){
		case 0 :
			printf("Version : %d (Invalide Version)\n",elfHdr.e_version);
			break;
		case 1 :
			printf("Version : %d (Current Version)\n",elfHdr.e_version);
			break;
	}

	// ecriture de e_entry
	printf("Entry point address : 0x%x\n",elfHdr.e_entry);

	// ecriture de e_phoff
	printf("Phoff : %d\n", elfHdr.e_phoff);

	// ecriture de e_shoff
	printf("Shoff : %d\n", elfHdr.e_shoff);

	// ecriture de e_flags
	printf("Flags : 0x%x\n",elfHdr.e_flags);

	// ecriture de e_shentsize
	printf("Taille de section headers : %d (bytes)\n",elfHdr.e_shentsize);

	// ecriture de e_shnum
	printf("Nombre de section headers : %d\n",elfHdr.e_shnum);

	// ecriture de e_shstrndx
	printf("L'index de la table des section headers : %d\n",elfHdr.e_shstrndx);
	
	// ecriture de e_ehsize
	printf("Taille de l'en-tête (en octets) : %d", elfHdr.e_ehsize);
	printf("\n");

	// ecriture de phentsize
	printf("Taille d'une entrée de la table d'en tête (en octets) : %d", elfHdr.e_phentsize);
	printf("\n");

	// ecriture de e_phnum
	printf("Nombre d'entrée de la table d'en tête : %d", elfHdr.e_phnum);
	printf("\n");
	int resultat = elfHdr.e_phentsize * elfHdr.e_phnum ; 
	printf("Taille de la table (en octets) : %d", resultat);
	printf("\n");



}


int main(int argc, char **argv){

	FILE* fichier = fopen(argv[1],"r");
	creation_en_tete(fichier);

	return 0;

}
