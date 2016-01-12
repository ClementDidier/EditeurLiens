#include "renumeration_section.h"

// prend une structure de type Sdr_list et supprime les sections REL ou RELA
Shdr_list * enlever_relocation(Elf32_Ehdr h, Shdr_list * l, Shdr_list * rel_list, int * num_sections)
{
	Shdr_list * copie = l->next, *prec = l, * copie_rel = rel_list, *L = l;
	int indice_section = 1; 
	int indice_saut = 1;
	
	unsigned int removed_size = 0; // Le nombre d'octets qu'on a supprime en enlevant des sections 
	
	while(copie != NULL)
	{
		if(copie->header.sh_type == SHT_REL)
		{
			//enlève cette section de shdr_list
			prec->next = copie->next;
			// et on met a jour le nombre d'octets supprimes 
			removed_size += (unsigned int)copie->header.sh_size;
			
			// ajout de la section dans rel_list
			if(copie_rel == NULL)
			{
				copie_rel = copie;
				rel_list = copie_rel;
			}
			else
			{
				copie_rel->next = copie;
				copie_rel = copie_rel->next;
				
			}
			copie = copie->next;
			copie_rel->next = NULL;
			num_sections[indice_section] = -1;
		}
		else
		{
			// avancer dans la liste chainee
			prec = copie;
			copie = copie->next;
			num_sections[indice_section] = indice_saut;
			indice_saut++;
		}
		indice_section++;

	}
	
	num_sections[indice_section] = indice_saut;
	h.e_shnum = indice_saut;
	h.e_shstrndx = num_sections[h.e_shstrndx];
	h.e_type = ET_EXEC;
	
	
	
	// mise à jour de l'offset des sections et de sh_link
	while( L != NULL )
	{
		L->header.sh_link = (unsigned int)num_sections[L->header.sh_link];
		L = L->next;	
	}
	
	return rel_list;
}

void afficher_tableau_sections(int * num_sections, int size_num_sections)
{
	int i;
	printf("table des sections : \n");
	for(i = 0; i < size_num_sections; i++)
	{
		printf("table[%d] = %d\n", i, num_sections[i]);
	}
}


