#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <byteswap.h>
#include "en_tete.h"


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
	printf("\n");

	// ecriture de e_machine

	elfHdr.e_machine = __bswap_16(elfHdr.e_machine);
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



	printf("\n");
	
	// ecriture de e_ehsize
	printf("Taille de l'en-tête (en octets) : %d", __bswap_16(elfHdr.e_ehsize));
	printf("\n");

	// ecriture de phentsize
	printf("Taille d'une entrée de la table d'en tête (en octets) : %d", __bswap_16(elfHdr.e_phentsize));
	printf("\n");

	// ecriture de e_phnum
	printf("Nombre d'entrée de la table d'en tête : %d", __bswap_16(elfHdr.e_phnum));
	printf("\n");
	int resultat = __bswap_16(elfHdr.e_phentsize) * __bswap_16(elfHdr.e_phnum) ; 
	printf("Taille de la table (en octets) : %d", resultat);
	printf("\n");



}


int main(int argc, char **argv){

	FILE* fichier = fopen(argv[1],"r");
	creation_en_tete(fichier);

	return 0;

}
