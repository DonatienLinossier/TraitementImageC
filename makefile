#Passer clean, cleanBuild et cleanOutput et init en .PHONY ? A tester a cytech

CC= gcc

SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)
OBJ2= $(addprefix $(BUILD_DIR)/, $(OBJ))

BUILD_DIR = build
OUTPUT_DIR = Output

#Rajouter init avant le build exec ? A tester a Cytech
all: $(BUILD_DIR)/exec

$(BUILD_DIR)/gestionFichierImg.o: gestionFichierImg.c gestionFichierImg.h $(BUILD_DIR)
	$(CC) -c $< -o $@ 

$(BUILD_DIR)/transformations.o: transformations.c transformations.h $(BUILD_DIR)
	$(CC) -c $< -o $@

$(BUILD_DIR)/interface.o: interface.c interface.h gestionFichierImg.h transformations.h $(BUILD_DIR)
	$(CC) -c $< -o $@

$(BUILD_DIR)/main.o: main.c gestionFichierImg.h interface.h $(BUILD_DIR)
	$(CC) -c $< -o $@

$(BUILD_DIR)/exec: $(OBJ2)
	$(CC) $^ -o $@ -lm

$(BUILD_DIR) :
	mkdir -p $(BUILD_DIR)/

$(OUTPUT_DIR) :
	mkdir -p $(OUTPUT_DIR)/

init : $(BUILD_DIR) $(OUTPUT_DIR)

cleanBuild : 
	rm -f $(BUILD_DIR)/*.o 
	rm $(BUILD_DIR)/exec

cleanOutput :
		rm -f $(OUTPUT_DIR)/*

clean : cleanBuild cleanOutput