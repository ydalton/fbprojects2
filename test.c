#include <fb.h>
#include <unistd.h>

int main()
{
	int fd;
	unsigned char *fbmem;
	int len;

	color_t colors[] = {
		{0xff, 0, 0},
		{0xff, 0xff, 0},
		{0, 0xff, 0},
		{0, 0xff, 0xff},
		{0, 0, 0xff},
		{0x7f, 0, 0xff},
		{0xff, 0, 0xff},
	};

	fb_init(&fbmem, &fd);
	for(int i = 0; i < 50; i++) {
		fb_draw_rect(&fbmem, i*20, i*20, 500, 500, colors[i % 7]);
		usleep(50000);
	}
	len = 500;
	for(int i = 0; i < 400; i++) {
		len -= 2;
		if(len < 0)
			break;
		fb_draw_line_vertical(&fbmem, 400+i, 400+i, len, colors[4]);
	}
	fb_draw_line(&fbmem, 50, 50, 500, 100, colors[4]);
	fb_close(&fbmem, &fd);
}
