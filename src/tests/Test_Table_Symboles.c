#include "Table_Symboles.h"
#include <stdio.h>

int main( int argc, char **argv ){
	FILE *f = fopen( argv[1], "r" );
	if ( f == NULL )
		return 1;
	afficher_table_symboles( f );
	fclose( f );
	return 0;
}
