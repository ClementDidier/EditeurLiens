CC=gcc
CFLAGS=-Wall -Werror -g -c

test_affichage_sections: test_affichage_sections.o affichage_sections.o
	$(CC) -g -o $@ $^ -lm

%.o: %.c
	$(CC) $(CFLAGS) $<


affichage_sections.o: affichage_sections.h
