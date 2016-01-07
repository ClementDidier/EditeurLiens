#include "correction_symboles.h"

//Rappel :
//sym_list est global
//tableau d'index sections est global
//le header est global
//

void correction_symboles(){
	int i;
	Shdr_list *L;
	Shdr_list *S = find_symbols_section();
	void* dump = S->dump;
	// Correction 
	for(i=0;i<sym_list.nb;i++){
		// Re-indexation des sections correspondants au symbole
		sym_list.list[i].st_shndx = num_sections[sym_list.list[i].st_shndx];
		// Calcul de la valeur du symbole, i.e son adresse qui vaut offset + adresse de chargement de sa section correspondante 
		L = find_section((unsigned int)sym_list.list[i].st_shndx);
		sym_list.list[i].st_value += L->header.sh_addr;
		// Ecriture dans la section
		((uint32_t*)dump)[0] = __bswap_32( sym_list.list[i].st_name );
		((uint32_t*)dump)[1] = __bswap_32( sym_list.list[i].st_value );
		((uint32_t*)dump)[2] = __bswap_32( sym_list.list[i].st_size );
		((unsigned char*)dump)[12] = sym_list.list[i].st_info;
		((unsigned char*)dump)[13] = sym_list.list[i].st_other;
		((uint16_t*)dump)[7] = __bswap_16( sym_list.list[i].st_shndx );
	}	
}
