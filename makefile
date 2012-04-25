EXECUTABLE = tracer
OBJS = main.o \
	parse.o tokens.o vecmath.o tga.o \
	scene.o light.o camera.o ray.o \
	shape.o sphere.o box.o cylinder.o polygon.o
CC = g++
CFLAGS = -c
LFLAGS = 



$(EXECUTABLE): $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o $(EXECUTABLE)

parse.o: parse.cpp tokens.h parse.h vecmath.h scene.h camera.h light.h
	$(CC) $(CFLAGS) $<

tokens.o: tokens.cpp tokens.h parse.h 
	$(CC) $(CFLAGS) $< 

vecmath.o: vecmath.cpp vecmath.h
	$(CC) $(CFLAGS) $<

shape.o: shape.cpp shape.h ray.h headers.h
	$(CC) $(CFLAGS) $<

scene.o: scene.cpp scene.h shape.h sphere.h box.h cylinder.h polygon.h ray.h light.h headers.h
	$(CC) $(CFLAGS) $<

camera.o: camera.cpp camera.h ray.h headers.h
	$(CC) $(CFLAGS) $<

light.o: light.cpp light.h ray.h
	$(CC) $(CFLAGS) $<

ray.o: ray.cpp ray.h headers.h
	$(CC) $(CFLAGS) $<

sphere.o: sphere.cpp sphere.h shape.h ray.h headers.h
	$(CC) $(CFLAGS) $<

box.o: box.cpp box.h shape.h ray.h headers.h
	$(CC) $(CFLAGS) $<

cylinder.o: cylinder.cpp cylinder.h shape.h ray.h headers.h
	$(CC) $(CFLAGS) $<

polygon.o: polygon.cpp polygon.h shape.h ray.h headers.h
	$(CC) $(CFLAGS) $<

tga.o: tga.cpp tga.h
	$(CC) $(CFLAGS) $<

main.o: main.cpp parse.h tga.o scene.h headers.h
	$(CC) $(CFLAGS) $<



clean:
	rm -f $(OBJS) $(EXECUTABLE) *.h.gch
