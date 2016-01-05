#ifndef __AFFICHAGE_SECTIONS__
#define __AFFICHAGE_SECTIONS__

char* sh_type(Elf32_Word typeData);
char* sh_flags(Elf32_WOrd flagsData);
void afficher_table_sections(FILE* elfFile);

#endif
