#include <fb.h>
#include <termios.h>
#include <stdio.h>
#include <unistd.h>

#define PLAYER_INCREMENT 0.2

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

void enter_raw_mode(struct termios *t)
{
	struct termios raw;
	tcgetattr(STDIN_FILENO, t);
	raw = *t;
	raw.c_lflag &= ~(ICANON | ECHO);
	raw.c_oflag &= ~(OPOST);
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void exit_raw_mode(struct termios *t) {
	tcsetattr(STDIN_FILENO, TCSAFLUSH, t);
}

/* draw map, we won't need this later */
void draw_map_2d(unsigned char *fb)
{
	const unsigned int rect = 200;
	color_t color;
	unsigned char col;
	for(int i = 0; i < 8; i++) {
		for(int j = 0; j < 8; j++) {
			col = (map[i][j]) ? 0xff : 0;
			for(int k = 0; k < 3; k++)
				color[k] = col;
			fb_draw_rect(&fb, j * rect, i * rect, rect, rect, color);
		}
	}
}

int main()
{
	int fd;
	unsigned char *fb;
	color_t color = {0x7f, 0x7f, 0x7f};
	struct termios orig_termios;
	int c;

	struct { unsigned int size; float x, y; } player = {
		.size = 20,
		.x = 5.0,
		.y = 5.0,
	};

	/* clear the screen */
	puts("\x1b[2J");
	/* hide the cursor */
	puts("\x1b[?25l");

	fb_init(&fb, &fd);

	draw_map_2d(fb);

	enter_raw_mode(&orig_termios);
	do {
		draw_map_2d(fb);
		fb_draw_rect(&fb, player.x * 200, 
				player.y * 200, 1, 1, color);
		c = getchar();
		/* convert to lowercase */
		if(c >= 'A' && c <= 'Z')
			c -= ('A' - 'a');
		/* wasd */
		switch(c) {
		case 'w':
			if(player.y < 0)
				break;
			player.y -= PLAYER_INCREMENT;
			break;
		case 'a':
			if(player.x < 0)
				break;
			player.x -= PLAYER_INCREMENT;
			break;
		case 's':
			if(player.y > 8)
				break;
			player.y += PLAYER_INCREMENT;
			break;
		case 'd':
			if(player.x > 8)
				break;
			player.x += PLAYER_INCREMENT;
			break;
		/* end the game if you pressed the ESC key */
		case 0x1b:
			goto end;
		default:
			break;
		}
	} while(1);
end:
	puts("\x1b[?25h");
	exit_raw_mode(&orig_termios);
	fb_close(&fb, &fd);
	return 0;
}
