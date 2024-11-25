BIN=./bin
SRC=./src
INCLUDE=./include/
LIB=./lib/

MAIN=main
OBJECTS=$(BIN)/$(MAIN).o $(BIN)/glad.o $(BIN)/SceneNode.o $(BIN)/stb_image_setup.o
LIBS=-lglfw -lX11 -lXxf86vm -lXrandr -lpthread -lassimp -lXi -ldl -lXinerama -lXcursor

all: clean compile run

compile: dirs $(OBJECTS)
	@g++ -I$(LIB) -I$(INCLUDE) $(OBJECTS) -o $(BIN)/$(MAIN) -std=c++11 -Wall $(LIBS)

$(BIN)/stb_image_setup.o: $(SRC)/stb_image_setup.cpp
	@g++ -c $(SRC)/stb_image_setup.cpp -o $(BIN)/stb_image_setup.o

$(BIN)/$(MAIN).o: $(SRC)/$(MAIN).cpp
	@g++ -c $(SRC)/$(MAIN).cpp -o $(BIN)/$(MAIN).o -I$(INCLUDE)

$(BIN)/SceneNode.o: $(SRC)/SceneNode.cpp
	@g++ -c $(SRC)/SceneNode.cpp -o $(BIN)/SceneNode.o -I$(INCLUDE)

$(BIN)/glad.o: $(SRC)/glad.c
	@gcc -c $(SRC)/glad.c -o $(BIN)/glad.o

dirs:
	@mkdir -p $(BIN)

run:
	@$(BIN)/$(MAIN)

clean:
	@rm -rf $(BIN)
