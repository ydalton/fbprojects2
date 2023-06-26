#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/mman.h>

#include <fb.h>

#define MAX_ITER 500

#define TRUNCATE(a) (255 * ((double) a/MAX_ITER))

/* initial conditions */
double cre = -1.4;
double cim = 0;
double diam = 0.2;

double minr;
double mini;
double maxr;
double maxi;

double stepr;
double stepi;

int mb_iterate(double x, double y, uint16_t max_iter) {
	int iter;
	double r = x;
	double i = y;
	for (iter = 0; iter < max_iter; ++iter) {
		double r2 = r*r, i2 = i*i;
		if(r2+i2 >= 4.0) 
			break;
		double ri = r*i;
		i = ri+ri + y;
		r = r2-i2 + x;
	}
	return iter;
}

const double ratio = ((double) SCREEN_WIDTH)/((double) SCREEN_HEIGHT);

void fb_draw_mandelbrot(unsigned char **fb)
{
	unsigned int x, y, iter;
	color_t color;

	const double zoom = 0.02;

	minr = cre - diam * zoom * ratio;
        mini = cim - diam * zoom;
        maxr = cre + diam * zoom * ratio;
        maxi = cim + diam * zoom;

        stepr = (maxr - minr) / SCREEN_WIDTH;
        stepi = (maxi - mini) / SCREEN_HEIGHT;

	for(y = 0; y < SCREEN_HEIGHT; y++) {
		for(x = 0; x < SCREEN_WIDTH; x++) {
			iter = mb_iterate(minr + x*stepr, 
					mini+y*stepi, MAX_ITER);
			color[0] = TRUNCATE(iter);
			color[1] = TRUNCATE(iter);
			color[2] = TRUNCATE(iter);
			fb_set_pixel(fb, x, y, color);
		}
	}

}

int main(int argc, char **argv) {
	int fd;
	//unsigned int jobs;
	unsigned char *fbmem;

	//jobs = (argc > 1) ? atoi(argv[1]) : 1;

	fb_init(&fbmem, &fd);

	fb_draw_mandelbrot(&fbmem);

	fb_close(&fbmem, &fd);

	return 0;
}
