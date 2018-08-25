CC = g++

CFLAGS = -g -std=c++11 -W

LIBS = -lglut -lGL -lGLU -lm -lGLEW -lIL -lILU -lILUT -lassimp

SOURCES = main.cpp loader/loader.cpp loader/shader.cpp  loader/mesh.cpp animate/model.cpp display/display.cpp

TARGET = skeleton-animation

output:
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET) $(LIBS)

clean: 
	rm -rf *o $(TARGET)
