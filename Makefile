CFLAGS :=-lm -I./include/ -Wall -Wpedantic -Werror
CC := cc
obj := bin/fb.o

test: $(obj) bin/test.o
	$(CC) -o bin/test bin/test.o $(obj) $(CFLAGS)

mandelbrot: $(obj)  bin/mandelbrot.o
	$(CC) -o bin/mandelbrot bin/mandelbrot.o $(obj) $(CFLAGS)

raycaster: $(obj) bin/raycaster.o
	$(CC) -o bin/raycaster bin/raycaster.o $(obj) $(CFLAGS)


bin/%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean: 
	rm -f bin/* 

.PHONY: clean mandelbrot raycaster test
