#include "API.h"
#include "reimplantation.h"
#include "correction_symboles.h"
#include "renumeration_section.h"
#include "arm_simulator_interface.h"
#include <stdlib.h>
#include <stdio.h>
#include "debug.h"

void init_simulation( arm_simulator_data_t *sim ){
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
	
	printf("[+]Lecture du fichier elf...\n");	
	// Lecture dans le ficheir 
	read_header(f);
	read_Shdr_list( f );
	read_Sym_list( f );	
	// Suppression des sections de relocation 
	printf("[+]Séparation des sections de relocation (.rel) ...\n");
	enlever_relocation();
	
	L = shdr_list;
	// Mise a jour des adresses des sections
	while ( L != NULL ){
		L->header.sh_addr = addr;
		addr += L->header.sh_size + 4 - (L->header.sh_size % 4);
		L = L->next;
	}
	
	// Correction des symboles : 
	printf("[+]Correction des symboles...\n");
	correction_symboles();
	// Reimplantation
	printf("[+]Reimplantation...\n");
	reimplantation();
	
	// Ecriture dans un fichier 	
	if((fres = fopen("elfres.exe", "w")) == NULL)
	{
		printf("Erreur lors de la lecture du fichier ELF");
		return -1;
	}
	
	printf("[+]Création du fichier elf executable 'elfres.exe'...\n");
	write_Elf32_Ehdr( fres, header );
	write_Shdr_list( fres );
	
	fclose(f);
	fclose(fres);
	
	printf("[-]Transformation terminee, le resultat peut etre observe via 'readelf [opt] elfres.exe' \n\n");
	
	printf("[+]Simulation... \n");
	init_simulation( &sim );
	
	//arm_run( sim );
	
	while( getc( stdin ) != '0' )
		arm_step( sim );
	
	return 0;
}



