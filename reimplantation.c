#include "API.h"


//ecrit à l'offset de c en parametre adresse
void ecrire(Elf32_Shdr_Content *c, int32_t result, Shdr_list *list){
	int j;	
	int32_t mask;
	printf("\nResult : %08x \n",result);
	for (j=3;j>=0;j--){
		mask = 0x000000ff;
		list->dump[c->offset+j] = result & mask;  			
		result = result >> 8;
	}

}


//parametre content_list + info
void ARM(Elf32_Shdr_Content *rel_content, int info){
	
	Elf32_Shdr_Content *c = rel_content;
	Shdr_list *list;
	int32_t S,A,P,result;
	
	int j;	
	int addresse;

	while(c!=NULL)
	{
		
		printf("info : %x\n", info);

		//Calcul des différentes valeurs necessaire : S, A, T, et P (T est égal à 0 dans notre cas)

		//Récupération de S l'addresse du symbole
		S = sym_list.list[c->sym].st_value;

		//Calcul de l'addend
		list = find_section(info);
		addresse = 0;
		for (j=0;j<4;j++){
			addresse = addresse << 8;
			addresse |= list->dump[c->offset+j];
		}
	
		printf("Addend :  %08x\nS : %08x\n",addresse,S );
		A = addresse;
		
		//Calcul de P
		P = c->offset;

		switch (c->type){
			case 2 :			//ABS_32
				result = S + A;
				ecrire(c,result,list);
				break;		
			case 5 :			//ABS_16
				result = S + A;
				ecrire(c,result,list);
				break;		
			case 8 :			//ABS_8
				result = S + A;
				ecrire(c,result,list);
				break;		
			case 28 :			//JUMP24
				result = (S + A) - P;
				ecrire(c,result,list);
				printf("\nP : %08x\n", P);
				break;	
			case 29 :			//ARMCALL
				result = (S + A) - P;
				ecrire(c,result,list);
				printf("\nP : %08x\n", P);
				break;
		}
		c = c->next;
	}	
}

//Réimplantation des types R_ARM_ABS
//parametre shdr_list : rel_list
//appel arm pour chaque content_list avec info
void ARM_ABS(Shdr_list *rel_list, int *num_sections){
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
