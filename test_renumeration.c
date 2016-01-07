#include "renumeration_section.h"


int main(int argc, char * argv[])
{
	FILE* f = NULL;
	if((f = fopen(argv[1], "r")) == NULL)
	{
		printf("Erreur lors de la lecture du fichier ELF");
		return -1;
	}
	

	
	read_header(f);
	read_Shdr_list( f );
	enlever_relocation();
	afficher_Shdr_list(shdr_list);
	afficher_Shdr_list(rel_list);
	afficher_tableau();
	printf("HEADER  : \n\te_type : %d\n\te_machine : %d\n\te_version : %d\n\te_entry : %d\n\te_phoff : %d\n\te_shoff : %d\n\te_flags : %d\n\te_ehsize : %d\n\te_phentsize : %d\n\te_phnum : %d\n\te_shentsize : %d\n\te_shnum : %d\n\te_shstrndx : %d\n\n",
		header.e_type, header.e_machine, header.e_version, header.e_entry, header.e_phoff, header.e_shoff, header.e_flags, header.e_ehsize, header.e_phentsize, header.e_phnum, header.e_shentsize, header.e_shnum, header.e_shstrndx);
	
	fclose(f);
	return 0;
}
