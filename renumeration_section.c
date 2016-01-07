#include "renumeration_section.h"


// prend une structure de type Sdr_list et supprime les sections REL ou RELA
void enlever_relocation()
{
	Shdr_list * copie = shdr_list;
	int indice_suppression = 0;
	int indice_section = 0;
	int indice_saut = 0;
	while(copie != NULL){

		if((copie->header.sh_type == SHT_REL) || (copie->header.sh_type == SHT_RELA)){
			copie = copie->next;
			indice_suppression++;
			num_sections[indice_section] = -1;
		}else{
			if(copie->next !=NULL){
				if((copie->next->header.sh_type == SHT_REL) || (copie->next->header.sh_type == SHT_RELA)){
					copie->next = copie->next->next;
					num_sections[indice_section+1] = -1;
					num_sections[indice_section] = indice_saut;
					indice_saut++;
					indice_suppression++;
					indice_section++;
				}else{
					num_sections[indice_section] = indice_saut;
					indice_saut++;
				}
			}else{
				num_sections[indice_section] = indice_saut;
				indice_saut++;
			}

			copie = copie->next;
		}
		indice_section++;
	}

	header.e_shnum = header.e_shnum - indice_suppression;

}


void afficher_tableau()
{
	int i;
	printf("table des sections : \n");
	for(i = 0; i < size_num_sections; i++){
		printf("table[%d] = %d\n", i, num_sections[i]);
	}



}

int main(int argc, char * argv[])
{
	FILE* f = NULL;
	if((f = fopen(argv[1], "r")) == NULL)
	{
		printf("Erreur lors de la lecture du fichier ELF");
		return -1;
	}
	

	
	read_header(f);
	read_Shdr_list( f );
	enlever_relocation();
	afficher_Shdr_list();
	afficher_tableau();
	printf("HEADER  : \n\te_type : %d\n\te_machine : %d\n\te_version : %d\n\te_entry : %d\n\te_phoff : %d\n\te_shoff : %d\n\te_flags : %d\n\te_ehsize : %d\n\te_phentsize : %d\n\te_phnum : %d\n\te_shentsize : %d\n\te_shnum : %d\n\te_shstrndx : %d\n\n",
		header.e_type, header.e_machine, header.e_version, header.e_entry, header.e_phoff, header.e_shoff, header.e_flags, header.e_ehsize, header.e_phentsize, header.e_phnum, header.e_shentsize, header.e_shnum, header.e_shstrndx);
	
	fclose(f);
	return 0;
}
