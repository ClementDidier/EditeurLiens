CC=gcc
CFLAGS=-Wall -Werror -g -c
OBJ= $(SRC:.c=.o)

all: test_API test_affichage_sections

test_affichage_sections: test_affichage_sections.o affichage_sections.o
	$(CC) -g -o $@ $^ -lm

test_API: test_API.o API.o
	$(CC) -g -o $@ $^ -lm

%.o: %.c
	$(CC) $(CFLAGS) $<

clean   :
	rm -f $(OBJ) ./*~ *.o *.*~

affichage_sections.o: affichage_sections.h
API.o: API.h
