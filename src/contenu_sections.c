#include "contenu_sections.h"

//Procédure d'écriture du contenu de la section
void ecrire_contenu(FILE *f,char strg[N],int size){
	printf("\nHex dump of section '%s' :\n",strg);
	unsigned char x;
	int i,j=0;
	for(i=0;i<size;i++){
		fscanf(f,"%c",&x);
		j++;
		printf("%.02x",x);
		if(j%4==0)printf(" ");
		if(j%16==0)printf("\n");
	}
	printf("\n\n");
}

int main(int argc, char *argv[]) {
	
	if(argc==3){
		FILE *elf_file;
				
		if((elf_file = fopen(argv[1],"r"))!= NULL){
		
			if(isdigit(argv[2][0])){
				unsigned int index,offset,size, base;

				index = atoi(argv[2]);
				//On récupère le ELF_header
				Elf32_Ehdr elfHeader;				
				read_Elf32_Ehdr(elf_file,&elfHeader);

				//On vérifie que la section recherchée existe avant de continuer
				if(index>elfHeader.e_shnum){
					printf("Warning section %d does not exit\n",index);
					return 0;
				}				
				
				//On calcul la position du section header de la section demandée et on y va
				offset = elfHeader.e_shoff + (index*elfHeader.e_shentsize);
				fseek(elf_file,offset,SEEK_SET);
			
				//On récupère le header de la section
				Elf32_Shdr elfSections;				
				read_Elf32_Shdr(elf_file,elfHeader,index,&elfSections);

				//On récupère l'offset de la section demandée, et sa taille
				offset = elfSections.sh_offset;
				size = elfSections.sh_size;
			
				// Recherche de l'offset de la table des noms de sections
				fseek( elf_file, elfHeader.e_shoff + elfHeader.e_shstrndx*elfHeader.e_shentsize, SEEK_SET );
				fseek( elf_file, 16, SEEK_CUR );
				fread( &base, 4, 1, elf_file );
				l2b_endian_32( &base );

				unsigned int index_nom=elfSections.sh_name;
				char strg[N];
				
				fseek(elf_file, base + index_nom, SEEK_SET );
				fscanf(elf_file,"%143s", strg);
				//Si la taille de la section est zero on ne cherche pas à écrire son contenu
				if(size==0){
					printf("Section '%s' has no data to dump\n",strg);
				}
				else{
					fseek(elf_file,offset,SEEK_SET);
					ecrire_contenu(elf_file,strg,size);
				}
			}
			else{
				char name[N],strg[N];
				strcpy(name,argv[2]);
				unsigned int i,offset,base,size,index_nom;

				
				//On récupère le ELF_header
				Elf32_Ehdr elfHeader;				
				read_Elf32_Ehdr(elf_file,&elfHeader);

				//Trouver le header de la section de nom donné
				i=0;				
				while(i<elfHeader.e_shnum && strcmp(name,strg)!=0){
					fseek( elf_file, elfHeader.e_shoff + i*elfHeader.e_shentsize , SEEK_SET );
					fread(&index_nom, 4,1,elf_file);
					l2b_endian_32( &index_nom);
			
					fseek( elf_file, elfHeader.e_shoff + elfHeader.e_shstrndx*elfHeader.e_shentsize, SEEK_SET );
					fseek( elf_file, 16, SEEK_CUR );
					fread( &base, 4, 1, elf_file );
					l2b_endian_32( &base );
					fseek(elf_file, base + index_nom, SEEK_SET );
					fscanf(elf_file,"%143s", strg);
	
					i++;
				}

				if(strcmp(name,strg)==0){
					i--;
					fseek( elf_file, elfHeader.e_shoff + i*elfHeader.e_shentsize +16 , SEEK_SET );
					
					fread(&offset,4,1,elf_file);
					fread(&size,4,1,elf_file);
					l2b_endian_32(&offset);
					l2b_endian_32(&size);

					if(size==0){
						printf("Section '%s' has no data to dump\n",strg);
					}
					else{
						fseek(elf_file,offset,SEEK_SET);
						ecrire_contenu(elf_file,strg,size);
					}
				}
				else{
					printf("\nWarning section %s does not exist\n",name);
				}				
			}	
		}
		else{
			//Erreur lors de l'ouverture du fichier
			printf("Erreur lors de l'ouverture du fichier ELF\n");
		}		
	}
	else{
		//Pas le bon nombre d'arguments
		printf("Not enough arguments\n");	
	}	

	return 0;
}
