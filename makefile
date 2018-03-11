HEADERS = src/bta.h src/video_decoder.h src/image_scaler.h
OBJECTS = src/main.o src/bta.o src/video_decoder.o src/image_scaler.o
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
