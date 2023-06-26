srctree := .

CFLAGS :=-lm -I$(srctree)/include/ -Wall -Wpedantic -g -Werror
CC := cc
obj := fb.o

mandelbrot: $(obj)  mandelbrot.o
	$(CC) -o bin/mandelbrot mandelbrot.o $(obj) $(CFLAGS)

raycaster: $(obj) raycaster.o
	$(CC) -o bin/raycaster raycaster.o $(obj) $(CFLAGS)

test: $(obj) test.o
	$(CC) -o bin/test test.o $(obj) $(CFLAGS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean: 
	rm -f $(obj) 

.PHONY: clean mandelbrot raycaster test
