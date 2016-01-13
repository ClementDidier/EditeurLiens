#include "../partie2/renumeration_section.h"
#include "../partie2/correction_symboles.h"

int main(int argc, char * argv[])
{
	FILE* f = NULL, *fres = NULL;
	if((f = fopen(argv[1], "r")) == NULL)
	{
		printf("Erreur lors de la lecture du fichier ELF");
		return -1;
	}
	
	printf("[+]Lecture du fichier elf...\n");
	
	Elf32_Ehdr header;
	Shdr_list shdr_list;
	Sym_list  sym_list;
	Shdr_list * rel_list = NULL;
	int * num_sections;
	int size_num_sections;
	
	// Lecture dans le fichier 
	read_Elf32_Ehdr(f, &header);
	
	char ** names;
	names = sections_names_table(f,header);	
	
	num_sections = malloc((header.e_shnum) * sizeof(int));
	size_num_sections = header.e_shnum;
	int i;
	for( i = 0; i < size_num_sections ; i++)
		num_sections[i] = i;
	
	read_Shdr_list(f, header, &shdr_list);

	sym_list = read_Sym_list(f, header, &sym_list, shdr_list,names);
	afficher_Sym_list(sym_list);
	//Ensemble d'instructions pour récupérer et charger les adresses dans le fichier ELF
	//Rappel : en paramètre on a 
	//"nom de section" "adresse de chargement"
	
	Shdr_list * x;	
	int j;	
	for(j=2;j<argc;j+=2){
		x = find_section_name(names,argv[j],&shdr_list);
		x->header.sh_addr = char_to_hex(argv[j+1]);
	}
	
	// Suppression des sections de relocation 
	printf("[+]Séparation des sections de relocation (.rel) ...\n");
	rel_list = enlever_relocation(header, &shdr_list, rel_list, num_sections);
	// Affichage a l'ecran 
	//afficher_Shdr_list(&shdr_list);
	//printf("rel_list.header : %d\n", rel_list->header.sh_name);
	//afficher_Shdr_list(rel_list);
	//afficher_tableau_sections(num_sections, size_num_sections);
	
	// Correction des symboles : 
	printf("[+]Correction des symboles...\n");
	correction_symboles(header, &shdr_list, &sym_list, num_sections);
	
	// Ecriture dans un fichier 
	
	if((fres = fopen("elfres.exe", "w")) == NULL)
	{
		printf("Erreur lors de la lecture du fichier ELF");
		return -1;
	}
	
	printf("[+]Création du fichier elf executable 'elfres.exe'...\n");
	write_Elf32_Ehdr(fres, header);
	write_Shdr_list(fres, header, &shdr_list);
	
	fclose(f);
	fclose(fres);
	
	printf("[-]Transformation terminee, le resultat peut etre observe via 'readelf [opt] elfres.exe' \n\n");
	
	return 0;
}
