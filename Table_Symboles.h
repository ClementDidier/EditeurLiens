#ifndef _TABLE_SYMBOLES_
#define _TABLE_SYMBOLES_

#include <elf.h>
#include <stdio.h>

int afficher_table_symboles(FILE *f);
void l2b_endian( unsigned int * val );


#endif
