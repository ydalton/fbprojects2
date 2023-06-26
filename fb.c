#include <fcntl.h>
#include <math.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#include <fb.h>

void fb_init(unsigned char **fb, int *fd) 
{
	*fd = open("/dev/fb0", O_RDWR);
	*fb = mmap(NULL, SCREEN_SIZE, PROT_WRITE, MAP_SHARED, *fd, 0);
}

int fb_close(unsigned char **fbmem, int *fb) 
{
	int ret;
	ret = munmap(*fbmem, SCREEN_SIZE);
	close(*fb);
	if(ret)
		return 1;
	return 0;
}

void fb_set_pixel(unsigned char **fb, unsigned int x, unsigned y, color_t color)
{
	(*fb)[4 * (x + SCREEN_WIDTH * y)] = color[0];
	(*fb)[4 * (x + SCREEN_WIDTH * y)+1] = color[1];
	(*fb)[4 * (x + SCREEN_WIDTH * y)+2] = color[2];
}

void fb_draw_line_horizontal(unsigned char **fb, unsigned int x, unsigned int y,
			unsigned int length, color_t color)
{
	if(x > SCREEN_WIDTH || y > SCREEN_HEIGHT)
		return;
	if(length < 0)
		length = 0;
	if(length + x > SCREEN_WIDTH)
		length = SCREEN_WIDTH - x;
	for(int i = 0; i < length; i++)
		fb_set_pixel(fb, x + i, y, color);
}

void fb_draw_line_vertical(unsigned char **fb, unsigned int x, unsigned int y,
			unsigned int length, color_t color)
{
	if(x > SCREEN_WIDTH || y > SCREEN_HEIGHT)
		return;
	if(length < 0)
		length = 0;
	if(length + x > SCREEN_HEIGHT)
		length = SCREEN_HEIGHT - y;
	for(int i = 0; i < length; i++)
		fb_set_pixel(fb, x, y + i, color);
}

void fb_draw_rect(unsigned char **fb, unsigned int x, unsigned int y,
		unsigned int w, unsigned int h, color_t color)
{
	if(x+w > SCREEN_WIDTH)
		w = SCREEN_WIDTH - x;
	if(y+h > SCREEN_HEIGHT)
		h = SCREEN_HEIGHT - y;
	for(int i = 0; i < h; i++) {
		fb_draw_line_horizontal(fb, x, y + i, w, color);
	}
}

void fb_draw_line(unsigned char **fb, unsigned int x1, unsigned int y1,
		unsigned int x2, unsigned int y2, color_t color) 
{
	unsigned int y;
	const float a = ((float) (y2 - y1))/((float) (x2 - x1));
	/* draw points */
	fb_set_pixel(fb, x1, y1, color);
	fb_set_pixel(fb, x2, y2, color);
	/* draw lines */
	for(int i = x1; i < x2; i++) {
		y = a * (i - x1) + y1;
		fb_set_pixel(fb, i, y, color);
	}
}

