#include "./src/Programme_Editeur_Liens/API.h"

int main(int argc, char * argv[])
{
	FILE* f = NULL;
	if((f = fopen(argv[1], "r")) == NULL)
	{
		printf("Erreur lors de la lecture du fichier ELF\n");
		return -1;
	}
	
	//Initialisations des variables utiles
	Elf32_Ehdr h;
	Shdr_list shdr_list;
	Sym_list  sym_list;
	unsigned char ** names;
	
	// Lecture dans le fichier 
	read_Elf32_Ehdr(f,&h);
	names = sections_names_table(f,h);	
	read_Shdr_list(f, h, &shdr_list);
	read_Sym_list(f, h, &sym_list, shdr_list, names);

//Affichage du fichiers
	char menu;

		int boolean = 1;
		while(boolean){
			printf("\nQue voulez-vous afficher ? \n\n");
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
					afficher_Shdr_list( &shdr_list, f , h , names, &shdr_list);
					break;
				case 's':
					afficher_Sym_list(sym_list, f , h , names, &shdr_list);
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
					afficher_Shdr_list( &shdr_list, f , h , names, &shdr_list);
					afficher_reimplantation(h, &shdr_list, names);
					afficher_Sym_list(sym_list, f , h , names, &shdr_list);
					break;
				case 'q':
					boolean = 0;
					break;
			}

		}
		printf("\n");

	
	/*
	afficher_en_tete(h);
	afficher_Shdr_list( &shdr_list, f , h , names, &shdr_list);
	afficher_reimplantation(h, &shdr_list, names);
	afficher_Sym_list(sym_list, f , h , names, &shdr_list);*/
	
	fclose(f);
}
