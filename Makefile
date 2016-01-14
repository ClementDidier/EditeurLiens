CC=gcc
CFLAGS=-Werror -g -c
CHEMINPROJET=src/Programme_Editeur_Liens/
CHEMINTEST=src/tests/
CHEMINNONUTIL = src/Fonction_Non_Utilisées/
OBJ= $(SRC:.c=.o)

all: test_API_read test_API_write test_renumeration test_affichage_sections

test_renumeration: test_renumeration.o renumeration_section.o API.o correction_symboles.o
	$(CC) -g -o $@ $^ -lm

test_affichage_sections: test_affichage_sections.o affichage_sections.o API.o
	$(CC) -g -o $@ $^ -lm

test_API_read: test_API_read.o API.o
	$(CC) -g -o $@ $^ -lm
	
test_API_write: test_API_write.o API.o affichage_sections.o
	$(CC) -g -o $@ $^ -lm

API.o: $(CHEMINPROJET)API.c  $(CHEMINPROJET)API.h
	$(CC) $(CFLAGS) $<

correction_symboles.o : $(CHEMINPROJET)correction_symboles.c $(CHEMINPROJET)correction_symboles.h 
	$(CC) $(CFLAGS) $<

renumeration_section.o : $(CHEMINPROJET)renumeration_section.c $(CHEMINPROJET)renumeration_section.h
	$(CC) $(CFLAGS) $<

test_renumeration.o : $(CHEMINTEST)test_renumeration.c
	$(CC) $(CFLAGS) $<

test_API_read.o : $(CHEMINTEST)test_API_read.c
	$(CC) $(CFLAGS) $<

test_API_write.o : $(CHEMINTEST)test_API_write.c $(CHEMINNONUTIL)affichage_sections.h
	$(CC) $(CFLAGS) $<

test_affichage_sections.o: $(CHEMINTEST)test_affichage_sections.c
	$(CC) $(CFLAGS) $<

affichage_sections.o: $(CHEMINNONUTIL)affichage_sections.c  $(CHEMINNONUTIL)affichage_sections.h
	$(CC) $(CFLAGS) $<


clean   :
	rm -f $(OBJ) ./*~
	rm -f *.o *.*~


