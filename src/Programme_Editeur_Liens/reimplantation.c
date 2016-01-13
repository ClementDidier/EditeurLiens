#include "reimplantation.h"


// Effectuer la reimplantation des sections 
void ri_abs32(Elf32_Ehdr h, Elf32_Rel R, Elf32_Shdr H ,Shdr_list* l, Sym_list sym_list , int *num_sections ){
	Shdr_list * S  = find_section( num_sections[H.sh_info], l ); // La section où la relocation doit avoir lieu 
	unsigned int *pdata = (unsigned int*)(S->dump + R.r_offset);
	unsigned int data = recuperer_valeur32(h, *pdata);
	// Calcul de la valeur finale 
	data += sym_list.list[ELF32_R_SYM(R.r_info)].st_value;
	*pdata = __bswap_32( data );
}

void ri_abs16(Elf32_Ehdr h, Elf32_Rel R, Elf32_Shdr H, Shdr_list* l, Sym_list sym_list  , int *num_sections  ){
	Shdr_list * S  = find_section( num_sections[H.sh_info], l ); // La section où la relocation doit avoir lieu 
	unsigned short int *pdata = (unsigned short int*)(S->dump + R.r_offset);
	unsigned short int data = recuperer_valeur16(h,*pdata);
	// Calcul de la valeur finale 
	data += (unsigned short int)sym_list.list[ELF32_R_SYM(R.r_info)].st_value;
	*pdata = __bswap_16( data );
}

void ri_abs8(Elf32_Ehdr h, Elf32_Rel R, Elf32_Shdr H, Shdr_list* l, Sym_list sym_list , int *num_sections  ){
	Shdr_list * S  = find_section( num_sections[H.sh_info], l ); // La section où la relocation doit avoir lieu 
	unsigned char data = S->dump[R.r_offset];
	// Calcul de la valeur finale 
	data += (unsigned char)sym_list.list[ELF32_R_SYM(R.r_info)].st_value;
	S->dump[R.r_offset] = data;
}

void ri_call(Elf32_Ehdr h, Elf32_Rel R, Elf32_Shdr H, Shdr_list* l, Sym_list sym_list , int *num_sections  ){
	Shdr_list * S  = find_section( num_sections[H.sh_info], l ); // La section où la relocation doit avoir lieu 
	unsigned int *pdata = (unsigned int*)(S->dump + R.r_offset);
	unsigned int data = recuperer_valeur32(h,*pdata), tmp;
	// Calcul de la valeur finale 
	tmp = data & 0xFF000000;
	data = (data & 0x00FFFFFF) << 2;
	//printf("Relocation in section %d: addend %x value 0x%x location 0x%x becomes ",num_sections[H.sh_info], data, sym_list.list[ELF32_R_SYM(R.r_info)].st_value, ( S->header.sh_addr + R.r_offset ) );
	data += sym_list.list[ELF32_R_SYM(R.r_info)].st_value;
	data -= (S->header.sh_addr + R.r_offset );
	data = ( data & 0x03FFFFFE ) >> 2;
	data = (data & 0x00FFFFFF) | tmp;
	*pdata = recuperer_valeur32(h, data );
	//printf(" %x\n", data & 0xFFFFFFFF );
}

void ri_jump24(Elf32_Ehdr h, Elf32_Rel R, Elf32_Shdr H, Shdr_list* l, Sym_list sym_list , int *num_sections  ){
	Shdr_list * S  = find_section( num_sections[H.sh_info] , l ); // La section où la relocation doit avoir lieu 
	unsigned int *pdata = (unsigned int*)(S->dump + R.r_offset);
	unsigned int data = recuperer_valeur32(h,*pdata), tmp;
	// Calcul de la valeur finale 
	tmp = data & 0xFF000000;
	data = (data & 0x00FFFFFF) << 2;
	//printf("Relocation in section %d: addend %x value 0x%x location 0x%x becomes ",num_sections[H.sh_info], data, sym_list.list[ELF32_R_SYM(R.r_info)].st_value, ( S->header.sh_addr + R.r_offset ) );
	data += sym_list.list[ELF32_R_SYM(R.r_info)].st_value;
	data -= (S->header.sh_addr + R.r_offset );
	data = ( data & 0x03FFFFFE ) >> 2;
	data = (data & 0x00FFFFFF) | tmp;
	*pdata = recuperer_valeur32(h, data );
	//printf(" %x\n", data & 0xFFFFFFFF );
}

void reimplantation(Elf32_Ehdr h, Shdr_list * rel_list, Shdr_list* l, Sym_list s  , int *num_sections  ){

	Shdr_list * L = rel_list;
	Elf32_Rel R; 
	void *dump;
	unsigned bytes_lus;
	if ( L == NULL )
		return;
	while( L != NULL ){
		dump = L->dump;
		// Lecture d'une relocation a effectuer 
		for( bytes_lus = 0; bytes_lus < L->header.sh_size; bytes_lus += sizeof(Elf32_Rel) )
		{
			R.r_offset = __bswap_32( *((Elf32_Addr*)dump) );
			R.r_info = __bswap_32( (((Elf32_Word*)dump)[1]) );
			//printf("Offset : %x type : %x sym : %x \n", R.r_offset, ELF32_R_TYPE(R.r_info), ELF32_R_SYM(R.r_info) );
			switch( ELF32_R_TYPE(R.r_info) ){
				case R_ARM_ABS32:
					ri_abs32(h, R , L->header, l, s, num_sections );
					break;
				case R_ARM_ABS16:
					ri_abs16(h, R, L->header, l, s,num_sections  );
					break;
				case R_ARM_ABS8:
					ri_abs8(h, R, L->header, l, s,num_sections  );
					break;
				case R_ARM_JUMP24:
					ri_jump24(h, R, L->header, l, s,num_sections  );
					break;
				case R_ARM_CALL:
					ri_call(h, R, L->header, l, s,num_sections  );
					break;
				default:
					break;
			}
			dump = dump + sizeof(Elf32_Rel);
		}
		L = L->next;
	}
}
