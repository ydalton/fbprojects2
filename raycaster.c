#include <fb.h>
#include <termios.h>
#include <unistd.h>

char map[8][8] =
{
	{1,1,1,1,1,1,1,1},
	{1,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,1},
	{1,1,1,1,1,1,1,1}
};

void enter_raw_mode()
{
	struct termios raw;
	tcgetattr(STDIN_FILENO, &raw);
	raw.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void exit_raw_mode(struct termios *t) {
	tcsetattr(STDIN_FILENO, TCSAFLUSH, t);
}

int main()
{
	int fd;
	unsigned char *fb;
	color_t color;
	unsigned char col;
	struct termios orig_termios;

	struct {
		unsigned int w, h;
	} rect = {
		.w = 200,
		.h = 200,
	};

	fb_init(&fb, &fd);
	/* draw map */
	for(int i = 0; i < 8; i++) {
		for(int j = 0; j < 8; j++) {
			col = (map[i][j]) ? 0xff : 0;
			for(int k = 0; k < 3; k++)
				color[k] = col;
			fb_draw_rect(&fb, j * rect.w + 1, i * rect.h + 1, 
					rect.w - 2, rect.h - 2, color);
		}
	}
	enter_raw_mode();
	exit_raw_mode(&orig_termios);
	fb_close(&fb, &fd);
	return 0;
}
