CC=gcc
CFLAGS=-Wall -Werror -g -c
CHEMINAPI=src/api/
CHEMINPART2=src/partie2/
CHEMINTEST=src/tests/
OBJ= $(SRC:.c=.o)

all: test_API_read test_API_write test_renumeration

test_renumeration: test_renumeration.o renumeration_section.o API.o correction_symboles.o
	$(CC) -g -o $@ $^ -lm

test_API_read: test_API_read.o API.o
	$(CC) -g -o $@ $^ -lm
	
test_API_write: test_API_write.o API.o affichage_sections.o
	$(CC) -g -o $@ $^ -lm

API.o: $(CHEMINAPI)API.c  $(CHEMINAPI)API.h
	$(CC) $(CFLAGS) $<

correction_symboles.o : $(CHEMINPART2)correction_symboles.c $(CHEMINPART2)correction_symboles.h 
	$(CC) $(CFLAGS) $<

renumeration_section.o : $(CHEMINPART2)renumeration_section.c $(CHEMINPART2)renumeration_section.h
	$(CC) $(CFLAGS) $<

test_renumeration.o : $(CHEMINTEST)test_renumeration.c
	$(CC) $(CFLAGS) $<

test_API_read.o : $(CHEMINTEST)test_API_read.c
	$(CC) $(CFLAGS) $<

test_API_write.o : $(CHEMINTEST)test_API_write.c src/affichage_sections.h
	$(CC) $(CFLAGS) $<

affichage_sections.o: src/affichage_sections.c  src/affichage_sections.h
	$(CC) $(CFLAGS) $<

clean   :
	rm -f $(OBJ) ./*~
	rm -f *.o *.*~


