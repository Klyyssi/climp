HEADERS = src/image.h src/video_decoder.h src/image_scaler.h src/ui.h src/ascii_converter.h
OBJECTS = src/main.o src/image.o src/video_decoder.o src/image_scaler.o src/ui.o src/ascii_converter.o
LIBS = -lturbojpeg -lm -lavcodec -lavutil -lavformat -lncurses

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
