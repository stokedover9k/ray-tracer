OBJS = main.o parse.o tokens.o vecmath.o shape.o scene.o camera.o ray.o light.o
CC = g++
CFLAGS = -c
LFLAGS = 



a.out: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS)

parse.o: parse.cpp tokens.h parse.h vecmath.h scene.h camera.h light.h
	$(CC) $(CFLAGS) $<

tokens.o: tokens.cpp tokens.h parse.h 
	$(CC) $(CFLAGS) $< 

vecmath.o: vecmath.cpp vecmath.h
	$(CC) $(CFLAGS) $<

shape.o: shape.cpp shape.h ray.h headers.h
	$(CC) $(CFLAGS) $<

scene.o: scene.cpp scene.h ray.h light.h headers.h
	$(CC) $(CFLAGS) $<

camera.o: camera.cpp camera.h ray.h headers.h
	$(CC) $(CFLAGS) $<

light.o: light.cpp light.h ray.h
	$(CC) $(CFLAGS) $<

ray.o: ray.cpp ray.h headers.h
	$(CC) $(CFLAGS) $<

main.o: main.cpp parse.h scene.h headers.h
	$(CC) $(CFLAGS) $<



clean:
	rm -f $(OBJS) a.out *.h.gch
