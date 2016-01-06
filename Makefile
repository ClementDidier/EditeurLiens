CC=gcc
CFLAGS=-Wall -Werror -g -c

test_affichage_sections: test_affichage_sections.o affichage_sections.o
	$(CC) -g -o $@ $^ -lm

test_API: test_API.o API.o
	$(CC) -g -o $@ $^ -lm

%.o: %.c
	$(CC) $(CFLAGS) $<


affichage_sections.o: affichage_sections.h
API.o: API.h
