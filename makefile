CC= gcc

#Liste des .c
SRC=$(wildcard *.c)

#Liste les .o
OBJ=$(SRC :.c=.o) 

all: exec

gestionFichier.o: gestionFichierImg.c gestionFichierImg.h
	$(CC) $< -c $@

transformations.o: transformations.c transformations.h
	$(CC) $< -c $@

interface.o: interface.c interface.h gestionFichierImg.h transformations.h
	$(CC) $< -c $@

main.o: main.c gestionFichierImg.h interface.h
	$(CC) $< -c $@

exec: $(OBJ)
	$(CC) $^ -o $@

clean :
	rm -f *.o 
	rm exec