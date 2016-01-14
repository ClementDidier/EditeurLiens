#include "API.h"
#include "reimplantation.h"
#include "correction_symboles.h"
#include "renumeration_section.h"
#include "arm_simulator_interface.h"
#include <stdlib.h>
#include <stdio.h>
#include "debug.h"


void init_simulation( arm_simulator_data_t *sim , Shdr_list *shdr_list){
	Shdr_list *L = shdr_list;
	*sim = arm_connect( "localhost", "6666" );
	while( L != NULL ){
		arm_write_memory(*sim, L->header.sh_addr, L->dump, L->header.sh_size);
		L = L->next;
	}
	arm_write_register( *sim, 15, 0x4 ); // R15 = PC 
	arm_write_register( *sim, 13, 0x1000 ); // R13 = SP
}

int main( int argc, char ** argv ){
	
	FILE* f = NULL, *fres = NULL;
	unsigned int addr = 0x0000;
	Shdr_list * L;
	arm_simulator_data_t sim = NULL;
	
	if((f = fopen(argv[1], "r")) == NULL)
	{
		printf("Erreur lors de la lecture du fichier ELF");
		return -1;
	}


	//Initialisations des variables utiles
	Elf32_Ehdr h;
	Shdr_list shdr_list, *S;
	Sym_list  sym_list;
	Shdr_list * rel_list = NULL;
	int * num_sections;
	int size_num_sections;
	unsigned char ** names;

	
	printf("[+]Lecture du fichier elf...\n");	
	// Lecture dans le ficheir 
	read_Elf32_Ehdr(f,&h);
	names = sections_names_table(f,h);	
	read_Shdr_list(f, h, &shdr_list);
	read_Sym_list(f, h, &sym_list, shdr_list,names);
	
	//Initialisation de num_sections
	num_sections = malloc((h.e_shnum) * sizeof(int));
	size_num_sections = h.e_shnum;
	int i;
	for( i = 0; i < size_num_sections ; i++)
		num_sections[i] = i;

	//Affichage du fichiers
	char menu;
	printf("Voulez-vous afficher les différentes structures du fichier d'entrée ? o ou n \n");
	scanf(" %c", &menu);
	while(menu != 'o' && menu != 'n'){
		printf("Ecrire o ou n s'il vous plaît \n");
		scanf(" %c", &menu);
	}
	if(menu == 'o'){
		int boolean = 1;
		while(boolean){
			printf("\nQuelle section voulez-vous afficher ? \n\n");
			printf("En-tête : h\n");
			printf("En-tête de section : S\n");
			printf("Table des symboles : s\n");
			printf("Section de relocation : r\n");
			printf("Affichage des sections : x\n");
			printf("Tout le fichier : a\n");
			printf("Ne rien afficher et accéder directement à la simulation : q\n");
			scanf(" %c", &menu);
			switch(menu){
				case 'h':
					afficher_en_tete(h);
					break;
				case 'S':
					afficher_Shdr_list(&shdr_list);
					break;
				case 's':
					afficher_Sym_list(sym_list);
					break;
				case 'r':
					afficher_reimplantation(h, &shdr_list, names);
					break;
				case 'x':
					printf("Ecrivez le nom ou le numéro de section : ");
					char *valeur;
					valeur= (char *) malloc(30*sizeof(char));
					scanf("%s", valeur);
					printf("valeur : %s", valeur);
					if(valeur[0]=='.'){
						afficher_section_name(&shdr_list,names, valeur);
					}else{
						afficher_section(&shdr_list,names, atoi(valeur));
					}
					free(valeur);
					break;
				case 'a':
					afficher_en_tete(h);
					afficher_Shdr(&shdr_list);
					afficher_reimplantation(h, &shdr_list, names);
					afficher_Sym_list(sym_list);
					break;
				case 'q':
					boolean = 0;
					break;
			}

		}
		printf("\n");

	}



	// Suppression des sections de relocation 
	printf("[+]Séparation des sections de relocation (.rel) ...\n");
	rel_list = enlever_relocation(&h, &shdr_list, rel_list, num_sections);	

	L = &shdr_list;
	// Mise a jour des adresses des sections
	while ( L != NULL ){
		L->header.sh_addr = addr;
		addr += L->header.sh_size + 4 - (L->header.sh_size % 4);
		L = L->next;
	}
	
	// Correction des symboles : 
	printf("[+]Correction des symboles...\n");
	correction_symboles(h, &shdr_list, &sym_list, num_sections);

	// Reimplantation
	printf("[+]Reimplantation...\n");
	reimplantation(h,rel_list,&shdr_list, sym_list,num_sections);

	// Point d'entree du fichier executable
	S = find_section_name( names, ".text", &shdr_list );
	h.e_entry = S->header.sh_addr;
	h.e_flags |= 0x2;
	
	// Ecriture dans un fichier
	if((fres = fopen("elfres.exe", "w")) == NULL)
	{
		printf("Erreur lors de la lecture du fichier ELF");
		return -1;
	}
	
	printf("[+]Création du fichier elf executable 'elfres.exe'...\n");
	write_Elf32_Ehdr( fres, h );
	write_Shdr_list( fres , h , &shdr_list );

	fclose(f);
	fclose(fres);
	
	printf("[-]Transformation terminee, le resultat peut etre observe via 'readelf [opt] elfres.exe' \n\n");
	
	printf("[+]Simulation... \n");
	init_simulation( &sim, &shdr_list );
	
	//arm_run( sim );
	
	while( getc( stdin ) != '0' )
		arm_step( sim );
	//
	//liberation des variables utilisées

	liberer_Shdr_list(&shdr_list);
	liberer_Shdr_list(rel_list);
	liberer_Sym_list(&sym_list);
	liberer_tab_name(names, size_num_sections);
	liberer_num_sections(num_sections);

	return 0;
}



