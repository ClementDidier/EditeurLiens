#include "API.h"

// Retourne 1 si l'endianess du header est en big endian, 0 dans le cas contraire
int my_is_big_endian(Elf32_Ehdr h)
{
	return h.e_ident[ENDIANESS_VALUE_INDEX] == ELFDATA2MSB;
}

// Swap conditionnel de la valeur sur 32 bits, en fonction de l'endianess
int32_t recuperer_valeur32(Elf32_Ehdr h, int32_t value)
{
	return my_is_big_endian(h) ? __bswap_32(value) : value;
}

// Swap conditionnel de la valeur sur 16 bits, en fonction de l'endianess
int16_t recuperer_valeur16(Elf32_Ehdr h, int16_t value)
{
	return my_is_big_endian(h) ? __bswap_16(value) : value;
}

// Lis le header du fichier ELF
int read_Elf32_Ehdr( FILE *f, Elf32_Ehdr * h )
{
	unsigned long int size = sizeof(Elf32_Ehdr);
	rewind(f); //fseek(f, 0, SEEK_SET);
	
	int rb = fread( h, size , 1, f);

	// Inversion de l'endianess 
	h->e_type 		= recuperer_valeur16(*h, h->e_type);
	h->e_machine 	= recuperer_valeur16(*h, h->e_machine);
	h->e_version 	= recuperer_valeur32(*h, h->e_version);
	h->e_entry 		= recuperer_valeur32(*h, h->e_entry);
	h->e_phoff 		= recuperer_valeur32(*h, h->e_phoff);
	h->e_shoff 		= recuperer_valeur32(*h, h->e_shoff);
	h->e_flags 		= recuperer_valeur32(*h, h->e_flags);
	h->e_ehsize 	= recuperer_valeur16(*h, h->e_ehsize);
	h->e_phentsize 	= recuperer_valeur16(*h, h->e_phentsize);
	h->e_phnum 		= recuperer_valeur16(*h, h->e_phnum);
	h->e_shentsize 	= recuperer_valeur16(*h, h->e_shentsize);
	h->e_shnum 		= recuperer_valeur16(*h, h->e_shnum);
	h->e_shstrndx 	= recuperer_valeur16(*h, h->e_shstrndx);

	return rb;
}
void afficher_Elf32_Ehdr( Elf32_Ehdr h )
{
	printf("HEADER  : \n\te_type : %d\n\te_machine : %d\n\te_version : %d\n\te_entry : %d\n\te_phoff : %d\n\te_shoff : %d\n\te_flags : %d\n\te_ehsize : %d\n\te_phentsize : %d\n\te_phnum : %d\n\te_shentsize : %d\n\te_shnum : %d\n\te_shstrndx : %d\n\n",
		h.e_type, h.e_machine, h.e_version, h.e_entry, h.e_phoff, h.e_shoff, h.e_flags, h.e_ehsize, h.e_phentsize, h.e_phnum, h.e_shentsize, h.e_shnum, h.e_shstrndx);	
}
// Lis un header de section 
int read_Elf32_Shdr( FILE *f, Elf32_Ehdr h, unsigned int index, Elf32_Shdr * s)
{
	unsigned long int size = sizeof(Elf32_Shdr);
	fseek(f, h.e_shoff + size * index, SEEK_SET);
	int rb = fread(s, size, 1, f);
	
	// Inversion de l'endianess 
	s->sh_name 		= recuperer_valeur32(h, s->sh_name);
	s->sh_type 		= recuperer_valeur32(h, s->sh_type);
	s->sh_flags 	= recuperer_valeur32(h, s->sh_flags);
	s->sh_addr		= recuperer_valeur32(h, s->sh_addr);
	s->sh_offset 	= recuperer_valeur32(h, s->sh_offset);
	s->sh_size 		= recuperer_valeur32(h, s->sh_size);
	s->sh_link 		= recuperer_valeur32(h, s->sh_link);
	s->sh_info 		= recuperer_valeur32(h, s->sh_info);
	s->sh_addralign = recuperer_valeur32(h, s->sh_addralign);
	s->sh_entsize 	= recuperer_valeur32(h, s->sh_entsize);
	
	return rb;
}

// Lis une structure Elf32_Sym
// Renvoie le nombre d'octets lus 
// Le placement du curseur doit etre effectué avant l'appel de la fonction
int read_Elf32_Sym( FILE *f, Elf32_Ehdr h, Elf32_Sym *s)
{
	unsigned long int size = sizeof(Elf32_Sym);
	int rb = fread( s, size , 1, f);
	
	// Inversion big/little sur les champs de plus d'un octet 
	s->st_name = recuperer_valeur32(h, s->st_name);
	s->st_value = recuperer_valeur32(h, s->st_value);
	s->st_size = recuperer_valeur32(h, s->st_size);
	s->st_shndx = recuperer_valeur16(h, s->st_shndx);
	
	return rb;
}


// Lecture de l'ensemble des Section Headers  
void read_Shdr_list(FILE *f, Elf32_Ehdr h, Shdr_list * L)
{
	int i;

	Shdr_list * Q = L, *N;
	L->next = NULL;
	
	read_Elf32_Shdr(f, h, 0, &(L->header));
	for( i = 1; i < h.e_shnum; i++ )
	{
		N = malloc( sizeof(Shdr_list) );
		// Lecture du header
		read_Elf32_Shdr(f, h, i, &(N->header));
		// Lecture du dump raw de la section
		fseek( f, N->header.sh_offset, SEEK_SET );
		N->dump = malloc( N->header.sh_size );
		fread( N->dump, N->header.sh_size, 1, f); 
		// Passage a la section suivante 
		Q->next = N;
		Q = N;
		N->next = NULL;
	}
}

void afficher_Shdr( Shdr_list * l)
{
	Elf32_Shdr s = l->header;
	int i;
	printf("  [+]Header\n\tsh_name : %d\n\tsh_type : %d\n\tsh_addr : %d\n\tsh_offset : %d\n\tsh_size : %d\n\tsh_entsize : %d\n\tsh_flags : %d\n\tsh_link : %d\n\tsh_info : %d\n\tsh_addralign : %d\n",  s.sh_name, s.sh_type, s.sh_addr, s.sh_offset, s.sh_size, s.sh_entsize, s.sh_flags, s.sh_link, s.sh_info, s.sh_addralign);	
	printf("  [+] Content\n\t");
	for( i = 0; i < l->header.sh_size; i++ )
	{
		printf("%.02x", (unsigned char)(l->dump[i]));
	}
	printf("\n");
}

void afficher_Shdr_list(Shdr_list * l)
{
	int i = 0;
	Shdr_list * L = l;
	printf(" Liste des section headers : \n");
	while( L != NULL ){
		printf(" Section [%d] :\n",i++);
		afficher_Shdr(L);
		L = L->next;
	}
}


int32_t lire_4_octets(unsigned char *dump, int i)
{
	unsigned int *pdata = (unsigned int*)(dump + i);
	unsigned int data = *pdata;	
	return data;
}

int16_t lire_2_octets(unsigned char *dump,int i)
{
	unsigned short *pdata = (unsigned int*)(dump + i);
	unsigned short data = *pdata;
	return data;
}


Sym_list read_Sym_list(FILE *f, Elf32_Ehdr h, Sym_list * list, Shdr_list sl, char **names)
{
	Sym_list * l = list;
	Shdr_list * retour;
	retour = find_section_name(names,".symtab",&sl);
	int i=0;
	int j=0;
	l->list = malloc((retour->header.sh_size));
	while(i != retour->header.sh_size){
		l->list[j].st_name = recuperer_valeur32(h,lire_4_octets(retour->dump,i)); i += 4;
		l->list[j].st_value = recuperer_valeur32(h,lire_4_octets(retour->dump,i)); i += 4;
		l->list[j].st_size = recuperer_valeur32(h,lire_4_octets(retour->dump,i)); i += 4;
		l->list[j].st_info = retour->dump[i]; i ++;
		l->list[j].st_other = retour->dump[i]; i ++;
		l->list[j].st_shndx = recuperer_valeur16(h,lire_2_octets(retour->dump,i)); i += 2;
		
		j++;
	}
	l->nb = j;
	return *l;

}


void afficher_Sym( Elf32_Sym S ){
	printf("\tname : %d \tvalue : %d \tsize : %d \tinfo : %d \tother : %d \tshndx : %d\n",
	S.st_name, S.st_value, S.st_size, S.st_info, S.st_other, S.st_shndx);  
}

void afficher_Sym_list(Sym_list l)
{
	int i;
	for( i = 0; i < l.nb; i++ )
	{
		printf("Symbole [%d] : \n", i);
		afficher_Sym(l.list[i]);
	}
}

// Charge le contenu de la section donnée dans une structure de données personnalisée
void read_Elf32_Shdr_Content(Shdr_list *s, unsigned int index, Elf32_Shdr_Content * cp)
{
	Shdr_list * sc = s;
	Elf32_Shdr_Content *c;
	
	c = cp;
	int i;
	for(i = 0; i < index; i++)
	{
		sc = sc->next;
	}
	
	// Lecture du dump
	int j, k;
	for(j = 0; j < sc->header.sh_size; j+=8)
	{
		c->offset = 0;
		
		for(k = 0; k < 4; k++)
		{
			c->offset <<= 8;
			c->offset |= sc->dump[j+k];
			 
			c->info <<= 8;
			c->info |= sc->dump[j+k+4];
		}
		
		c->sym = ELF32_R_SYM(c->info);
		c->type = ELF32_R_TYPE(c->info);
		
		if(j + 8 < sc->header.sh_size)
		{
			c->next = malloc(sizeof(Elf32_Shdr_Content));
			c = c->next;
		}
	}
	c->next = NULL;
}

void afficher_Elf32_Shdr_Content(Elf32_Shdr_Content c)
{
	Elf32_Shdr_Content *cp = &c;
	while(cp != NULL)
	{
		printf("\tOffset : %08x\n", cp->offset);
		printf("\tInfo : %08x\n", cp->info);
		printf("\tSym : %06x\n", cp->sym);
		printf("\tType : %02d\n", cp->type);
		cp = cp->next;
	}
}

// Lis une structure Elf32_Rel (lis une ligne du tableau de relocation)
// Renvoie le nombre d'octets lus 
int read_Elf32_Rel( FILE *f, Elf32_Ehdr h, Elf32_Rel *r, int indice, Elf32_Shdr s)
{
	unsigned long int size = sizeof(Elf32_Rel);
	fseek(f, s.sh_offset, SEEK_SET);
	fseek(f, indice * sizeof(Elf32_Rel), SEEK_CUR);
	fread(r, sizeof(Elf32_Rel), 1,f);
	
	r->r_offset = recuperer_valeur32(h, r->r_offset);
	r->r_info 	= recuperer_valeur32(h, r->r_info);
	return size;
}


// Lis une structure Elf32_Rela (lis une ligne du tableau de relocation)
// Renvoie le nombre d'octets lus 
int read_Elf32_Rela( FILE *f, Elf32_Ehdr h, Elf32_Rela *ra, int indice, Elf32_Shdr s)
{
	unsigned long int size = sizeof(Elf32_Rela);
	fseek(f, s.sh_offset, SEEK_SET);
	fseek(f, indice * sizeof(Elf32_Rela), SEEK_CUR);
	fread(ra, sizeof(Elf32_Rela), 1, f);

	ra->r_offset 	= recuperer_valeur32(h, ra->r_offset);
	ra->r_info		= recuperer_valeur32(h, ra->r_info);
	ra->r_addend	= recuperer_valeur32(h, ra->r_addend);
	
	return size;
}

inline void fwrite_value16(FILE * f, Elf32_Ehdr h, int value, int size)
{
	int16_t v = recuperer_valeur16(h, value);
	fwrite(&v, size, 1, f);
}

inline void fwrite_value32(FILE * f, Elf32_Ehdr h, int value, int size)
{
	int32_t v = recuperer_valeur32(h, value);
	fwrite(&v, size, 1, f);
}

// Ecrit le header dans le stream
void write_Elf32_Ehdr(FILE *f, Elf32_Ehdr h)
{
	rewind(f);
	
	fwrite(h.e_ident, sizeof(unsigned char), EI_NIDENT, f);
	
	fwrite_value16(f, h, h.e_type, sizeof(Elf32_Half));
	fwrite_value16(f, h, h.e_machine, sizeof(Elf32_Half));
	fwrite_value32(f, h, h.e_version, sizeof(Elf32_Word));
	fwrite_value32(f, h, h.e_entry, sizeof(Elf32_Addr));
	fwrite_value32(f, h, h.e_phoff, sizeof(Elf32_Off));
	fwrite_value32(f, h, h.e_shoff, sizeof(Elf32_Off));
	fwrite_value32(f, h, h.e_flags, sizeof(Elf32_Word));
	fwrite_value16(f, h, h.e_ehsize, sizeof(Elf32_Half));
	fwrite_value16(f, h, h.e_phentsize, sizeof(Elf32_Half));
	fwrite_value16(f, h, h.e_phnum, sizeof(Elf32_Half));
	fwrite_value16(f, h, h.e_shentsize, sizeof(Elf32_Half));
	fwrite_value16(f, h, h.e_shnum, sizeof(Elf32_Half));
	fwrite_value16(f, h, h.e_shstrndx, sizeof(Elf32_Half));
}

// Ecrit un header de section
void write_Elf32_Shdr(FILE *f, Elf32_Ehdr h, unsigned int index, Elf32_Shdr s)
{
	unsigned long int size = sizeof(Elf32_Shdr);
	fseek(f, h.e_shoff + size * index, SEEK_SET);
	
	fwrite_value32(f, h, s.sh_name, sizeof(Elf32_Word));
	fwrite_value32(f, h, s.sh_type, sizeof(Elf32_Word));
	fwrite_value32(f, h, s.sh_flags, sizeof(Elf32_Word));
	fwrite_value32(f, h, s.sh_addr, sizeof(Elf32_Addr));
	fwrite_value32(f, h, s.sh_offset, sizeof(Elf32_Off));
	fwrite_value32(f, h, s.sh_size, sizeof(Elf32_Word));
	fwrite_value32(f, h, s.sh_link, sizeof(Elf32_Word));
	fwrite_value32(f, h, s.sh_info, sizeof(Elf32_Word));
	fwrite_value32(f, h, s.sh_addralign, sizeof(Elf32_Word));
	fwrite_value32(f, h, s.sh_entsize, sizeof(Elf32_Word));
}

// Ecriture d'un dump
// Ecriture en position courante du curseur
void write_dump(FILE * f,  unsigned char * dump, Elf32_Word size, Elf32_Off offset)
{
		unsigned int i;
		fseek( f, offset , SEEK_SET);
		for( i = 0; i < (unsigned int)size; i++ )
			fprintf(f,"%c", dump[i]);
}

// Ecriture d'un symbole
// Ecriture en position courante du curseur
void write_Elf32_Sym(FILE * f, Elf32_Ehdr h, Elf32_Sym s)
{
	fwrite_value32(f, h, s.st_name, sizeof(Elf32_Word));
	fwrite_value32(f, h, s.st_value, sizeof(Elf32_Addr));
	fwrite_value32(f, h, s.st_size, sizeof(Elf32_Word));
	fwrite_value32(f, h, s.st_info, sizeof(unsigned char));
	fwrite_value32(f, h, s.st_other, sizeof(unsigned char));
	fwrite_value16(f, h, s.st_shndx, sizeof(Elf32_Half));
}

// Ecrit l'ensemble des sections
// Ecriture en position courante du curseur
void write_Shdr_list(FILE * f, Elf32_Ehdr h, Shdr_list * l)
{
	int i = 0;
	Shdr_list * L = l;
	while( L != NULL )
	{
		write_dump(f, L->dump, L->header.sh_size, L->header.sh_offset);
		L = L->next;
	}
	L = l;
	while( L != NULL )
	{
		write_Elf32_Shdr(f, h, i, L->header );
		L = L->next;
		i++;
	}
}

// Ecrit la liste de symboles
// Ecriture en position courante du curseur
void write_Sym_list(FILE * f, Elf32_Ehdr h, Sym_list l)
{
	int i;
	for(i = 0; i < l.nb; i++)
	{
		write_Elf32_Sym(f, h, l.list[i]);
	}
}

Shdr_list * find_section_name(char **names,char *name, Shdr_list *l)
{
	Shdr_list *list = l;
	int i=0;
	while(strcmp(names[i],name)!=0 && list!=NULL){
		i++;
		list = list->next;
	}
	if(list!=NULL) list=find_section(i, l);
	return list;
}

Shdr_list * find_section(int num, Shdr_list * l)
{
	Shdr_list * L = l;
	int i = 0;
	for( i = 0; i < num; i++ )
		L = L->next;
	return L;
}

Shdr_list * find_symbols_section(Shdr_list * l)
{
	Shdr_list * L = l;
	while( L != NULL )
	{
		if ( L->header.sh_type == SHT_SYMTAB )
			break;
		L = L->next;
	}
	return L;
}


char ** sections_names_table(FILE * f, Elf32_Ehdr h)
{
	Elf32_Shdr s;
	int offset = h.e_shoff + h.e_shstrndx * h.e_shentsize;
	fseek(f, offset, SEEK_SET);
	fseek(f, 16, SEEK_CUR );
	fread( &offset, 4, 1, f );
	offset = recuperer_valeur32(h, offset);
	
	char ** table = (char**) malloc(h.e_shnum * sizeof(char*));
	
	int i = 0;
	for(; i < h.e_shnum; i++)
	{
		table[i] = (char *) malloc(sizeof(char *));
		read_Elf32_Shdr(f, h, i, &s);
		fseek(f, offset + s.sh_name, SEEK_SET);
		char name[256];
		fscanf(f, "%255s", name);
		
		strcpy(table[i], name);
	}
	
	return table;
}

