#ifndef __AFFICHAGE_SECTIONS__
#define __AFFICHAGE_SECTIONS__

#include <elf.h>
#include <stdio.h>
#include <byteswap.h>
#include <string.h>
#include <stdlib.h>
#include "api/API.h"

char* sh_type(Elf32_Word typeData);

char* sh_flags(Elf32_Word flagsData);

void bytes_swap_file_header(Elf32_Ehdr* header);

void bytes_swap_section_header(Elf32_Shdr* section);

void afficher_table_sections(FILE* elfFile);

#endif
