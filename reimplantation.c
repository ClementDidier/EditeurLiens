#include "API.h"


//Fonction pour récupérer l'addend d'un symbole
/*
int32_t addend (Elf32_Shdr_Content *rel_content, int indice, int info){

	int i;
	Shdr_list *list;	
	Elf32_Shdr_Content *c = rel_content;

	for(i = 0; i < indice; i++)
	{
		c = c->next;
	}
	
	list = find_section(info);
	afficher_Shdr(list);
	int j;	
	int addresse=0;
	
	for (j=0;j<4;j++){
		addresse = addresse << 8;	
		addresse |= list->dump[c->offset+j]; 
	}
	
	printf("Addend : \n %8x",addresse );
	return addresse;
}*/

//parametre content_list + info
void ARM(Elf32_Shdr_Content *rel_content, int info){
	
	Elf32_Shdr_Content *c = rel_content;
	Shdr_list *list;
	int32_t S,A,result,mask;
	
	int j;	
	int addresse;

	while(c!=NULL)
	{
		
		printf("info : %x\n", info);
		//Récupération de S l'addresse du symbole
		S = sym_list.list[c->sym].st_value;

		//Calcul de l'addend
		list = find_section(info);
		addresse = 0;
		for (j=0;j<4;j++){
			addresse = addresse << 8;
			addresse |= list->dump[c->offset+j];
		}
	
		printf("Addend :  %08x\n",addresse );
		A = addresse;
				
		result = S+A;

		for (j=3;j>=0;j--){
			mask = 0x000000ff;
			printf(" Result : %08x \t Offset+j : %08x \n",result,c->offset+j);
			list->dump[c->offset+j] = result & mask;  			
			result = result >> 8;
		}
		
		
		c = c->next;
	}	
}

//Réimplantation des types R_ARM_ABS
//parametre shdr_list : rel_list
//appel arm pour chaque content_list avec info
void ARM_ABS(Shdr_list *rel_list){
	int cmpt=0,info;
	Shdr_list *copie_shdr = rel_list;
	Elf32_Shdr_Content *c;
	c = malloc(sizeof(Elf32_Shdr_Content));
	while (copie_shdr != NULL){
		read_Elf32_Shdr_Content(rel_list,cmpt,c);
		info = copie_shdr->header.sh_info;
		info = num_sections[info];
		ARM(c,info);		
		copie_shdr = copie_shdr->next;
		cmpt++;	
	}
}
