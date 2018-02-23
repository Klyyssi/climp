HEADERS = src/bta.h
OBJECTS = src/main.o src/bta.o
LIBS = -lturbojpeg -lm

default: bta

%.o: %.c $(HEADERS)
	gcc -c $< -o $@

bta: $(OBJECTS)
	gcc $(OBJECTS) -o $@ $(LIBS)

clean_objects:
	-rm -f $(OBJECTS)

clean:
	-rm -f $(OBJECTS)
	-rm -f bta
