HEADERS = src/bta.h src/video_decoder.h
OBJECTS = src/main.o src/bta.o src/video_decoder.o
LIBS = -lturbojpeg -lm -lavcodec -lavutil -lavformat

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
