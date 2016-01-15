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

////Afficher l'en tête du fichier en format présentable

// ecriture de e_ident (Magique)
void aff_ET_indent(Elf32_Ehdr h){
	int i;
	printf(" Magique:    ");
	for(i=0;i<16;i++){
		if(h.e_ident[i]/16 == 0){
			printf(" 0%x", h.e_ident[i]);
		}else{
			printf(" %02x",h.e_ident[i]);
		}
	}
	printf("\n");
}

// ecriture de e_version
void aff_ET_version(Elf32_Ehdr h){
	switch(h.e_version){
		case 0 :
			printf(" Version:\t\t\t\t%d (Invalid Version)\n",h.e_version);
			break;
		case 1 :
			printf(" Version:\t\t\t\t%d (Current Version)\n",h.e_version);
			break;
	}
}


// ecriture de e_type
void aff_ET_type(Elf32_Ehdr h){
	switch(h.e_type){
		case ET_NONE :
			printf(" Type:\t\t\t\t\tNo file type\n");
			break;
		case ET_REL :
			printf(" Type:\t\t\t\t\tRelocatable file\n");
			break;
		case 2 :
			printf(" Type:\t\t\t\t\tExecutable file\n");
			break;
		case 3 :
			printf(" Type:\t\t\t\t\tShared object file\n");
			break;
		case 4 :
			printf(" Type:\t\t\t\t\tCore file\n");
			break;
		case 0xff00 :
			printf(" Type:\t\t\t\t\tProcessor-specific\n");
			break;
		case 0xffff :
			printf(" Type:\t\t\t\t\tProcessor-specific\n");
			break;
		default :
			printf(" ERROR ON THE TYPE OF THE FILE ;)\n");
			break;
	}
}

// ecriture de e_machine
void aff_ET_machine(Elf32_Ehdr h){
	switch(h.e_machine){
		case EM_NONE :
			printf(" Machine:\t\t\t\tmachine inconne");
			break;
		case EM_M32 :
			printf(" Machine:\t\t\t\tAT&T WE 32100");
			break;
		case EM_SPARC :
			printf(" Machine:\t\t\t\tSun Microsystems SPARC");
			break;
		case EM_386 :
			printf(" Machine:\t\t\t\tIntel 80386");
			break;
		case EM_68K :
			printf(" Machine:\t\t\t\tMotorola 68000");
			break;
		case EM_88K :
			printf(" Machine:\t\t\t\tMotorola 88000");
			break;
		case EM_860 :
			printf(" Machine:\t\t\t\tIntel 80860");
			break;
		case EM_MIPS :
			printf(" Machine:\t\t\t\tMIPS RS3000");
			break;
		case EM_PARISC :
			printf(" Machine:\t\t\t\tHP/PA");
			break;
		case EM_SPARC32PLUS :
			printf(" Machine:\t\t\t\tSPARC avec jeu d'instruction étendu");
			break;
		case EM_PPC :
			printf(" Machine:\t\t\t\tPowerPC");
			break;
		case EM_PPC64 :
			printf(" Machine:\t\t\t\tPowerPC 64 bits");
			break;
		case EM_S390 :
			printf(" Machine:\t\t\t\tIBM S/390");
			break;
		case EM_ARM :
			printf(" Machine:\t\t\t\tARM RISC");
			break;
		case EM_SH :
			printf(" Machine:\t\t\t\tRenesas SuperH");
			break;
		case EM_SPARCV9 :
			printf(" Machine:\t\t\t\tSPARCC v9 64 bits");
			break;
		case EM_IA_64 :
			printf(" Machine:\t\t\t\tIntel Itanium");
			break;
		case EM_X86_64 :
			printf(" Machine:\t\t\t\tAMD x86-64");
			break;
		case EM_VAX :
			printf(" Machine:\t\t\t\tDEC Vax");
			break;
		default :
			printf(" ERROR ON THE FILE'S 'MACHINE'");
			break;
	}
	printf("\n");
}

void afficher_en_tete(Elf32_Ehdr h){
	printf("En_tête ELF:\n");
	//Affichage de l'indent (Magique)	
	aff_ET_indent(h);
	//Affichage de la classe	
	printf(" Classe:\t\t\t\tELF32\n");
	//Affichage de l'endianness
	if(my_is_big_endian(h)){
		printf(" Données:\t\t\t\tcomplément à 2, système à octets de poids fort d'abord (big endian)\n");
	}
	else{
		printf(" Données:\t\t\t\tlittle endian\n");
	}
	//Affichage du type
	aff_ET_type(h);
	//Affichage de machine
	aff_ET_machine(h);
	//Affichage de la version
	aff_ET_version(h);
	//Affichage du e_entry
	printf(" Addresse du point d'entrée:\t\t0x%x\n",h.e_entry);
	//Affichage du e_phoff
	printf(" Début des en_têtes du programme:\t%d\n", h.e_phoff);
	//Affichage du e_shoff
	printf(" Shoff:\t\t\t\t\t%d\n",h.e_shoff);
	//Affichage des e_flags
	printf(" Flags:\t\t\t\t\t0x%x\n",h.e_flags);
	//Affichage des e_ehsize
	printf(" Taille de l'en-tête (en octets):\t%d\n", h.e_ehsize);
	//Affichage du e_phentsize
	printf(" Taille d'une entrée de la table d'en tête (en octets):\t%d\n", h.e_phentsize);
	//Affichage du e_phnum
	printf(" Nombre d'entrée de la table d'en tête:\t%d\n", h.e_phnum);
	//Affichage du e_shentsize
	printf(" Taille de section headers:\t\t%d (bytes)\n",h.e_shentsize);
	//Affichage du e_shnum
	printf(" Nombre de section headers:\t\t%d\n",h.e_shnum);
	//Affichage du e_shstrndx
	printf(" L'index de la table des section headers:\t%d\n",h.e_shstrndx);
	//Affichage de la taille de l'en_tête
	int resultat = h.e_phentsize * h.e_phnum ; 
	printf(" Taille de cet en_tête (en octets):\t\t%d", resultat);
	printf("\n\n");
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

///////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
void aff_Shdr_type(Elf32_Shdr s){
	switch(s.sh_type){
		case SHT_NULL :
			printf(" %15s ","NULL");
			break;
		case SHT_PROGBITS :
			printf(" %15s ","PROGBITS");
			break;
		case SHT_SYMTAB :
			printf(" %15s ","SYMTAB");
			break;
		case SHT_STRTAB :
			printf(" %15s ","STRTAB");
			break;
		case SHT_RELA :
			printf(" %15s ","RELA");
			break;
		case SHT_HASH :
			printf(" %15s ","HASH");
			break;
		case SHT_DYNAMIC :
			printf(" %15s ","DYNAMIC");
			break;
		case SHT_NOTE :
			printf(" %15s ","NOTE");
			break;
		case SHT_NOBITS :
			printf(" %15s ","NOBITS");
			break;
		case SHT_REL :
			printf(" %15s ","REL");
			break;
		case SHT_SHLIB :
			printf(" %15s ","SHLIB");
			break;
		case SHT_DYNSYM :
			printf(" %15s ","DYNSYM");
			break;
		case SHT_LOPROC :
			printf(" %15s ","LOPROC");
			break;
		case SHT_HIPROC :
			printf(" %15s ","HIPROC");
			break;
		case SHT_LOUSER :
			printf(" %15s ","LOUSER");
			break;
		case SHT_HIUSER :
			printf(" %15s ","HIUSER");
			break;
		default :
			printf(" %15s ","ARM_ATTRIBUTES");
			break;
	}
}


void aff_Shdr_name( Elf32_Shdr S, FILE *f, Elf32_Ehdr h, unsigned char **names,Shdr_list *s){

	Shdr_list *list;
	list = find_section_name(names,".shstrtab",s);	
	fseek(f, list->header.sh_offset + S.sh_name, SEEK_SET);
	char name[256];
	fscanf(f, "%255s", name);
	printf(" %-16s\t",name);

}

void afficher_Shdr( Shdr_list *l, FILE *f, Elf32_Ehdr h, unsigned char **names,Shdr_list *s){
	Elf32_Shdr S = l->header;	
	aff_Shdr_name(S,f,h,names,s);
	aff_Shdr_type(S);
	printf(" %08x ",S.sh_addr);
	printf(" %06x ",S.sh_offset);	
	printf(" %06x ",S.sh_size);
	printf(" %02x ",S.sh_entsize);
	printf(" %03d ",S.sh_flags);
	printf(" %02d ",S.sh_link);
	printf(" %03d ",S.sh_info);
	printf(" %02d ",S.sh_addralign);
	printf("\n");
}

void afficher_Shdr_list(Shdr_list * l, FILE *f, Elf32_Ehdr h, unsigned char **names,Shdr_list *s)
{
	int i = 0;
	Shdr_list * L = l;
	printf(" Liste des section headers : \n");
	printf("%3s\t %s \t\t\t%16s  %7s %8s %6s %4s %4s %3s %4s %3s\n","Num","Name","Type","Address","Offset","Size","ES","Fan","LN","Inf","Al");	
	while( L != NULL ){
		printf(" [%d]\t",i++);
		afficher_Shdr(L,f,h,names,s);
		L = L->next;
	}
	printf("\n");
}


int32_t lire_4_octets(unsigned char *dump, int i)
{
	unsigned int *pdata = (unsigned int*)(dump + i);
	unsigned int data = *pdata;	
	return data;
}

int16_t lire_2_octets(unsigned char *dump,int i)
{
	unsigned short *pdata = (unsigned short*)(dump + i);
	unsigned short data = *pdata;
	return data;
}


Sym_list read_Sym_list(FILE *f, Elf32_Ehdr h, Sym_list * list, Shdr_list sl, unsigned char **names)
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

void aff_S_type(Elf32_Sym S){
	switch (ELF32_ST_TYPE(S.st_info)){
		case STT_NOTYPE :
			printf(" %7s ","NOTYPE");
			break;
		case STT_OBJECT :
			printf(" %7s ","OBJECT");
			break;
		case STT_FUNC :
			printf(" %7s ","FUNC");
			break;
		case STT_SECTION :
			printf(" %7s ","SECTION");
			break;
		case STT_FILE :
			printf(" %7s ","FILE");
			break;
		case STT_LOPROC :
			printf(" %7s ","LOPROC");
			break;
		case STT_HIPROC :
			printf(" %7s ","HIPROC");
			break;
		default :
			printf("Si on est ici c'est pas normal");
			break;
	}
}

void aff_S_link(Elf32_Sym S){
	switch (ELF32_ST_BIND(S.st_info)){
		case STB_LOCAL :
			printf(" %7s ","LOCAL");
			break;
		case STB_GLOBAL :
			printf(" %7s ","GLOBAL");
			break;
		case STB_WEAK :
			printf(" %7s ","WEAK");
			break;
		case STB_LOPROC :
			printf(" %7s ","LOPROC");
			break;
		case STB_HIPROC :
			printf(" %7s ","HIPROC");
			break;
		default :
			printf("Si on est ici c'est pas normal");
			break;
	}
}

void aff_S_ndx(Elf32_Sym S){
	if(S.st_shndx == 0){
		printf(" %3s ","UND");
	}
	else{
		printf(" %3d ",S.st_shndx);
	}
}

void aff_S_name(Elf32_Sym S, FILE *f, Elf32_Ehdr h, unsigned char **names,Shdr_list *s){
	if(S.st_name == 0){
		printf(" ");
	}
	else{
		Shdr_list *list;
		list = find_section_name(names,".strtab",s);	
		int offset;
		//offset = recuperer_valeur32(h, list->header.sh_offset);
	
		fseek(f, list->header.sh_offset + S.st_name, SEEK_SET);
		char name[256];
		fscanf(f, "%255s", name);

		printf(" %s ",name);
	}
}

void afficher_Sym( Elf32_Sym S ,FILE *f, Elf32_Ehdr h, unsigned char **names,Shdr_list *s){
	printf("%08x %4x   ", S.st_value, S.st_size);
	aff_S_type(S);
	aff_S_link(S);
	aff_S_ndx(S);
	aff_S_name(S,f,h,names,s);
	printf("\n");
}

void afficher_Sym_list(Sym_list l, FILE *f, Elf32_Ehdr h, unsigned char **names,Shdr_list *s)
{
	int i;
	printf("Table des symboles : \n");
	printf("%3s %8s  %4s %7s %7s %5s %3s\n","Num ","Valeur","Tail  ","Type", "Lien", "Ndx", "Nom");
	for( i = 0; i < l.nb; i++ )
	{
		printf("%3d%2c", i,' ');
		afficher_Sym(l.list[i],f,h,names,s);
	}
	printf("\n");
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

Shdr_list * find_section_name( unsigned char **names,char *name, Shdr_list *l)
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


unsigned char ** sections_names_table(FILE * f, Elf32_Ehdr h)
{
	Elf32_Shdr s;
	int offset = h.e_shoff + h.e_shstrndx * h.e_shentsize;
	fseek(f, offset, SEEK_SET);
	fseek(f, 16, SEEK_CUR );
	fread( &offset, 4, 1, f );
	offset = recuperer_valeur32(h, offset);
	
	unsigned char ** table = (unsigned char**) malloc(h.e_shnum * sizeof(char*));
	
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


//Création des structure pour afficher proprement les différentes sections
void afficher_reimplantation(Elf32_Ehdr h, Shdr_list * shdr_list, unsigned char ** names){
	Shdr_list * copie = shdr_list; 
	unsigned int i = 0;
	char * nom;
	// Lecture de chaque section de relocation et affichage
	while(copie != NULL)
	{
		if(copie->header.sh_type == SHT_REL){
			nom = names[i];
			printf("Section de relocation : %s à l'adresse de décalage : 0x%x :\n", nom,copie->header.sh_offset );
			int taille;
			// lecture et affichage de chaque relocation pour une section
			for(taille=0; taille < copie->header.sh_size; taille += sizeof(Elf32_Rel)){
				Elf32_Rel * pelfRel = (Elf32_Rel *)(copie->dump + taille);
				Elf32_Rel elfRel = *pelfRel;	
				// Affichage du nom des colonnes de données
				printf("%16s %16s %16s %16s\n","Offset(hex)","Info","Index","Type");
				printf("%16x %16x %16d %16d\n",recuperer_valeur32(h, elfRel.r_offset),recuperer_valeur32(h, elfRel.r_info),recuperer_valeur16(h, ELF32_R_SYM(elfRel.r_info)),recuperer_valeur16(h, ELF32_R_TYPE(elfRel.r_info)));
			}
			printf("\n");
		}
		i++;
		copie = copie->next;
	}
}

//Afficher joliment le dump d'une section dont on connait le numéro
void afficher_section(Shdr_list *s,unsigned char **names,int x){
	Shdr_list *list = s;
	list = find_section(x,list);	
	if(list->header.sh_size==0){
		printf("\nSection '%s' has no data to dump\n\n",names[x]);
	}
	else{
		printf("\nHexdump of section '%s' :\n\t",names[x]);
		int i,j=0;
		for(i=0;i<list->header.sh_size;i++){
			j++;
			printf("%02x",list->dump[i]);
			if(j%4==0)printf(" ");
			if(j%16==0)printf("\n\t");
		}
		printf("\n\n");
	}
}

//Afficher joliment le dump d'une section dont on connait le nom
void afficher_section_name(Shdr_list *s,unsigned char **names,char *name){
	Shdr_list *list = s;
	list = find_section_name(names,name,list);
	if(list->header.sh_size==0){
		printf("\nSection '%s' has no data to dump\n\n",name);
	}
	else{
		printf("\nHexdump of section '%s' :\n\t",name);
		int i,j=0;
		for(i=0;i<list->header.sh_size;i++){
			j++;
			printf("%02x",list->dump[i]);
			if(j%4==0)printf(" ");
			if(j%16==0)printf("\n\t");
		}
		printf("\n\n");
	}
}


//Pour liberer les structures utilisées

void liberer_Shdr_list(Shdr_list *sl){
	Shdr_list *prec = sl;
	int i =0;
	while(sl != NULL){
		if(sl->header.sh_name == 0){
			sl = sl->next;
			prec = sl;
		}
		else{
			sl = sl->next;
			free(prec->dump);
			free(prec);
			prec = sl;
			i++;
		}
	}
}
//
void liberer_Sym_list(Sym_list *sl){

	free(sl->list);
}

void liberer_tab_name(unsigned char ** names, int h_shnum){
	int i ;
	for(i=0; i<h_shnum; i++){
		free(names[i]);
	}
	free(names);
}

void liberer_num_sections(int * num_sections){
	free(num_sections);
}



