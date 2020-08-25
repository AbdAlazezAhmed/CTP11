HEADERS = GL/GLIncludes.h GL/3DView.h Graphics2D/graph.h Graphics2D/graphicsInclude.h CommonInc.h Serial/Serial.h
SAUCE = main.c GL/3DView.c Graphics2D/graph.c Serial/Serial.c
DIR1 = ./
DIR2 = GL
DIR3 = Graphics2D
DIR4 = Serial
INC=$(DIR1) $(DIR2) $(DIR3)
INC_PARAMS=$(foreach d, $(INC), -I$d)
default: main
remove:
	rm main
reload:
	rm main
	make run
# main.o: main.c $(HEADERS) 
# 	gcc -c main.c `pkg-config --cflags gtk+-3.0 gl` $(INC_PARAMS)  -o main.o `pkg-config --cflags gtk+-3.0 gl`

# main: main.o
# 	gcc main.o -o main `pkg-config --cflags gtk+-3.0 gl`
main: main.c $(HEADERS) 
	gcc `pkg-config --cflags gtk+-3.0 gl` -DGL_GLEXT_PROTOTYPES $(INC_PARAMS) $(SAUCE)  -o main  `pkg-config --libs gtk+-3.0 gl` -lm
clean:
	# -rm -f main.o
	-rm -f main

run: main 
	./main