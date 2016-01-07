#include "API.h"
#include <stdio.h>
#include <elf.h>

int main(int argc, char * argv[])
{
	FILE* f = NULL;
	if((f = fopen(argv[1], "r")) == NULL)
	{
		printf("Erreur lors de la lecture du fichier ELF");
		return -1;
	}
	
	Elf32_Ehdr h;
	Elf32_Shdr s0, s1;
	
	read_Elf32_Ehdr(f, &h);
	read_Elf32_Shdr(f, h, 0, &s0);
	read_Elf32_Shdr(f, h, 1, &s1);

	printf("HEADER FICHIER 1 : \n\te_type : %d\n\te_machine : %d\n\te_version : %d\n\te_entry : %d\n\te_phoff : %d\n\te_shoff : %d\n\te_flags : %d\n\te_ehsize : %d\n\te_phentsize : %d\n\te_phnum : %d\n\te_shentsize : %d\n\te_shnum : %d\n\te_shstrndx : %d\n\n",
		h.e_type, h.e_machine, h.e_version, h.e_entry, h.e_phoff, h.e_shoff, h.e_flags, h.e_ehsize, h.e_phentsize, h.e_phnum, h.e_shentsize, h.e_shnum, h.e_shstrndx);
	
	h.e_shnum = 2;

	
	// Creation du fichier
	FILE * fresult = NULL;
	if((fresult = fopen("elfResult.o", "wb")) == NULL)
	{
		printf("Erreur lors de l'écriture du fichier ELF");
		return -1;
	}
	
	write_Elf32_Ehdr(fresult, h);
	//write_Elf32_Shdr(fresult, h, 0, s0);
	//write_Elf32_Shdr(fresult, h, 1, s1);
	
	Elf32_Ehdr hresult;
	read_Elf32_Ehdr(fresult, &hresult);
	
	printf("HEADER FICHIER RESULTAT : \n\te_type : %d\n\te_machine : %d\n\te_version : %d\n\te_entry : %d\n\te_phoff : %d\n\te_shoff : %d\n\te_flags : %d\n\te_ehsize : %d\n\te_phentsize : %d\n\te_phnum : %d\n\te_shentsize : %d\n\te_shnum : %d\n\te_shstrndx : %d\n\n",
		hresult.e_type, hresult.e_machine, hresult.e_version, hresult.e_entry, hresult.e_phoff, hresult.e_shoff, hresult.e_flags, hresult.e_ehsize, hresult.e_phentsize, hresult.e_phnum, hresult.e_shentsize, hresult.e_shnum, hresult.e_shstrndx);
	
	fclose(f);
	fclose(fresult);
	return 0;
}
