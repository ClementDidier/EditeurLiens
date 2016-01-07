#include "renumeration_section.h"


// prend une structure de type Sdr_list et supprime les sections REL ou RELA
void enlever_relocation()
{
	Shdr_list * copie = shdr_list->next, *prec = shdr_list, *copie_rel = NULL;
	int indice_section = 0;
	int indice_saut = 0;
	
	while(copie != NULL){
		if(copie->header.sh_type == SHT_REL){
			//enlève cette section de shdr_list
			prec->next = copie->next;
			 
			// ajout de la section dans rel_list
			if(copie_rel ==NULL){
				copie_rel = copie;
				rel_list = copie_rel;
			}else{
				copie_rel->next = copie;
				copie_rel = copie_rel->next;
			}

			copie = copie->next;
			copie_rel->next = NULL;
			num_sections[indice_section] = -1;
		}else{
			prec = copie;
			copie = copie->next;
			num_sections[indice_section] = indice_saut;
			indice_saut++;
		}
		indice_section++;

	}

	num_sections[indice_section] = indice_saut;
	

	header.e_shnum = indice_saut;

}


void afficher_tableau()
{
	int i;
	printf("table des sections : \n");
	for(i = 0; i < size_num_sections; i++){
		printf("table[%d] = %d\n", i, num_sections[i]);
	}



}


