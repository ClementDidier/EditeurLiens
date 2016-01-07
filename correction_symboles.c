#include "API.h"

//Rappel :
//sym_list est global
//tableau d'index sections est global
//le header est global
//

void correction_symboles(){
	int i,addr;
	Shdr_list *L;
	for(i=0;i<sym_list.nb;i++){
		sym_list.list[i].st_shndx = tab_sections[i];
	
		L = find_section(i);
		sym_list.list[i].st_value += L->header.sh_addr;		
	}
}
