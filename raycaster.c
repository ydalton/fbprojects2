#include <fb.h>
#include <math.h>
#include <termios.h>
#include <stdio.h>
#include <unistd.h>

#define PLAYER_INCREMENT 0.2
#define PLAYER_ROTATION_INCREMENT 0.1

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#define MAP_X 11
#define MAP_Y 11

char map[MAP_Y][MAP_X] = {
	{1,1,1,1,1,1,1,1,1,1,1},
	{1,1,0,0,0,0,0,0,0,1,1},
	{1,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,1},
	{1,1,1,0,0,0,0,0,1,1,1},
	{1,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,1},
	{1,1,0,0,0,0,0,0,0,1,1},
	{1,1,1,1,1,1,1,1,1,1,1},
};

struct { unsigned int size; float x, y, r; } player = {
	.size = 20,
	.x = 3.0,
	.y = 3.0,
	.r = 0.0,
};

color_t gray = {0x7f, 0x7f, 0x7f};
color_t brown = {0x7f, 0, 0};
color_t white = {0xff, 0xff, 0xff};

void enter_raw_mode(struct termios *t)
{
	struct termios raw;
	tcgetattr(STDIN_FILENO, t);
	raw = *t;
	raw.c_lflag &= ~(ICANON | ECHO);
	//raw.c_oflag &= ~(OPOST);
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void exit_raw_mode(struct termios *t) {
	tcsetattr(STDIN_FILENO, TCSAFLUSH, t);
}

/* draw map, we won't need this later */
void draw_map_2d(unsigned char *fb, unsigned int x, unsigned int y, unsigned int size)
{
	color_t color;
	unsigned char col;
	for(int i = 0; i < MAP_Y; i++) {
		for(int j = 0; j < MAP_Y; j++) {
			col = (map[i][j]) ? 0xff : 0;
			for(int k = 0; k < 3; k++)
				color[k] = col;
			fb_draw_rect(&fb, x + j * size, y + i * size, size, size, color);
		}
	}
}

float get_line_length(float x1, float y1, float x2, float y2)
{
	return sqrt(pow((x2-x1), 2) + pow((y2-y1),2));
}

/* i need to find a more efficient algorithm... */
float cast_ray(const float x, const float y, const float r) {
	float rx, ry, distance;
	const float inc = 0.05;

	rx = x;
	ry = y;
	distance = 0;

	while(!map[(int) ry][(int) rx]) {
		ry += inc * cos(r);
		rx += inc * sin(r);
		if(rx > MAP_X || ry > MAP_Y)
			return 20;
		distance += inc;
	}
	return distance;
}

int main()
{
	int fd;
	unsigned char *fb;
	struct termios orig_termios;
	int c;

	/* clear the screen */
	puts("\x1b[2J");
	/* hide the cursor */
	puts("\x1b[?25l");

	fb_init(&fb, &fd);


	enter_raw_mode(&orig_termios);

	float dist, size;
	const float fov = 0.5;
	int idx;
	color_t color;
	do {
		fb_draw_rect(&fb, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT/2, gray);
		fb_draw_rect(&fb, 0, WINDOW_HEIGHT/2, WINDOW_WIDTH, WINDOW_HEIGHT/2, brown);
		for(int i = 0; i < 2; i++) {
			idx = 0;
			for(float f = player.r - fov; f < (player.r + fov); f += ((2*fov)/WINDOW_WIDTH)) {
				dist = cast_ray(player.x, player.y, f);
				size = 500*(1/(sqrt(2 * dist)));
				if(size > WINDOW_HEIGHT)
					size = WINDOW_HEIGHT;
				//size *= tan(f - player.r);
				color[0] = size/5;
				color[1] = size/5;
				color[2] = size/5;
				fb_draw_line_vertical(&fb, idx, (WINDOW_HEIGHT - size)/2, size, color);
				idx++;
			}
		}
		/* draw map */
		draw_map_2d(fb, 1500, 0, 50);
		fb_draw_rect(&fb, 1500 + (player.x * 50), player.y * 50, 10, 10, brown);
		c = getchar();
		/* convert to lowercase */
		if(c >= 'A' && c <= 'Z')
			c -= ('A' - 'a');
		/* wasd */
		switch(c) {
		case 'w':
			player.y += PLAYER_INCREMENT * cos(player.r);
			player.x += PLAYER_INCREMENT * sin(player.r);
			break;
		case 'a':
			player.r -= PLAYER_ROTATION_INCREMENT;
			break;
		case 's':
			player.y -= PLAYER_INCREMENT * cos(player.r);
			player.x -= PLAYER_INCREMENT * sin(player.r);
			break;
		case 'd':
			player.r += PLAYER_ROTATION_INCREMENT;
			break;
		/* end the game if you pressed the q or ESC key */
		case 'q':
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
