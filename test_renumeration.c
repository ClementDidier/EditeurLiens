#include "renumeration_section.h"
#include "correction_symboles.h"

int main(int argc, char * argv[])
{
	FILE* f = NULL, *fres = NULL;
	if((f = fopen(argv[1], "r")) == NULL)
	{
		printf("Erreur lors de la lecture du fichier ELF");
		return -1;
	}
	
	// Lecture dans le ficheir 
	read_header(f);
	read_Shdr_list( f );
	read_Sym_list( f );
	// Suppression des sections de relocation 
	enlever_relocation();
	printf("HEADER  : \n\te_type : %d\n\te_machine : %d\n\te_version : %d\n\te_entry : %d\n\te_phoff : %d\n\te_shoff : %d\n\te_flags : %d\n\te_ehsize : %d\n\te_phentsize : %d\n\te_phnum : %d\n\te_shentsize : %d\n\te_shnum : %d\n\te_shstrndx : %d\n\n",
		header.e_type, header.e_machine, header.e_version, header.e_entry, header.e_phoff, header.e_shoff, header.e_flags, header.e_ehsize, header.e_phentsize, header.e_phnum, header.e_shentsize, header.e_shnum, header.e_shstrndx);
		
	// Affichage a l'ecran 
	//afficher_Shdr_list(shdr_list);
	//afficher_Shdr_list(rel_list);
	afficher_tableau_sections();
	
	// Correction des symboles : 
	//correction_symboles();
	
	// Ecriture dans un fichier 
	
	if((fres = fopen("elfres.exe", "w")) == NULL)
	{
		printf("Erreur lors de la lecture du fichier ELF");
		return -1;
	}
	
	write_Elf32_Ehdr( fres, header );
	write_Shdr_list( fres );
	
	fclose(f);
	fclose(fres);
	return 0;
}
