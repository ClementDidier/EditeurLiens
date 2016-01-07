#include "relocation.h"


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
			num_section[indice_section] = -1;
		}else{
			if(copie->next !=NULL){
				if((copie->next->header.sh_type == SHT_REL) || (copie->next->header.sh_type == SHT_RELA)){
					copie->next = copie->next->next;
					num_section[indice_section] = -1;
					indice_suppression++;
				}
			}else{
				num_section[indice_section] = indice_saut;
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
	for(i = 0; i < ; i++){
		printf("t");

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
	
	Elf32_Ehdr h;
	
	read_Elf32_Ehdr(f, &h);
	read_Shdr_list( f );
	enlever_relocation(shdr_list);
	afficher_Shdr_list();
	
	fclose(f);
	return 0;
}
