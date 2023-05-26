.PHONY: all init clean cleanBuild cleanOutput run

CC= gcc


SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)
OBJ2= $(addprefix $(BUILD_DIR)/, $(OBJ))

BUILD_DIR = build

OUTPUT_DIR = Images/Output

STEGANO_DIR = Stegano
STEGANO_OUTPUT_DIR = Output.txt
STEGANO_INPUT_DIR = Input.txt

all: init $(BUILD_DIR)/exec

run : 
	./$(BUILD_DIR)/exec

#Compilation
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


#Initialiation
$(BUILD_DIR) :
	mkdir -p $(BUILD_DIR)/

$(OUTPUT_DIR) :
	mkdir -p $(OUTPUT_DIR)/

$(STEGANO_DIR) :
	mkdir -p $(STEGANO_DIR)/
	touch $(STEGANO_DIR)/$(STEGANO_OUTPUT_DIR)
	touch $(STEGANO_DIR)/$(STEGANO_INPUT_DIR)


init : $(BUILD_DIR) $(OUTPUT_DIR) $(STEGANO_DIR)


#Clean
cleanBuild : 
	rm -f $(BUILD_DIR)/*.o 
	rm $(BUILD_DIR)/exec

cleanOutput :
		rm -f $(OUTPUT_DIR)/*

clean : cleanBuild cleanOutput