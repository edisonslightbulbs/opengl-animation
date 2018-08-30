CC = g++

CFLAGS = -g -std=c++11

LIBS = -lglut -lGL -lGLU -lm -lGLEW -lIL -lILU -lILUT -lassimp -lSDL

SOURCES = main.cpp animation.cpp mesh.cpp  model.cpp shaderLinker.cpp display.cpp display_input.cpp

TARGET = skeleton-animation

output:
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET) $(LIBS)

clean: 
	rm -rf *o $(TARGET)
